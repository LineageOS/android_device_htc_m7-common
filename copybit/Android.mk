# Copyright (C) 2008 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)

#Common headers
common_includes := hardware/qcom/display-caf/libgralloc
common_includes += hardware/qcom/display-caf/libgenlock
common_includes += hardware/qcom/display-caf/liboverlay
common_includes += hardware/qcom/display-caf/libqdutils
common_includes += hardware/qcom/display-caf/libhwcomposer
common_includes += hardware/qcom/display-caf/libexternal
common_includes += hardware/qcom/display-caf/libqservice
common_includes += $(LOCAL_PATH)

#Common libraries external to display-caf HAL
common_libs := liblog libutils libcutils libhardware

#Common C flags
common_flags := -DDEBUG_CALC_FPS -Wno-missing-field-initializers
common_flags += -Werror

ifeq ($(ARCH_ARM_HAVE_NEON),true)
    common_flags += -D__ARM_HAVE_NEON
endif

include $(CLEAR_VARS)

LOCAL_MODULE                  := copybit.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_PATH             := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_TAGS             := optional
LOCAL_C_INCLUDES              := $(common_includes)
LOCAL_SHARED_LIBRARIES        := $(common_libs) libdl libmemalloc
LOCAL_CFLAGS                  := $(common_flags) -DLOG_TAG=\"qdcopybit\"

LOCAL_CFLAGS += -DCOPYBIT_Z180=1 -DC2D_SUPPORT_DISPLAY=1
LOCAL_SRC_FILES := copybit_c2d.cpp software_converter.cpp

include $(BUILD_SHARED_LIBRARY)

