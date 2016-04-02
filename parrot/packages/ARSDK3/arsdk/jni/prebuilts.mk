LOCAL_PATH := $(call my-dir)

# Rename armeabi-v7a to armeabi_v7a, note there should be no space in subst arguments
MY_ARCH_ABI := $(subst -,_,$(TARGET_ARCH_ABI))

REL_PATH_LIBS := ../../../../out/Android-$(MY_ARCH_ABI)/staging/usr/lib
REL_PATH_INCLUDE := ../../../../out/Android-$(MY_ARCH_ABI)/staging/usr/include

# libARCommands
include $(CLEAR_VARS)

LOCAL_MODULE := libARCommands-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarcommands.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARController
include $(CLEAR_VARS)

LOCAL_MODULE := libARController-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarcontroller.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARDiscovery
include $(CLEAR_VARS)

LOCAL_MODULE := libARDiscovery-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libardiscovery.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARNetwork
include $(CLEAR_VARS)

LOCAL_MODULE := libARNetwork-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarnetwork.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARNetworkAL
include $(CLEAR_VARS)

LOCAL_MODULE := libARNetworkAL-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarnetworkal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARSAL
include $(CLEAR_VARS)

LOCAL_MODULE := libARSAL-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarsal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARStream
include $(CLEAR_VARS)

LOCAL_MODULE := libARStream-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarstream.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARStream2
include $(CLEAR_VARS)

LOCAL_MODULE := libARStream2-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarstream2.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARDataTransfer
include $(CLEAR_VARS)

LOCAL_MODULE := libARDataTransfer-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libardatatransfer.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARMavlink
include $(CLEAR_VARS)

LOCAL_MODULE := libARMavlink-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarmavlink.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARMedia
include $(CLEAR_VARS)

LOCAL_MODULE := libARMedia-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarmedia.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARUpdater
include $(CLEAR_VARS)

LOCAL_MODULE := libARUpdater-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarupdater.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# libARUtils
include $(CLEAR_VARS)

LOCAL_MODULE := libARUtils-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libarutils.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# json
include $(CLEAR_VARS)

LOCAL_MODULE := json-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libjson.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

# curl
include $(CLEAR_VARS)

LOCAL_MODULE := curl-prebuilt
LOCAL_SRC_FILES := $(REL_PATH_LIBS)/libcurl.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(REL_PATH_INCLUDE)

include $(PREBUILT_SHARED_LIBRARY)

