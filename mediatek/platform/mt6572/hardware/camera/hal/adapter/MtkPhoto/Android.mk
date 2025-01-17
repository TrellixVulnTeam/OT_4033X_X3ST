LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

#-----------------------------------------------------------
LOCAL_SRC_FILES += $(call all-c-cpp-files-under, .)
LOCAL_SRC_FILES += $(call all-c-cpp-files-under, ./Preview)
#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(TOP)/bionic
LOCAL_C_INCLUDES += $(TOP)/external/stlport/stlport
#
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/frameworks-ext/av/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/custom/common/kernel/imgsensor/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc/common/camutils
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc/common/camutils
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/adapter
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/adapter/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/inc
#
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/m4u

#-----------------------------------------------------------
PLATFORM_VERSION_MAJOR := $(word 1,$(subst .,$(space),$(PLATFORM_VERSION)))
LOCAL_CFLAGS += -DPLATFORM_VERSION_MAJOR=$(PLATFORM_VERSION_MAJOR)

#-----------------------------------------------------------
LOCAL_WHOLE_STATIC_LIBRARIES += libcam.camadapter.mtkphoto.state
#
LOCAL_STATIC_LIBRARIES += 

#-----------------------------------------------------------
LOCAL_MODULE := libcam.camadapter.mtkphoto

#-----------------------------------------------------------
include $(BUILD_STATIC_LIBRARY)


################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

