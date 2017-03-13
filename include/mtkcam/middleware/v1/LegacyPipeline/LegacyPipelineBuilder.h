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

#ifndef _MTK_HARDWARE_INCLUDE_MTKCAM_LEGACYPIPELINE_LEGACYPIPELINEBUILDER_H_
#define _MTK_HARDWARE_INCLUDE_MTKCAM_LEGACYPIPELINE_LEGACYPIPELINEBUILDER_H_

#include <mtkcam/pipeline/utils/streaminfo/MetaStreamInfo.h>
#include <mtkcam/pipeline/utils/streaminfo/ImageStreamInfo.h>
#include <mtkcam/pipeline/utils/streambuf/StreamBuffers.h>
#include <mtkcam/pipeline/utils/streambuf/StreamBufferProvider.h>
#include <mtkcam/utils/hw/IScenarioControl.h>
#include "ILegacyPipeline.h"

using namespace NSCam;
using namespace NSCam::v3;
using namespace NSCam::v1;

namespace NSCam {
namespace v1 {
namespace NSLegacyPipeline {
/******************************************************************************
 *
 ******************************************************************************/

struct PipelineSensorParam
{
    PipelineSensorParam(MUINT _mode,
                        MINT32 _rawType,
                        MSize _size,
                        MUINT _fps,
                        MUINT32 _pixelMode,
                        MUINT32 _vhdrMode)
        : mode(_mode)
        , rawType(_rawType)
        , size(_size)
        , fps(_fps)
        , pixelMode(_pixelMode)
        , vhdrMode(_vhdrMode)
        {
        };
    //
    PipelineSensorParam()
        : mode(0)
        , rawType(0)
        , size(MSize(0,0))
        , fps(0)
        , pixelMode(0)
        , vhdrMode(0)
        {
        };
    //
    /* Refer to SENSOR_SCENARIO_ID_XXX in IHalSensor.h */
    MUINT       mode;
    MINT32      rawType;   // 0: pure-raw, 1: preprocessed-raw
    MSize       size;
    MUINT       fps;
    MUINT32     pixelMode;
    /* Refer to SENSOR_VHDR_MODE_XXX in IHalSensor.h.
    * It is independant with SENSOR_SCENARIO_ID_XXX currently.
    * It depends on intersection of :
    * (1) app open hdr or not
    * (2) which vhdr mode sensor supported.
    */
    MUINT32     vhdrMode;

};

struct PipelineImageParam
{
    sp<IImageStreamInfo>        pInfo;
    sp<StreamBufferProvider>    pProvider;
    //
    //extended flag to specified some special use case
    MUINT32                     usage;
};

struct LegacyPipelineMode_T
{
    enum {
        PipelineMode_Preview,
        PipelineMode_ZsdPreview,
        PipelineMode_NormalMShotPreview,
        PipelineMode_VideoRecord,
        PipelineMode_HighSpeedVideo,
        PipelineMode_Capture,
        PipelineMode_Capture_VSS,
        PipelineMode_MShotCapture,
        PipelineMode_Hdr,
        PipelineMode_ZsdHdr,
        PipelineMode_Mfll,
        PipelineMode_ZsdMfll,
        PipelineMode_Feature_Preview,
        PipelineMode_Feature_ZsdPreview,
        PipelineMode_Feature_NormalMShotPreview,
        PipelineMode_Feature_VideoRecord,
        PipelineMode_Feature_HighSpeedVideo,
        PipelineMode_Feature_Capture,
        PipelineMode_Feature_Capture_VSS,
        PipelineMode_Feature_MShotCapture,
        PipelineMode_Feature_StereoZSD,
    };
};

class LegacyPipelineBuilder
        : public virtual RefBase
{
public:
    struct  ConfigParams
    {
        ConfigParams()
            : mode(0)
            , enableEIS(MFALSE)
            , enableLCS(MFALSE)
            , enableDualPD(MFALSE)
            , eisMode(0)
            , enableBurstNum(MTRUE)
            , optimizedCaptureFlow(MFALSE)
            , disableFrontalBinning(MFALSE)
            {
            };
        //
        MINT32  mode;
        MBOOL   enableEIS;
        MBOOL   enableLCS;
        MBOOL   enableDualPD;
        MINT32  eisMode;
        MBOOL   enableBurstNum;
        MBOOL   optimizedCaptureFlow;
        MBOOL   disableFrontalBinning;
    };

    static sp<LegacyPipelineBuilder> createInstance(
                                            MINT32 const openId,
                                            char const* pipeName,
                                            ConfigParams const & rParam
                                            );

public:     ////                    Interface for setup PipelineContext.
    /**
     * set pipeline's source: sensor or memory
     *
     * @param[in] rParam: sensor parameters if use sensor as source.
     *                    or the the stream info of the source memory
     *
     */
     virtual MERROR                 setSrc(PipelineSensorParam const & rParam) = 0;
     virtual MERROR                 setSrc(Vector<PipelineImageParam> const& pParams) = 0;

    /**
     * set pipeline's destination buffers
     *
     * @param[in] vParams: the stream infos of the requested output streams
     *
     */
     virtual MERROR                 setDst(Vector<PipelineImageParam> const& vParams) = 0;

    /**
     * set dispatcher
     *
     * @param[in] spDispatcher: the dispatcher for pipeline
     *
     */
     virtual MERROR                 setDispatcher(sp<IDispatcher> const& spDispatcher) = 0;

    /**
     * set scenario control
     *
     * @param[in] pControl: the scenario control
     *
     */
     virtual MERROR                 setScenarioControl(sp<IScenarioControl> pControl) = 0;

    /**
     * create the ILegacyPipeline
     *
     * PipelineManager will hold the sp of this ILegacyPipeline.
     *
     */
     virtual sp<ILegacyPipeline>    create() = 0;

};
}; //namespace NSLegacyPipeline
}; //namespace v1
}; //namespace NSCam
#endif
