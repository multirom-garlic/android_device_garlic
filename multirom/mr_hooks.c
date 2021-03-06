/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <private/android_filesystem_config.h>

#include <log.h>
#include <util.h>
#include <multirom.h>

#define GATEKEEPER_PATH "/system/vendor/lib64/hw/gatekeeper.msm8937.so"
#define GATEKEEPER_DATA_DIR "/data/misc/gatekeeper"
#define GATEKEEPER_COLDBOOT_PATH "/data/misc/gatekeeper/.coldboot"

#if MR_DEVICE_HOOKS >= 1

int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    // wiping the data of a primary or secondary ROM causes the ROM to delete
    // all lockscreen accounts from the gatekeeper on next boot, preventing
    // the user from logging into the other ROMs.
    // To work around this, create the .coldboot file to prevent the wipe.
    if (access(GATEKEEPER_COLDBOOT_PATH, F_OK) == -1)
    {
        // the permission should be fixed during the first boot
        int err = mkdir_recursive(GATEKEEPER_DATA_DIR, 0700);
        if (err)
            ERROR("failed to mkdir " GATEKEEPER_DATA_DIR ": %s", strerror(err));
        else
        {
            int fd = open(GATEKEEPER_COLDBOOT_PATH, O_WRONLY|O_TRUNC|O_CREAT,
                S_IRUSR|S_IWUSR);
            if (fd < 0)
                ERROR("failed to open " GATEKEEPER_COLDBOOT_PATH ": %s",
                    strerror(errno));
            else
            {
                fchown(fd, AID_SYSTEM, AID_SYSTEM);
                close(fd);
            }
        }
    }
    if (type == ROM_DEFAULT)
        return 0;
    // Delete the old gatekeeper workaround
    // remove after people stop using Beta 1-4
    remove(GATEKEEPER_PATH);
    return 0;
}
#endif /* MR_DEVICE_HOOKS >= 1 */


#if MR_DEVICE_HOOKS >= 2
void mrom_hook_before_fb_close(void)
{
}
#endif /* MR_DEVICE_HOOKS >= 2 */


#if MR_DEVICE_HOOKS >= 3
void tramp_hook_before_device_init(void)
{
}
#endif /* MR_DEVICE_HOOKS >= 3 */

#if MR_DEVICE_HOOKS >= 4
int mrom_hook_allow_incomplete_fstab(void)
{
    return 0;
}
#endif

#if MR_DEVICE_HOOKS >= 5

void mrom_hook_fixup_bootimg_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
}

int mrom_hook_has_kexec(void)
{
    // check for fdt blob
    static const char *checkfile = "/sys/firmware/fdt";
    if(access(checkfile, R_OK) < 0)
    {
        ERROR("%s was not found!\n", checkfile);
        return 0;
    }
    return 1;
}
#endif

#if MR_DEVICE_HOOKS >= 6
static int fork_and_exec(char **cmd, char** env)
{
    pid_t pID = fork();
    if(pID == 0)
    {
        stdio_to_null();
        setpgid(0, getpid());
        execve(cmd[0], cmd, env);
        ERROR("Failed to exec %s: %s\n", cmd[0], strerror(errno));
        _exit(127);
    }
    return pID;
}

static int qseecomd_pid = -1;

void tramp_hook_encryption_setup(void)
{
    // start qseecomd
    char* cmd[] = {"/mrom_enc/qseecomd", NULL};
    char* env[] = {"LD_LIBRARY_PATH=/mrom_enc", NULL};
    // setup links and permissions based on TWRP's init.recovery.angler.rc
    symlink("/dev/block/platform/soc.0/f9824900.sdhci", "/dev/block/bootdevice");
    chmod("/dev/qseecom", 0660);
    chown("/dev/qseecom", AID_SYSTEM, AID_DRMRPC);
    chown("/dev/ion", AID_SYSTEM, AID_SYSTEM);
    chmod("/mrom_enc/qseecomd", 0755);
    qseecomd_pid = fork_and_exec(cmd, env);
    if (qseecomd_pid == -1)
        ERROR("Failed to fork for qseecomd; should never happen!\n");
    else
        INFO("qseecomd started: pid=%d\n", qseecomd_pid);
}
void tramp_hook_encryption_cleanup(void)
{
    struct stat info;
    if (qseecomd_pid != -1)
    {
        kill(-qseecomd_pid, SIGTERM); // kill the entire process group
        waitpid(qseecomd_pid, NULL, 0);
    }
    // make sure we're removing our symlink
    if (lstat("/dev/block/bootdevice", &info) >= 0 && S_ISLNK(info.st_mode))
        remove("/dev/block/bootdevice");
    INFO("cleaned up after qseecomd\n");
}

static void replace_tag(char *cmdline, size_t cap, const char *tag, const char *what)
{
    char *start, *end;
    char *str = cmdline;
    char *str_end = str + strlen(str);
    size_t replace_len = strlen(what);

    while((start = strstr(str, tag)))
    {
        end = strstr(start, " ");
        if(!end)
            end = str_end;
        else if(replace_len == 0)
            ++end;

        if(end != start)
        {

            size_t len = str_end - end;
            if((start - cmdline)+replace_len+len > cap)
                len = cap - replace_len - (start - cmdline);
            memmove(start+replace_len, end, len+1); // with \0
            memcpy(start, what, replace_len);
        }

        str = start+replace_len;
    }
}

void mrom_hook_fixup_full_cmdline(char *bootimg_cmdline, size_t bootimg_cmdline_cap)
{
    // force dm-verity to Logging mode to prevent scary "your phone is corrupt" message
    replace_tag(bootimg_cmdline, bootimg_cmdline_cap, "androidboot.veritymode=", "androidboot.veritymode=logging");
}
#endif
