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

#ifndef _MTK_CAMERA_FEATURE_PIPE_CORE_GRAPHIC_BUFFER_POOL_H_
#define _MTK_CAMERA_FEATURE_PIPE_CORE_GRAPHIC_BUFFER_POOL_H_

#include "MtkHeader.h"
//#include <mtkcam/common.h>
//#include <mtkcam/utils/imagebuf/IGrallocImageBufferHeap.h>
//#include <mtkcam/utils/common.h>

#include <ui/GraphicBuffer.h>
#include <utils/Mutex.h>

#include "BufferPool.h"
#include "IIBuffer.h"

namespace NSCam {
namespace NSCamFeature {
namespace NSFeaturePipe {

class GraphicBufferPool;

class GraphicBufferHandle : public BufferHandle<GraphicBufferHandle>
{
public:
  GraphicBufferHandle(const android::sp<BufferPool<GraphicBufferHandle> > &pool);
  virtual ~GraphicBufferHandle();
  sp<GraphicBuffer>* getGraphicBufferAddr();
  #if 0
  MBOOL resize(MUINT32 width, MUINT32 height);
  MBOOL lock();
  MBOOL unlock();
  #endif

public:
  android::sp<IImageBuffer> mImageBuffer;
  android::sp<android::GraphicBuffer> mGraphicBuffer;

private:
  enum Type { ALLOCATE, REGISTER };
  friend class GraphicBufferPool;
  Type mType;
};
typedef sb<GraphicBufferHandle> SmartGraphicBuffer;

class GraphicBufferPool : public BufferPool<GraphicBufferHandle>
{
public:
  static const MUINT32 USAGE_HW_TEXTURE;
  static const MUINT32 USAGE_HW_RENDER;

public:
  class BufferInfo
  {
  public:
    android::sp<android::GraphicBuffer> mGraphic;
    android::sp<IImageBuffer> mImage;
    MUINT32 mSize;

    BufferInfo()
    {}

    BufferInfo(android::sp<android::GraphicBuffer> graphic, android::sp<IImageBuffer> image, MUINT32 size) : mGraphic(graphic), mImage(image), mSize(size)
    {}
  };

public:
  static android::sp<GraphicBufferPool> create(const char *name, MUINT32 width, MUINT32 height, android::PixelFormat format, MUINT32 usage);
  static MVOID destroy(android::sp<GraphicBufferPool> &pool);
  virtual ~GraphicBufferPool();

  MSize getImageSize() const;
  android::sp<IIBuffer> requestIIBuffer();
  #if 0
    MBOOL add(const BufferInfo &info);
    MBOOL add(const android::sp<android::GraphicBuffer> &graphic, const android::sp<IImageBuffer> &image, MUINT32 size);
  #endif

protected:
  GraphicBufferPool(const char *name);
  MBOOL init(MUINT32 width, MUINT32 height, android::PixelFormat format, MUINT32 usage);
  MVOID uninit();
  virtual android::sp<GraphicBufferHandle> doAllocate();
  virtual MBOOL doRelease(GraphicBufferHandle *handle);

private:
  MBOOL initConfig(MUINT32 width, MUINT32 height, android::PixelFormat format, MUINT32 usage);
  EImageFormat toImageFormat(android::PixelFormat graphFormat);

private:
  android::Mutex mMutex;
  MBOOL mReady;

private:
  MUINT32 mWidth;
  MUINT32 mHeight;
  EImageFormat mImageFormat;
  android::PixelFormat mGraphicFormat;
  MUINT32 mImageUsage;
  MUINT32 mGraphicUsage;
  MUINT32 mPlane;
  size_t mStride[3];
  size_t mBoundary[3];
  IImageBufferAllocator::ImgParam mAllocatorParam;
  IGrallocImageBufferHeap::AllocExtraParam mAllocatorExtraParam;
};

}; // namespace NSFeaturePipe
}; // namespace NSCamFeature
}; // namespace NSCam
#endif  // _MTK_CAMERA_FEATURE_PIPE_CORE_GRAPHIC_BUFFER_POOL_H_
