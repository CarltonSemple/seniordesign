#This file in automatically included by alchemy.

include $(TARGET_CONFIG_DIR)/../../../common/config/iOS/product.mk

TARGET_AR := $(shell xcrun --find --sdk iphonesimulator ar)
TARGET_OS_FLAVOUR := iphonesimulator

# if you want to build for i386, you can use TARGET_ARCH = x86 instead
TARGET_ARCH := x64

TARGET_FORCE_STATIC := 1
TARGET_GLOBAL_CFLAGS += -std=gnu99
