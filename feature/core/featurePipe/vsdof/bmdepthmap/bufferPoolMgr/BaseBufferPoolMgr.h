/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

/**
 * @file BaseBufferPoolMgr.h
 * @brief base classes of BufferPoolMgr
*/
#ifndef _MTK_CAMERA_FEATURE_PIPE_DEPTH_MAP_BUFFERPOOL_MGR_H_
#define _MTK_CAMERA_FEATURE_PIPE_DEPTH_MAP_BUFFERPOOL_MGR_H_

// Standard C header file

// Android system/core header file
#include <utils/RefBase.h>
// mtkcam custom header file

// mtkcam global header file
#include <mtkcam/feature/stereo/pipe/IBMDepthMapPipe.h>
#include <mtkcam/feature/stereo/pipe/IDepthMapEffectRequest.h>

// Module header file
#include <featurePipe/core/include/ImageBufferPool.h>
#include <featurePipe/core/include/GraphicBufferPool.h>
#include <featurePipe/vsdof/util/TuningBufferPool.h>

// Local header file


/*******************************************************************************
* Namespace start.
********************************************************************************/
namespace NSCam {
namespace NSCamFeature {
namespace NSFeaturePipe_DepthMap_BM {

using namespace NSCam::NSCamFeature::NSFeaturePipe;

#define CREATE_IMGBUF_POOL(rBufPool, name, size, format, usage, bSingle) \
    MY_LOGD("Create ImageBufferPool:: "#name " size=%dx%d format=%x usage=%x single=%d", \
                size.w, size.h, format, usage, bSingle); \
    rBufPool = ImageBufferPool::create(name, size.w, size.h, format, usage, bSingle);

#define ALLOCATE_BUFFER_POOL(bufPool, size) \
    MY_LOGD("ALLOCATE_BUFFER_POOL~" #bufPool "~~~~~~~%d", size);\
    if(!bufPool->allocate(size))\
    { \
        MY_LOGE("Allocate "#bufPool" image buffer failed"); \
        return MFALSE; \
    }

/*******************************************************************************
* Enum Definition
********************************************************************************/
typedef enum eBufferPoolScenario
{
    eBUFFER_POOL_SCENARIO_PREVIEW,   /*!< indicate the pool is used for preview */
    eBUFFER_POOL_SCENARIO_RECORD,    /*!< indicate the pool is used for record */
    eBUFFER_POOL_SCENARIO_CAPTURE    /*!< indicate the pool is used for capture */
} BufferPoolScenario;

typedef enum eDepthBufferType
{
    eBUFFER_IMAGE,      /*!< indicate the image buffer */
    eBUFFER_GRAPHIC,    /*!< indicate the gralloc buffer */
    eBUFFER_TUNING,     /*!< indicate the tuning buffer */

    // any buffer need to be define above this line.
    eBUFFER_SIZE

} DepthBufferType;

/*******************************************************************************
* Type Definition
********************************************************************************/

/**
 * @brief Buffer id map to record which buffers are inside the EffectRequest
 *        for all scenarios or for specific scenario
 */
typedef KeyedVector<DepthMapBufferID, DepthNodeBufIOType> BufferIDMap;
typedef KeyedVector<BufferPoolScenario, BufferIDMap> RequestBufferMap;


/*******************************************************************************
* Class Definition
********************************************************************************/
class BaseBufferHandler;
/**
 * @class BaseBufferPoolMgr
 * @brief Base class of BufferPoolMgr
 */
class BaseBufferPoolMgr : public virtual android::VirtualLightRefBase
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Instantiation.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    BaseBufferPoolMgr() {}
    virtual ~BaseBufferPoolMgr() {}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  BaseBufferPoolMgr Public Operations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /**
     * @brief request buffer from the bufferPool belong to the buffer id
     * @param [in] id buffer id used to request buffer.
     * @return
     * - requested buffer from buffer pool
     */
    virtual SmartImageBuffer request(
                            DepthMapBufferID id,
                            BufferPoolScenario scen
                            ) = 0;

    /**
     * @brief request graphic buffer from the bufferPool belong to the buffer id
     * @param [in] id buffer id used to request buffer.
     * @return
     * - requested buffer from buffer pool
     */
    virtual SmartGraphicBuffer requestGB(
                            DepthMapBufferID id,
                            BufferPoolScenario scen
                            ) = 0;

    /**
     * @brief request tuning buffer from the bufferPool belong to the buffer id
     * @param [in] id buffer id used to request buffer.
     * @return
     * - requested buffer from buffer pool
     */
    virtual SmartTuningBuffer requestTB(
                            DepthMapBufferID id,
                            BufferPoolScenario scen
                            ) = 0;

    /**
     * @brief create the BufferPoolHandler of this mgr
     * @return
     * - BufferHandler pointer
     */
    virtual sp<BaseBufferHandler> createBufferPoolHandler() = 0;

    /**
     * @brief query the buffer type of the buffer under specific scenario
     * @param [in] bid buffer id
     * @param [in] scen buffer scenario
     * @param [out] rOutBufType buffer type
     * @return
     * - MTRUE indicates success.
     * - MFALSE indicates the buffer is not working buffer.
     */
    virtual MBOOL queryBufferType(
                            DepthMapBufferID bid,
                            BufferPoolScenario scen,
                            DepthBufferType& rOutBufType) = 0;

    /**
     * @brief get the buffer pool for the bufferID
     * @param [int] bufferID buffer id
     * @return
     * - ImageBufferPool for the buffer id
     */
    virtual sp<ImageBufferPool> getImageBufferPool(DepthMapBufferID bufferID) = 0;

};


}; // NSFeaturePipe_DepthMap_BM
}; // NSCamFeature
}; // NSCam

#endif