###############################################################################
## @file product.mk
##
##
## Product common alchemy variables.
###############################################################################

# Product common config dir
COMMON_CONFIG_ANDROID_DIR := $(call my-dir)

include $(COMMON_CONFIG_ANDROID_DIR)/../common.mk

$(call check_defined, ANDROID_NDK_PATH, path to Android NDK)
$(call check_defined, ANDROID_SDK_PATH, path to Android SDK)

TARGET_OS := linux
TARGET_OS_FLAVOUR := android
TARGET_LIBC := bionic

TARGET_ANDROID_APILEVEL := 14
TARGET_ANDROID_NDK := $(ANDROID_NDK_PATH)
TARGET_ANDROID_SDK := $(ANDROID_SDK_PATH)

TARGET_DEFAULT_LIB_DESTDIR := usr/lib
