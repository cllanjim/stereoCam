# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.

# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.


################################################################################
#
################################################################################
ifneq (,$(filter $(strip $(TARGET_BOARD_PLATFORM)), mt6797 mt6757))

#-----------------------------------------------------------
# CCT Tool
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libmtkcam_cct
MY_LOCAL_SRC_FILES +=
MY_LOCAL_C_INCLUDES +=
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# Hal3A
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3
MY_LOCAL_SRC_FILES += aaa/register_Hal3A.cpp
MY_LOCAL_C_INCLUDES += $(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/isp_tuning
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# HalFlash
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3
MY_LOCAL_SRC_FILES += aaa/register_HalFlash.cpp
MY_LOCAL_C_INCLUDES +=
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# IspMgrIf
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3
MY_LOCAL_SRC_FILES += aaa/register_IspMgrIf.cpp
MY_LOCAL_C_INCLUDES += $(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/isp_tuning
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# Sync3A
#-----------------------------------------------------------
ifeq ($(MTK_CAM_STEREO_CAMERA_SUPPORT),yes)

MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3
MY_LOCAL_SRC_FILES += aaa/register_Sync3A.cpp
MY_LOCAL_C_INCLUDES += $(MTK_PATH_CUSTOM_PLATFORM)/hal/inc/isp_tuning
MY_LOCAL_CFLAGS +=

endif

#-----------------------------------------------------------
# SwNR
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3
MY_LOCAL_SRC_FILES += aaa/register_SwNR.cpp
MY_LOCAL_C_INCLUDES += $(MTK_PATH_CUSTOM_PLATFORM)/hal/inc
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# DngInfo
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3.dng
MY_LOCAL_SRC_FILES += aaa/register_DngInfo.cpp
MY_LOCAL_C_INCLUDES +=
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# NvBufUtil
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3.nvram
MY_LOCAL_SRC_FILES += aaa/register_NvBufUtil.cpp
MY_LOCAL_C_INCLUDES += $(MTK_PATH_CUSTOM_PLATFORM)/hal/inc
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# LscTable
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.hal3a.v3.lsctbl
MY_LOCAL_SRC_FILES += aaa/register_LscTable.cpp
MY_LOCAL_C_INCLUDES +=
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
# Lcs Hal
#-----------------------------------------------------------
MY_LOCAL_SHARED_LIBRARIES += libcam.lcs
MY_LOCAL_SRC_FILES += aaa/register_lcs_hal.cpp
MY_LOCAL_C_INCLUDES +=
MY_LOCAL_CFLAGS +=

#-----------------------------------------------------------
#
#-----------------------------------------------------------

endif ### (,$(filter $(strip $(TARGET_BOARD_PLATFORM)), ...))

################################################################################
#
################################################################################
