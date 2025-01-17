LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

#-----------------------------------------------------------
$(call config-custom-folder,hal:hal)

#-----------------------------------------------------------
LOCAL_SRC_FILES += 

#-----------------------------------------------------------
LOCAL_C_INCLUDES += 

#-----------------------------------------------------------
LOCAL_STATIC_LIBRARIES += 
#
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.camera
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.flashlight
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.lens
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.imgsensor
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.eeprom
LOCAL_WHOLE_STATIC_LIBRARIES += libcameracustom.cam_cal

#-----------------------------------------------------------
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_SHARED_LIBRARIES += libcutils

#-----------------------------------------------------------
#LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcameracustom

#-----------------------------------------------------------
include $(BUILD_SHARED_LIBRARY)


################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

