ifdef ARSDK_BUILD_FOR_APP

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libressl
LOCAL_DESCRIPTION := LibreSSL
LOCAL_CATEGORY_PATH := libs

LOCAL_AUTOTOOLS_VERSION := 2.2.1
LOCAL_AUTOTOOLS_ARCHIVE := libressl-$(LOCAL_AUTOTOOLS_VERSION).tar.gz
LOCAL_AUTOTOOLS_SUBDIR := libressl-$(LOCAL_AUTOTOOLS_VERSION)
LOCAL_AUTOTOOLS_PATCHES := \
	libressl-2.2.1.patch

LOCAL_AUTOTOOLS_CONFIGURE_ARGS := \
	--enable-static \
	--disable-shared \
	INSTALL="/usr/bin/install -C"

ifeq ("$(TARGET_OS_FLAVOUR)","android")

LOCAL_AUTOTOOLS_CONFIGURE_ARGS += \
	LIBS=" -llog -lz"

endif

# User define command to be launch before configure step.
# Generates files used by configure
define LOCAL_AUTOTOOLS_CMD_POST_UNPACK
	$(Q) cd $(PRIVATE_SRC_DIR) && autoreconf -fiv
endef

# Don't redefine alloc functions
LOCAL_AUTOTOOLS_CONFIGURE_ENV := \
	ac_cv_func_malloc_0_nonnull=yes \
	ac_cv_func_realloc_0_nonnull=yes \

include $(BUILD_AUTOTOOLS)

endif
