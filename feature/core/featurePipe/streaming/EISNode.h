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

#ifndef _MTK_CAMERA_STREAMING_FEATURE_PIPE_EIS_NODE_H_
#define _MTK_CAMERA_STREAMING_FEATURE_PIPE_EIS_NODE_H_

#include "StreamingFeatureNode.h"

class EisHal;

namespace NSCam {
namespace NSCamFeature {
namespace NSFeaturePipe {

class EISStore
{
public:

    EISStore()
        : mRequest(NULL)
    {
        memset(&mConfig, 0, sizeof(mConfig));
    }

    EISStore(const RequestPtr &request, const EIS_HAL_CONFIG_DATA &config)
        : mRequest(request)
        , mConfig(config)
    {
    }

    RequestPtr mRequest;
    EIS_HAL_CONFIG_DATA mConfig;
};

class EISNode : public StreamingFeatureNode
{
public:
    EISNode(const char *name);
    virtual ~EISNode();

public:
    virtual MBOOL onData(DataID id, const RequestPtr &data);
    virtual MBOOL onData(DataID id, const EisConfigData &data);
    virtual MBOOL onData(DataID id, const FMData &data);

protected:
    virtual MBOOL onInit();
    virtual MBOOL onUninit();
    virtual MBOOL onThreadStart();
    virtual MBOOL onThreadStop();
    virtual MBOOL onThreadLoop();

private:
    MBOOL processEIS22(const RequestPtr &request, const EIS_HAL_CONFIG_DATA &config);
    MBOOL processEIS25(const RequestPtr &request, const EIS_HAL_CONFIG_DATA &config, const FMResult &fm);
    MVOID applyDZ(const RequestPtr &request, const MSize &srcSize);
    MBOOL handleWarpData(const ImgBuffer &warp, const RequestPtr &request);
    MVOID flushQueue();

private:
    WaitQueue<FMData> mRequests;
    WaitQueue<EisConfigData> mConfigDatas;
    android::sp<ImageBufferPool> mWarpMapBufferPool;

    EisHal*   mpEisHal;
    EIS_PLUS_GPU_INFO mEisPlusGpuInfo;

    std::queue<EISStore> mQueue;
    MUINT32 mAlgCounter;
};

} // namespace NSFeaturePipe
} // namespace NSCamFeature
} // namespace NSCam

#endif // _MTK_CAMERA_STREAMING_FEATURE_PIPE_EIS_NODE_H_
