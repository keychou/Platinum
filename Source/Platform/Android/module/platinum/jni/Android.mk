LOCAL_PATH := $(call my-dir)

PLT_ROOT := $(LOCAL_PATH)/../../../../../..
PLT_SRC_ROOT := $(PLT_ROOT)/Source

ifeq ($(NDK_DEBUG),1)
PLT_PREBUILT_PATH := ../../../../../../Build/Targets/arm-android-linux/Debug
else
PLT_PREBUILT_PATH := ../../../../../../Build/Targets/arm-android-linux/Release
endif

include $(CLEAR_VARS)
LOCAL_MODULE := Platinum
LOCAL_SRC_FILES := $(PLT_PREBUILT_PATH)/libPlatinum.a
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Platinum
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Core
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaConnect
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaServer
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Devices/MediaRenderer
LOCAL_EXPORT_C_INCLUDES += $(PLT_SRC_ROOT)/Extras
LOCAL_C_INCLUDES += $(PLT_ROOT)/ThirdParty/Neptune/Source/Core/

$(warning "LOCAL_C_INCLUDES =:"$(LOCAL_C_INCLUDES))

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := Neptune
LOCAL_SRC_FILES := $(PLT_PREBUILT_PATH)/libNeptune.a
LOCAL_EXPORT_C_INCLUDES += $(PLT_ROOT)/ThirdParty/Neptune/Source/Core/
include $(PREBUILT_STATIC_LIBRARY)

ifneq ($(NPT_CONFIG_NO_SSL),1)
include $(CLEAR_VARS)
LOCAL_MODULE := axTLS
LOCAL_SRC_FILES := $(PLT_PREBUILT_PATH)/libaxTLS.a
include $(PREBUILT_STATIC_LIBRARY)
endif

include $(CLEAR_VARS)
LOCAL_MODULE     := platinum-jni

MY_CPP_PATH  := $(LOCAL_PATH)/
My_All_Files := $(shell find $(MY_CPP_PATH)/.)
My_All_Files := $(My_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_CPP_LIST  := $(filter %.cpp %.c,$(My_All_Files)) 
MY_CPP_LIST  := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(MY_CPP_LIST)

LOCAL_LDLIBS     += -llog
LOCAL_LDLIBS     += -landroid

LOCAL_CFLAGS += -DNPT_CONFIG_ENABLE_LOGGING

LOCAL_STATIC_LIBRARIES := Platinum
LOCAL_STATIC_LIBRARIES += Neptune

ifneq ($(NPT_CONFIG_NO_SSL),1)
LOCAL_STATIC_LIBRARIES += axTLS
endif

include $(BUILD_SHARED_LIBRARY)
