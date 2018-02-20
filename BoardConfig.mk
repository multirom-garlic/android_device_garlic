#
# Copyright (C) 2018 The TwrpBuilder Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := device/YU/garlic

TARGET_BOARD_PLATFORM := msm8937
TARGET_BOOTLOADER_BOARD_NAME := garlic

# Recovery
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_BOOTIMAGE_PARTITION_SIZE  := 67108864
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864 
BOARD_FLASH_BLOCK_SIZE := 1000000
BOARD_HAS_NO_REAL_SDCARD := true
TW_EXCLUDE_SUPERSU := true

# Kernel
TARGET_KERNEL_SOURCE := kernel/yu/msm8937
TARGET_KERNEL_CONFIG := lineage_garlic_defconfig
BOARD_KERNEL_CMDLINE := g_android.luns=1 g_android.removable=1 g_android.cdrom=1 console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=qcom msm_rtb.filter=0x237 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 androidboot.bootdevice=7824900.sdhci earlycon=msm_hsl_uart,0x78B0000 androidboot.selinux=permissive
BOARD_KERNEL_BASE := 0x80000000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb

# MR config. MultiROM also uses parts of TWRP config
TARGET_RECOVERY_IS_MULTIROM := true
MR_NO_KEXEC := enabled
MR_ALLOW_NKK71_NOKEXEC_WORKAROUND := true
MR_CONTINUOUS_FB_UPDATE := true
MR_DPI := xhdpi
MR_DPI_FONT := 340
MR_USE_MROM_FSTAB := true
MR_FSTAB := $(LOCAL_PATH)/multirom/mrom.fstab
MR_INPUT_TYPE := type_b
MR_INIT_DEVICES := $(LOCAL_PATH)/multirom/mr_init_devices.c
MR_KEXEC_MEM_MIN := 0x00200000
MR_KEXEC_DTB := true
MR_DEVICE_HOOKS := $(LOCAL_PATH)/multirom/mr_hooks.c
MR_DEVICE_HOOKS_VER := 1
MR_DEVICE_VARIANTS := garlic

# bootmenu
DEVICE_RESOLUTION := 1080x1920
MR_PIXEL_FORMAT := "RGBA_8888"
RECOVERY_GRAPHICS_USE_LINELENGTH := true
MR_DEV_BLOCK_BOOTDEVICE := true

#Force populating from the emmc
MR_POPULATE_BY_NAME_PATH := "/dev/block/platform/soc/7824900.sdhci/by-name"

include $(LOCAL_PATH)/multirom/version/MR_REC_VERSION.mk
MR_REC_VERSION := $(shell date -u +%Y%m%d)-01
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x01000000 --tags_offset 0x00000100 --board mrom$(MR_REC_VERSION)

include device/generic/twrpbuilder/BoardConfig64.mk
