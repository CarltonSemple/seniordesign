###############################################################################
## @file product.mk
##
##
## Product common alchemy variables.
###############################################################################

# Product common config dir
COMMON_CONFIG_UNIX_DIR := $(call my-dir)

include $(COMMON_CONFIG_UNIX_DIR)/../common.mk

prebuilt.json.override := 1
prebuilt.ncurses.override := 1
