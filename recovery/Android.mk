LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE		:= lpm.rc
LOCAL_MODULE_TAGS	:= optional eng
LOCAL_MODULE_CLASS	:= ETC
LOCAL_SRC_FILES		:= etc/lpm.rc
LOCAL_MODULE_PATH	:= $(TARGET_ROOT_OUT)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE		:= choice_fn
LOCAL_MODULE_TAGS	:= optional eng
LOCAL_MODULE_CLASS	:= ETC
LOCAL_SRC_FILES		:= sbin/choice_fn
LOCAL_MODULE_PATH	:= $(TARGET_RECOVERY_ROOT_OUT)/sbin
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE		:= offmode_charging
LOCAL_MODULE_TAGS	:= optional eng
LOCAL_MODULE_CLASS	:= ETC
LOCAL_SRC_FILES		:= sbin/offmode_charging
LOCAL_MODULE_PATH	:= $(TARGET_RECOVERY_ROOT_OUT)/sbin
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
CHARGER_BIN := charger
CHARGER_BIN_SYMLINK := $(addprefix $(TARGET_RECOVERY_ROOT_OUT)/sbin/,$(notdir $(CHARGER_BIN)))
$(CHARGER_BIN_SYMLINK): $(LOCAL_INSTALLED_MODULE)
	@echo "Recovery charger link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CHARGER_BIN_SYMLINK)
