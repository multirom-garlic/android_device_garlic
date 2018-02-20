#include <stdlib.h>

// These are paths to folders in /sys which contain "uevent" file
// need to init this device.
// MultiROM needs to init framebuffer, mmc blocks, input devices,
// some ADB-related stuff and USB drives, if OTG is supported
// You can use * at the end to init this folder and all its subfolders
const char *mr_init_devices[] =
{

    "/sys/class/graphics/fb0",
    "/sys/class/graphics/fb1",
    "/sys/devices/virtual/graphics/fb0",
    "/sys/devices/virtual/graphics/fb1",

    "/sys/block/mmcblk0/*",
    "/sys/block/mmcblk1/*",

    // boot and data
    "/sys/block/mmcblk0/mmcblk0p21", //boot
    "/sys/block/mmcblk0/mmcblk0p22", //data

    // system ,cache and persist
    "/sys/block/mmcblk0/mmcblk0p24", //system
    "/sys/block/mmcblk0/mmcblk0p25", //cache
    "/sys/block/mmcblk0/mmcblk0p26", // persist

    "/sys/bus/mmc*",
    "/sys/bus/mmc/drivers/mmcblk",
    "/sys/bus/sdio/drivers",
    "/sys/bus/sdio/drivers/bcmsdh_sdmmc",
    "/sys/module/mmc_core",
    "/sys/module/mmcblk",

    // for input
    "/sys/devices/virtual/input*",
    "/sys/devices/virtual/input/input0",
    "/sys/devices/virtual/misc/uinput",
    "/sys/devices/soc/soc:gpio_keys/input/input6*",
    "/sys/devices/virtual/misc/uinput",
    "/sys/devices/virtual/misc/uinput/*",
    "/sys/devices/soc/78b7000.i2c/i2c-3/3-0038/input*"
    "/sys/devices/soc/78b7000.i2c/i2c-3/3-0038/input/input1*"
    "/sys/class/input/input0",
    "/sys/class/input/input0/event0",
    "/sys/class/input/input1",
    "/sys/class/input/input1/event1",
    "/sys/class/input/input2",
    "/sys/class/input/input2/event2",
    "/sys/class/input/input3",
    "/sys/class/input/input3/event3",
    "/sys/class/input/input4",
    "/sys/class/input/input4/event4",
    "/sys/class/input/input5",
    "/sys/class/input/input5/event5",
    "/sys/class/input/input6",
    "/sys/class/input/input6/event6",
    "/sys/class/input/input7",
    "/sys/class/input/input7/event7",
    "/sys/class/input/input8",
    "/sys/class/input/input8/event8",

    // for adb
    "/sys/class/tty/ptmx",
    "/sys/class/misc/android_adb",
    "/sys/class/android_usb/android0*",
    "/sys/class/android_usb/android0/f_adb",
    "/sys/bus/platform/drivers/android_usb",
    "/sys/bus/usb",

    // USB Drive
    "/sys/bus/platform/drivers/xhci-hcd*",

    // Encryption
    "/sys/devices/virtual/misc/device-mapper",
    "/sys/devices/virtual/misc/ion",
    "/sys/devices/virtual/qseecom/qseecom",

    // for qualcomm overlay - /dev/ion
    "/sys/devices/virtual/misc/ion",

    // exfat requires fuse device
    "/sys/devices/virtual/misc/fuse",
    NULL
};
