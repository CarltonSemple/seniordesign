###############################################################################
## @file product.mk
##
##
## Product common alchemy variables.
###############################################################################

# Product common config dir
COMMON_CONFIG_IOS_DIR := $(call my-dir)

include $(COMMON_CONFIG_IOS_DIR)/../common.mk

TARGET_OS := darwin
TARGET_LIBC := darwin
TARGET_IPHONE_VERSION := 7.0
TARGET_GLOBAL_OBJCFLAGS += -fobjc-arc
