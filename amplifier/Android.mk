LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
	liblog libutils libtinyalsa

LOCAL_C_INCLUDES := \
	external/tinyalsa/include \
	hardware/libhardware/include

LOCAL_SRC_FILES := \
	rt5501.c \
	tfa9887.c \
	audio_amplifier.c

LOCAL_MODULE := audio_amplifier.msm8960
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
