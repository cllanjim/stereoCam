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

#define LOG_TAG "MtkCam/PipelineMgr"
//
#include "MyUtils.h"
//
#include <mtkcam/v3/pipeline/IPipelineModel.h>
#include "PipelineModelMgr.h"
#include <mtkcam/v3/pipeline/IPipelineFrameNumberGenerator.h>
#include <mtkcam/v3/pipeline/IPipelineResourceMgr.h>
#include <mtkcam/v3/hwpipeline/PipelineModelFactory.h>
#include <mtkcam/metadata/IMetadataConverter.h>
//
#include <mtkcam/metadata/client/mtk_metadata_tag.h>
#include <mtkcam/v3/utils/FeatureMgr/FeatureMgr.h>
using namespace android;
using namespace NSCam;
using namespace NSCam::v3;
using namespace NSCam::v3::NSPipelineFactory;


/******************************************************************************
 *
 ******************************************************************************/
#define MY_LOGV(fmt, arg...)        CAM_LOGV("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)        CAM_LOGD("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)        CAM_LOGI("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)        CAM_LOGW("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)        CAM_LOGE("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGA(fmt, arg...)        CAM_LOGA("[%s] " fmt, __FUNCTION__, ##arg)
#define MY_LOGF(fmt, arg...)        CAM_LOGF("[%s] " fmt, __FUNCTION__, ##arg)
//
#define MY_LOGV_IF(cond, ...)       do { if ( (cond) ) { MY_LOGV(__VA_ARGS__); } }while(0)
#define MY_LOGD_IF(cond, ...)       do { if ( (cond) ) { MY_LOGD(__VA_ARGS__); } }while(0)
#define MY_LOGI_IF(cond, ...)       do { if ( (cond) ) { MY_LOGI(__VA_ARGS__); } }while(0)
#define MY_LOGW_IF(cond, ...)       do { if ( (cond) ) { MY_LOGW(__VA_ARGS__); } }while(0)
#define MY_LOGE_IF(cond, ...)       do { if ( (cond) ) { MY_LOGE(__VA_ARGS__); } }while(0)
#define MY_LOGA_IF(cond, ...)       do { if ( (cond) ) { MY_LOGA(__VA_ARGS__); } }while(0)
#define MY_LOGF_IF(cond, ...)       do { if ( (cond) ) { MY_LOGF(__VA_ARGS__); } }while(0)

#if 0
#define FUNC_START     MY_LOGD("+")
#define FUNC_END       MY_LOGD("-")
#else
#define FUNC_START
#define FUNC_END
#endif

/******************************************************************************
 *
 ******************************************************************************/
#define MAX_PIPELINEMODEL_INFLIGHT      (3)

/******************************************************************************
 *
 ******************************************************************************/
sp<IPipelineModelMgr>
IPipelineModelMgr::
create(
    MINT32 const openId,
    android::wp<IPipelineModelMgr::IAppCallback>const& pAppCallback
)
{
    return new PipelineModelMgr(openId, pAppCallback);
}

/******************************************************************************
 *
 ******************************************************************************/
PipelineModelMgr::
PipelineModelMgr(
    MINT32 const openId,
    android::wp<IPipelineModelMgr::IAppCallback>const& pAppCallback
)
    : mOpenId(openId)
    , mConfigParams()
    , mpAppCallback(pAppCallback)
    , mPipelineLock()
    , mpPipelineFrameNumberGenerator(
        IPipelineFrameNumberGenerator::create()
    )
{
    FUNC_START;
    //
    Mutex::Autolock _l(mPipelineLock);
    vpPipelineModel.setCapacity(MAX_PIPELINEMODEL_INFLIGHT);
    //
    sp<IPipelineResourceMgr> handle = IPipelineResourceMgr::create(openId);
    mpPipelineResourceMgr = PipelineResourceMgr::getInstance(openId);
    //
    FUNC_END;
}

/******************************************************************************
 *
 ******************************************************************************/
PipelineModelMgr::
~PipelineModelMgr()
{
    FUNC_START;
    //
    Mutex::Autolock _l(mPipelineLock);
    for ( size_t i=0; i<vpPipelineModel.size(); i++ ) {
        sp<IPipelineModel> pPipeline = vpPipelineModel.getPipelineModel(i);
        if ( pPipeline != 0 ) {
            MERROR err = pPipeline->beginFlush();
            MY_LOGW_IF(OK!=err, "IPipelineModel::flush err:%d(%s)", err, ::strerror(-err));
            //
            pPipeline->endFlush();
        }
    }
    //
    //reset FeatureMgr
    NSCam::v3::Utils::FeatureMgr* pFeatureMgr = NSCam::v3::Utils::FeatureMgr::getFeatureMgrSingleInst();
    pFeatureMgr->setCameraMode(0);
    MY_LOGD("reset pFeatureMgr CameraMode to 0");
    vpPipelineModel.clear();
    CAM_TRACE_INT("In-flight Pipeline", vpPipelineModel.size());
    //
    FUNC_END;
}

/******************************************************************************
 *
 ******************************************************************************/
char const*
PipelineModelMgr::
getName()
const
{
    Mutex::Autolock _l(mPipelineLock);
    if ( 1 > vpPipelineModel.size() )
        return NULL;
    sp<IPipelineModel> const pPipeline =  vpPipelineModel.getPipelineModel(0);
    if ( ! pPipeline.get() )
        return NULL;
    //
    return pPipeline->getName();
}

/******************************************************************************
 *
 ******************************************************************************/
MINT32
PipelineModelMgr::
getOpenId()
const
{
    Mutex::Autolock _l(mPipelineLock);
    if ( 1 > vpPipelineModel.size() )
        return -1;
    sp<IPipelineModel> const pPipeline =  vpPipelineModel.getPipelineModel(0);
    if ( ! pPipeline.get() )
        return -1;
    //
    return pPipeline->getOpenId();

}

/******************************************************************************
*
******************************************************************************/
MERROR
PipelineModelMgr::
beginFlush()
{
    FUNC_START;
    //
    MERROR err = OK;
    Mutex::Autolock _l(mPipelineLock);
    //
    if ( 1 > vpPipelineModel.size() ) {
        FUNC_END;
        return err;
    }
    //
    MY_LOGD("Begin flush of inflight pipelineS...");
    for ( size_t i=0; i<vpPipelineModel.size(); i++ ) {
        sp<IPipelineModel> pPipeline = vpPipelineModel.getPipelineModel(i);
        if ( pPipeline.get() ) {
            MY_LOGD("begin flush %d/%d pipeline", i, vpPipelineModel.size());
            // workaround: flush latest pipeline w/ p1 permission
            err = pPipeline->beginFlush();
            MY_LOGW_IF(OK!=err, "IPipelineModel::flush err:%d(%s)", err, ::strerror(-err));
        }
        else {
            MY_LOGD("WTF");
        }
    }
    //
    FUNC_END;
    return err;
}

/******************************************************************************
*
******************************************************************************/
MVOID
PipelineModelMgr::
endFlush()
{
    FUNC_START;
    //
    Mutex::Autolock _l(mPipelineLock);
    if ( 1 > vpPipelineModel.size() ) {
        FUNC_END;
        return;
    }
    //
    for ( size_t i=0; i<vpPipelineModel.size(); i++ ) {
        sp<IPipelineModel> pPipeline = vpPipelineModel.getPipelineModel(i);
        if ( pPipeline != 0 ) {
              pPipeline->endFlush();
        }
    }
    //
    FUNC_END;
}

/******************************************************************************
 *
 ******************************************************************************/
MERROR
PipelineModelMgr::
configurePipeline(
    ConfigurePipelineMgr::Params& rParams
)
{
    FUNC_START;

    CAM_TRACE_CALL();
    //
    MERROR err = MFALSE;
    // have flushed previous requests in Cam3Device
    sp<IPipelineModel> pPipeline = NULL;
    //
    //ConfigurePipeline::Params params;
    mConfigParams.openId = rParams.openId;
    //
    ConfigurePipeline::ImageStreamsT                vImageStreamsDst;
    ConfigurePipeline::MetaStreamsT                 vMetaStreamsDst;
    ConfigurePipeline::MinFrameDurationT            vMinFrameDurationDst;
    ConfigurePipeline::StallFrameDurationT          vStallFrameDurationDst;
    ConfigurePipeline::ImageStreamsT const&         vImageStreamsSrc = *rParams.pvImageStreams;
    ConfigurePipeline::MetaStreamsT  const&         vMetaStreamsSrc  = *rParams.pvMetaStreams;
    ConfigurePipeline::MinFrameDurationT const&     vMinFrameDurationSrc = *rParams.pvMinFrameDuration;
    ConfigurePipeline::StallFrameDurationT  const&  vStallFrameDurationSrc  = *rParams.pvStallFrameDuration;

#define _CLONE_(src, dst) \
            do  { \
                dst.setCapacity(src.size()); \
                dst.clear(); \
                for (size_t i = 0; i < src.size(); i++) { \
                    dst.add(src.keyAt(i), src.valueAt(i)); \
                } \
            } while (0) \

        _CLONE_(vImageStreamsSrc,       vImageStreamsDst);
        _CLONE_(vMetaStreamsSrc,        vMetaStreamsDst);
        _CLONE_(vMinFrameDurationSrc,   vMinFrameDurationDst);
        _CLONE_(vStallFrameDurationSrc, vStallFrameDurationDst);
#undef  _CLONE_

    //
    mConfigParams.vImageStreams = vImageStreamsDst;
    mConfigParams.vMetaStreams  = vMetaStreamsDst;
    mConfigParams.vMinFrameDuration = vMinFrameDurationDst;
    mConfigParams.vStallFrameDuration = vStallFrameDurationDst;
    mConfigParams.pPipelineModel = pPipeline;

    // determine pipelinemodel
    //Use FeatureMgr to determine pipelinemodel
    MINT32 DetermineModel = ePIPELINEMODEL_DEFAULT;
    NSCam::v3::Utils::FeatureMgr* pFeatureMgr = NSCam::v3::Utils::FeatureMgr::getFeatureMgrSingleInst();
    pFeatureMgr->decidePipelineModel(DetermineModel);
    mConfigParams.pipelineScene  = (PipelineScene)DetermineModel;
    //mConfigParams.pipelineScene  = ePIPELINEMODEL_DEFAULT;

    //
    {   // check current inflight pipeline w/ pass1-infight-request
        Mutex::Autolock _l(mPipelineLock);
        if ( vpPipelineModel.size()>1 ) {
            sp<IPipelineModel> pPipeline = NULL;
            sp<InFlightPipeline> pInFlightPipeline = vpPipelineModel.getNewestPipeline();
            MY_LOGD("Inflight pipeline + %d", vpPipelineModel.size());
            if ( pInFlightPipeline.get() ) {
                pPipeline = pInFlightPipeline->getPipelineModel();
                if( pPipeline.get() )
                    pPipeline->waitDrained();
                vpPipelineModel.removeAt(0);
                CAM_TRACE_INT("In-flight Pipeline", vpPipelineModel.size());
            }
            MY_LOGD("Inflight pipeline - %d", vpPipelineModel.size());
        }
    }
    //
    err = ConfigurePipeline()(mConfigParams);
    if ( OK != err ) {
        dump();
        dumpConfigure();
        MY_LOGE("configure pipeline %d", err);
        return err;
    }
    pPipeline = mConfigParams.pPipelineModel;
    if ( mpPipelineResourceMgr.get() ) {
        Vector<Pipeline_NodeId_T> vNodeList;
        mpPipelineResourceMgr->getReusableNodes(vNodeList);
        for ( size_t i=0; i<vNodeList.size(); i++ ) {
            mpPipelineResourceMgr->registerOwnerOfNode(vNodeList[i], pPipeline.get());
        }
    }
    pPipeline->setPipelineFrameNumberGenerator(getPipelineFrameNumberGenerator());
    //
    {
        Mutex::Autolock _l(mPipelineLock);
        MY_LOGD("Inflight pipeline + %d", vpPipelineModel.size());
        sp<InFlightPipeline> pInFlightPipeline = new InFlightPipeline(pPipeline, mConfigParams.pipelineScene, 0);
        vpPipelineModel.addInFlightPipeline(pInFlightPipeline);
        MY_LOGD("Inflight pipeline - %d", vpPipelineModel.size());
        CAM_TRACE_INT("In-flight Pipeline", vpPipelineModel.size());
    }
    MY_LOGD("configure pipeline %d", err);
    //
    FUNC_END;
    return err;
}

/******************************************************************************
 *
 ******************************************************************************/
MERROR
PipelineModelMgr::
submitRequest(IPipelineModelMgr::AppRequest& request)
{
    FUNC_START;

    CAM_TRACE_CALL();
    //
    MBOOL  bReconfigure = MFALSE;
    MERROR err = MTRUE;
    PipelineScene eScene;
    // determine whether the lastest pipeline meets this request or not
    //      if Y -> submit request to the lastest pipeline (but hal might processes oldest existing-pipeline)
    //      if N -> create new pipeline and submit request to the this pipeline
    if ( request.requestNo>0 )
        bReconfigure = determinePipelineModel(request, eScene);
    //
    if ( bReconfigure ) { //        create new InFlightPipeline and insert into vector
        MY_LOGD("Create new InFlightPipeline for scene %d", eScene);
        {
            Mutex::Autolock _l(mPipelineLock);
            //
            sp<IPipelineModel> pPipeline = NULL;
            sp<InFlightPipeline> pInFlightPipeline = vpPipelineModel.getNewestPipeline();
            pPipeline = pInFlightPipeline->getPipelineModel();
            pPipeline->waitDrained();
            //
            mConfigParams.pipelineScene = eScene;
            mConfigParams.pPipelineModel = pPipeline;
            // determine pipelinemodel
            err = ConfigurePipeline()(mConfigParams);
            if ( OK != err ) {
                MY_LOGE("configure pipeline %d", err);
                return err;
            }
            pPipeline = mConfigParams.pPipelineModel;
            if ( mpPipelineResourceMgr.get() ) {
                Vector<Pipeline_NodeId_T> vNodeList;
                mpPipelineResourceMgr->getReusableNodes(vNodeList);
                for ( size_t i=0; i<vNodeList.size(); i++ ) {
                    mpPipelineResourceMgr->registerOwnerOfNode(vNodeList[i], pPipeline.get());
                }
            }
            pPipeline->setPipelineFrameNumberGenerator(getPipelineFrameNumberGenerator());
            //
            {
                MY_LOGD("Inflight pipeline + %d", vpPipelineModel.size());
                sp<InFlightPipeline> pInFlightPipeline = new InFlightPipeline(pPipeline, eScene, request.requestNo);
                vpPipelineModel.addInFlightPipeline(pInFlightPipeline);
                MY_LOGD("Inflight pipeline - %d", vpPipelineModel.size());
                CAM_TRACE_INT("In-flight Pipeline", vpPipelineModel.size());
            }
        }
        dump();
    }
    //MY_LOGD("we add request to latest InFlightPipeline");
    CAM_TRACE_BEGIN("submitRequest to pipeline");
    sp<IPipelineModel> pPipeline = NULL;
    {
        Mutex::Autolock _l(mPipelineLock);
        sp<InFlightPipeline> pInFlightPipeline = vpPipelineModel.getNewestPipeline();
        pPipeline = pInFlightPipeline->getPipelineModel();
        if ( ! pPipeline.get() ) {
            MY_LOGE("Can not get PipelineModel (%d/%d)", vpPipelineModel.size(), vpPipelineModel.size() );
            FUNC_END;
            return err;
        }
        pInFlightPipeline->updateEndReqNo(request.requestNo);
    }
    //
    IPipelineModel::AppRequest appRequestParams;
#define _CLONE_(src, dst) \
            do  { \
                dst.setCapacity(src.size()); \
                dst.clear(); \
                for (size_t i = 0; i < src.size(); i++) { \
                    dst.add(src.keyAt(i), src.valueAt(i)); \
                } \
            } while (0) \

    appRequestParams.requestNo = request.requestNo;
    appRequestParams.pAppCallback = this;
    _CLONE_(request.vIImageBuffers,  appRequestParams.vIImageBuffers);
    _CLONE_(request.vOImageBuffers, appRequestParams.vOImageBuffers);
    _CLONE_(request.vIMetaBuffers,   appRequestParams.vIMetaBuffers);

#undef  _CLONE_
    //
    err = pPipeline->submitRequest(appRequestParams);
    if  ( OK != err ) {
        dump();
        dumpConfigure();
        return err;
    }
    CAM_TRACE_END();

    // check oldest inflight pipeline should be removed or not
    {
        Mutex::Autolock _l(mPipelineLock);
        if ( vpPipelineModel.size() > 1 ) {
            if ( request.oldestRequestNo > vpPipelineModel.getCurrentPipeline()->getEndReqNo() ) {
                MY_LOGD("Remove oldest InFlight Pipeline size(%d) : Oldest request of AppStreamMgr(%d) vs End request of oldest inflightPipeline(%d)",
                        vpPipelineModel.size(), request.oldestRequestNo, vpPipelineModel.getCurrentPipeline()->getEndReqNo());
                //
                MY_LOGD("Inflight pipeline + %d", vpPipelineModel.size());
                vpPipelineModel.removeAt(0);
                MY_LOGD("Inflight pipeline - %d", vpPipelineModel.size());
                CAM_TRACE_INT("In-flight Pipeline", vpPipelineModel.size());
            }
        }
   }
    //
    FUNC_END;
    return err;
}

/******************************************************************************
*
******************************************************************************/
MBOOL
PipelineModelMgr::
determinePipelineModel(IPipelineModelMgr::AppRequest& request, PipelineScene &rScene)
{
    CAM_TRACE_CALL();

    MERROR err = MFALSE;
    //
    sp<IMetaStreamBuffer> pStreamBuffer = request.vIMetaBuffers[0];
    IMetadata* pMetadata = pStreamBuffer->tryReadLock(LOG_TAG);
    IMetadata::IEntry const eSceneMode = pMetadata->entryFor(MTK_CONTROL_SCENE_MODE);
    IMetadata::IEntry const eCaptureIntent = pMetadata->entryFor(MTK_CONTROL_CAPTURE_INTENT);
    pStreamBuffer->unlock(LOG_TAG, pMetadata);

    /*android::sp<IMetadataConverter> metadataConverter = IMetadataConverter::createInstance(IDefaultMetadataTagSet::singleton()->getTagSet());
    metadataConverter->dumpAll(*pMetadata, request.requestNo);*/
    IMetadata::IEntry const e3dnrMode = pMetadata->entryFor(MTK_NR_FEATURE_3DNR_MODE);


    //
    Mutex::Autolock _l(mPipelineLock);
    sp<InFlightPipeline> pInFlightPipeline = vpPipelineModel.getNewestPipeline();
    PipelineScene eNewestScene = pInFlightPipeline->getPipelineScene();
    // FIX ME: modify here
    if ( ! eSceneMode.isEmpty() &&
           eSceneMode.itemAt(0, Type2Type<MUINT8>()) == MTK_CONTROL_SCENE_MODE_HDR &&
         ! eCaptureIntent.isEmpty() &&
           eCaptureIntent.itemAt(0, Type2Type<MUINT8>()) == MTK_CONTROL_CAPTURE_INTENT_STILL_CAPTURE )
    {
        // hdr pipeline
        if ( ePIPELINEMODEL_HDR == eNewestScene ) {
            rScene = ePIPELINEMODEL_HDR;
            return MFALSE;
        } else {
            rScene = ePIPELINEMODEL_HDR;
            return MTRUE;
        }
    } else if( !e3dnrMode.isEmpty()  &&
           e3dnrMode.itemAt(0, Type2Type<MINT32>()) == MTK_NR_FEATURE_3DNR_MODE_ON) {
        MY_LOGD("Feature pipelineModel selected, request.requestNo=%d, eNewestScene=%d, value=%d",
            request.requestNo, eNewestScene, e3dnrMode.itemAt(0, Type2Type<MINT32>()));
        // feature pipeline
        if ( ePIPELINEMODEL_FEATURE == eNewestScene ) {
            rScene = ePIPELINEMODEL_FEATURE;
            return MFALSE;
        } else {
            rScene = ePIPELINEMODEL_FEATURE;
            return MTRUE;
        }
    } else {
        //MY_LOGD("Default pipelineModel selected, request.requestNo=%d", request.requestNo);
        // default pipeline
        if ( ePIPELINEMODEL_DEFAULT == eNewestScene ) {
            rScene = ePIPELINEMODEL_DEFAULT;
            return MFALSE;
        } else {
            rScene = ePIPELINEMODEL_DEFAULT;
            return MTRUE;
        }
    }
    //
    return err;
}

/******************************************************************************
*
******************************************************************************/
MVOID
PipelineModelMgr::
dump()
{
    // for debug
    Mutex::Autolock _l(mPipelineLock);
    for ( size_t i=0; i<vpPipelineModel.size(); i++ ) {
        sp<InFlightPipeline> pInFlightPipeline = vpPipelineModel.getInFlightPipeline(i);
        //sp<IPipelineModel>   pPipeline = pInFlightPipeline->getPipelineModel(0);
        MY_LOGD("dump InFlightPipeline %d of %d: pipeline(%p) Scene(%d) Request(%d-%d)",
                i, vpPipelineModel.size(),
                pInFlightPipeline->getPipelineModel().get(), pInFlightPipeline->getPipelineScene(),
                pInFlightPipeline->getStartReqNo(), pInFlightPipeline->getEndReqNo());
    }
}

/******************************************************************************
*
******************************************************************************/
MVOID
PipelineModelMgr::
dumpConfigure()
{
    Mutex::Autolock _l(mPipelineLock);
    MY_LOGD("dump configure openId(%d)", mConfigParams.openId);
    MY_LOGD("dump configure PipelineModel(%p)", mConfigParams.pPipelineModel.get());
    MY_LOGD("dump configure Scene(%d)", mConfigParams.pipelineScene);
    //
    ConfigurePipeline::MetaStreamsT  const& vMetaStreams  = mConfigParams.vMetaStreams;
    ConfigurePipeline::ImageStreamsT const& vImageStreams = mConfigParams.vImageStreams;
    for ( size_t i=0; i<vImageStreams.size(); i++) {
        MY_LOGD("ImageStream(%d/%d) StreamInfo id(%d) p(%p)",
            i, vImageStreams.size(), vImageStreams.keyAt(i), vImageStreams.valueAt(i).get());
    }
    for ( size_t i=0; i<vMetaStreams.size(); i++) {
        MY_LOGD("MetaStream(%d/%d) StreamInfo id(%d) p(%p)",
            i, vMetaStreams.size(), vMetaStreams.keyAt(i), vMetaStreams.valueAt(i).get());
    }
}

/******************************************************************************
 *  IPipelineBufferSetFrameControl::IAppCallback Interfaces.
 ******************************************************************************/
MVOID
PipelineModelMgr::
updateFrame(
    MUINT32 const frameNo,
    MINTPTR const userId,
    ssize_t const nOutMetaLeft,
    android::Vector<android::sp<IMetaStreamBuffer> > vOutMeta
)
{
    FUNC_START;
    //
    sp<IPipelineModelMgr::IAppCallback> pAppCallback;
    pAppCallback = mpAppCallback.promote();
    if ( ! pAppCallback.get() ) {
        MY_LOGE("Have not set callback to device");
        FUNC_END;
        return;
    }
    pAppCallback->updateFrame(frameNo, userId, nOutMetaLeft, vOutMeta);
    //
    FUNC_END;
}


