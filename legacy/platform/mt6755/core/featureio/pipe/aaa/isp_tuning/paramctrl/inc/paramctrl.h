/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#ifndef _PARAMCTRL_H_
#define _PARAMCTRL_H_

#include <aaa_hal.h>
#include <utils/threads.h>
#include <mtkcam/featureio/tuning_mgr.h>
#include "paramctrl_if.h"

namespace NSIspTuning
{

class PcaMgr;
class CcmMgr;

class LscMgr2;
class GmaMgr;

class Paramctrl : public IParamctrl
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Ctor/Dtor.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    //  Copy constructor is disallowed.
    Paramctrl(Paramctrl const&);
    //  Copy-assignment operator is disallowed.
    Paramctrl& operator=(Paramctrl const&);

protected:
    Paramctrl(ESensorDev_T const eSensorDev,
              MINT32 const i4SensorIdx, 
              MUINT32 const u4SensorID,
              ESensorTG_T const eSensorTG,
              NVRAM_CAMERA_ISP_PARAM_STRUCT*const pNvram_Isp);
    ~Paramctrl();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Instance
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    static Paramctrl* getInstance(ESensorDev_T const eSensorDev, MINT32 const i4SensorIdx);
    virtual MVOID destroyInstance();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Parameters Change.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////

    template <class T>
    MBOOL checkParamChange(T const old_value, T const new_value)
    {
        if  ( old_value != new_value )
        {
            m_u4ParamChangeCount++;
            return  MTRUE;
        }
        return  MFALSE;
    }

protected:  ////
    inline MUINT32  getParamChangeCount() const { return m_u4ParamChangeCount; }
    inline MVOID    resetParamChangeCount() { m_u4ParamChangeCount = 0; }
    

// Data Members
private:
    //  It means that any params have changed if > 0.
    MUINT32         m_u4ParamChangeCount;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public: // Dynamic Tuning
    virtual MVOID   enableDynamicTuning(MBOOL const fgEnable);
    virtual MBOOL   isDynamicTuning() const { return m_fgDynamicTuning; }
    virtual MVOID   enableDynamicCCM(MBOOL const fgEnable);
    virtual MBOOL   isDynamicCCM() const { return m_fgDynamicCCM; }
	virtual MVOID   enableDynamicBypass(MBOOL const fgEnable) {m_fgDynamicBypass = fgEnable; }
	virtual MBOOL   isDynamicBypass() const {return m_fgDynamicBypass; }
    virtual MVOID   enableDynamicShading(MBOOL fgEnable);
    virtual MBOOL   isDynamicShading() const { return m_fgDynamicShading; }
    virtual MVOID   notifyRPGEnable(MBOOL const fgEnable) {m_fgEnableRPG = fgEnable; }
    virtual MBOOL   isRPGEnable() const {return m_fgEnableRPG; }
    virtual MVOID   forceValidate() { m_u4ParamChangeCount++; }

private:
    MBOOL m_fgDynamicTuning; //  Enable dynamic tuning if true; otherwise false. It's true by default (normal mode).
    MBOOL m_fgDynamicCCM; //  Enable dynamic CCM if true; otherwise false. It's true by default (normal mode).
    MBOOL m_fgDynamicBypass;
    MBOOL m_fgDynamicShading;   // Enable dynamic shading if true; otherwise false. It's true by default.
    MBOOL m_fgEnableRPG;
	ISP_NVRAM_OBC_T m_backup_OBCInfo;

protected:  ////
    //  Enable Shading if true; otherwise false.
    //  It's true by default (Enable).
    //MBOOL           m_fgShadingNVRAMdataChange;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public: // Attributes
    virtual MERROR_ENUM  setIspProfile(EIspProfile_T const eIspProfile);
    virtual MERROR_ENUM  setSceneMode(EIndex_Scene_T const eScene);
    virtual MERROR_ENUM  setEffect(EIndex_Effect_T const eEffect);
    virtual ESensorDev_T getSensorDev() const { return m_eSensorDev; }
    virtual EOperMode_T getOperMode()   const { return m_eOperMode; }
    virtual MERROR_ENUM setOperMode(EOperMode_T const eOperMode);
    virtual MERROR_ENUM  setCamMode(MINT32 const i4CamMode); 
    virtual MERROR_ENUM  setSensorMode(ESensorMode_T const eSensorMode);
    virtual ESensorMode_T  getSensorMode()   const { return m_rIspCamInfo.eSensorMode; }
    virtual MERROR_ENUM setZoomRatio(MINT32 const i4ZoomRatio_x100);
    virtual MERROR_ENUM setAWBInfo(AWB_INFO_T const &rAWBInfo);
    virtual MERROR_ENUM setAWBGain(AWB_GAIN_T& rNewIspAWBGain);
    virtual MERROR_ENUM setIspAEPreGain2(MINT32 i4SensorIndex, AWB_GAIN_T& rNewIspAEPreGain2);
    virtual MERROR_ENUM setAEInfo(AE_INFO_T const &rAEInfo);
    virtual MERROR_ENUM setISPAEGain(MBOOL bEnableWorkAround, MUINT32 u4NewISPAEGain);
    virtual MERROR_ENUM setIspFlareGainOffset(MINT16 i2FlareGain, MINT16 i2FlareOffset);
    virtual MERROR_ENUM setAFInfo(AF_INFO_T const &rAFInfo);
    virtual MERROR_ENUM setFlashInfo(FLASH_INFO_T const &rFlashInfo);
    virtual MERROR_ENUM setFlashDynamicInfo(MINT32 const i4FlashOnOff);
    virtual MERROR_ENUM setIndex_Shading(MINT32 const i4IDX);
    virtual MERROR_ENUM getIndex_Shading(MVOID*const pCmdArg);
	virtual MERROR_ENUM setPureOBCInfo(const ISP_NVRAM_OBC_T *pOBCInfo);
	virtual MERROR_ENUM getPureOBCInfo(ISP_NVRAM_OBC_T *pOBCInfo);
    virtual MERROR_ENUM setTGInfo(MINT32 const i4TGInfo);
    //        MERROR_ENUM setIso(MUINT32 const u4ISOValue);
    virtual MVOID* getDefaultISPIndex(MUINT32 eIspProfile, MUINT32 eSensorMode, MUINT32 eIdx_ISO);
    virtual MVOID  dumpAllNvramIndexMapping(MUINT32 bReset);
    virtual MVOID sendIspTuningIOCtrl(MUINT32 ctrl, MINTPTR arg1, MINTPTR arg2);
    virtual MBOOL  getIspGamma(MUINT32* pIspGamma, MBOOL* pEnable) const;
    virtual MERROR_ENUM getDNGColorMatrix1(double cm[3][3]) const;
    virtual MERROR_ENUM getDNGForwardMatrix1(double fm[3][3]) const;
    virtual MBOOL  setISO(MINT32 const i4ISOValue);


private:
    EIndex_Effect_T m_eIdx_Effect;
    ESensorDev_T    m_eSensorDev;
    EOperMode_T     m_eOperMode;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
    virtual MERROR_ENUM validate(MINT32 const i4FrameID, MBOOL const fgForce);
    virtual MERROR_ENUM validateFrameless();
    virtual MERROR_ENUM validatePerFrame(MINT32 const i4FrameID, MBOOL const fgForce);
    virtual MERROR_ENUM init();
    virtual MERROR_ENUM uninit();

protected:
    inline EIndex_Effect_T getEffect() const { return m_eIdx_Effect; }
    //virtual MERROR_ENUM do_init();
    MERROR_ENUM convertColorTransform();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:     ////    Exif.
    virtual MERROR_ENUM getDebugInfo(
        NSIspExifDebug::IspExifDebugInfo_T& rIspExifDebugInfo,
        MBOOL const fgReadFromHW
    ) const;

protected:  ////

    //MERROR_ENUM         saveDebugInfo();

    mutable NSIspExifDebug::IspExifDebugInfo_T    m_rIspExifDebugInfo;
    mutable NSIspExifDebug::IspExifDebugInfo_T::IspGmaInfo m_GmaExifInfo;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementation: Frameless
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:
    MBOOL   prepareHw_Frameless_All();
    MBOOL   applyToHw_Frameless_All();

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////    Module.
    MBOOL   prepare_Frameless_Shading();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementation: Per-Frame
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  // Invoked only by validatePerFrame()
    MERROR_ENUM do_validatePerFrame(MINT32 const i4FrameID);

protected:  ////    All
    MBOOL   prepareHw_PerFrame_All();
	MBOOL   prepareHw_PerFrame_Partial();
	MBOOL   prepareHw_DynamicBypass_OBC();
    MBOOL   applyToHw_PerFrame_All(MINT32 const i4FrameID);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////    Module.
    INDEX_T getISPIndex_byISO(MUINT32 u4Idx_ISO);
    MBOOL   prepareHw_PerFrame_DBS();
    MBOOL   prepareHw_PerFrame_OBC();
    MBOOL   prepareHw_PerFrame_BPC();
    MBOOL   prepareHw_PerFrame_NR1();
    MBOOL   prepareHw_PerFrame_LSC();
    MBOOL   prepareHw_PerFrame_RPG();
    MBOOL   prepareHw_PerFrame_NSL2();
    MBOOL   prepareHw_PerFrame_PGN();
    MBOOL   prepareHw_PerFrame_UDM();
    MBOOL   prepareHw_PerFrame_CCM();
    MBOOL   prepareHw_PerFrame_GGM();
    MBOOL   prepareHw_PerFrame_ANR();
    MBOOL   prepareHw_PerFrame_ANR2();
    MBOOL   prepareHw_PerFrame_CCR();
    MBOOL   prepareHw_PerFrame_BOK();
    MBOOL   prepareHw_PerFrame_PCA();
    MBOOL   prepareHw_PerFrame_EE();
    MBOOL   prepareHw_PerFrame_EFFECT();
    MBOOL   prepareHw_PerFrame_NR3D();
    MBOOL   prepareHw_PerFrame_MFB();
    MBOOL   prepareHw_PerFrame_MIXER3();
    MBOOL   prepareHw_PerFrame_LCE();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  ISP End-User-Define Tuning Index.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    virtual MERROR_ENUM setIspUserIdx_Edge(EIndex_Isp_Edge_T const eIndex);
    virtual MERROR_ENUM setIspUserIdx_Hue(EIndex_Isp_Hue_T const eIndex);
    virtual MERROR_ENUM setIspUserIdx_Sat(EIndex_Isp_Saturation_T const eIndex);
    virtual MERROR_ENUM setIspUserIdx_Bright(EIndex_Isp_Brightness_T const eIndex);
    virtual MERROR_ENUM setIspUserIdx_Contrast(EIndex_Isp_Contrast_T const eIndex);

protected:

    inline
    IspUsrSelectLevel_T const&
    getIspUsrSelectLevel() const
    {
        return m_IspUsrSelectLevel;
    }

// Data Members
private:
    IspUsrSelectLevel_T     m_IspUsrSelectLevel;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Data Members.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////
    RAWIspCamInfo       m_rIspCamInfo;
    IspTuningCustom*    m_pIspTuningCustom;

//..............................................................................
//  ISP Tuning Parameters.
//..............................................................................
protected:  ////    ISP Tuning Parameters.

    //  Reference to a given buffer.
    NVRAM_CAMERA_ISP_PARAM_STRUCT&  m_rIspParam;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////    ISP Common Parameters.

    //  Reference to m_rIspParam.ISPComm
    ISP_NVRAM_COMMON_STRUCT&        m_rIspComm;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:  ////    ISP Register Parameters.
    //  Reference to m_rIspParam.ISPRegs & m_rIspParam.ISPRegs.Idx
    class IspNvramMgr : public IspNvramRegMgr
    {
    public:
        IspNvramMgr(ISP_NVRAM_REGISTER_STRUCT*const pIspNvramRegs)
            : IspNvramRegMgr(pIspNvramRegs)
        {}

        IspNvramMgr& operator=(IndexMgr const& rIdxmgr)
        {
            setIdx_DBS (rIdxmgr.getIdx_DBS());
            setIdx_OBC (rIdxmgr.getIdx_OBC());
            setIdx_BPC (rIdxmgr.getIdx_BPC());
            setIdx_NR1 (rIdxmgr.getIdx_NR1());
            setIdx_UDM (rIdxmgr.getIdx_UDM());
            setIdx_GGM (rIdxmgr.getIdx_GGM());
            setIdx_ANR (rIdxmgr.getIdx_ANR());
            setIdx_ANR2 (rIdxmgr.getIdx_ANR2());
            setIdx_CCR (rIdxmgr.getIdx_CCR());
            setIdx_BOK (rIdxmgr.getIdx_BOK());
            setIdx_EE  (rIdxmgr.getIdx_EE());
            setIdx_NR3D (rIdxmgr.getIdx_NR3D());
            setIdx_MFB (rIdxmgr.getIdx_MFB());
            setIdx_LCE (rIdxmgr.getIdx_LCE());
            return  (*this);
        }
    };
    IspNvramMgr m_IspNvramMgr;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected:

    //  PCA Manager.
    PcaMgr*  m_pPcaMgr;

    // CCM Manager
    CcmMgr* m_pCcmMgr;

    // GMA Manager
    GmaMgr* m_pGmaMgr;

#if 1

//..............................................................................
//  ISP Shading Parameters.
//..............................................................................
protected:  ////    ISP Shading Manager.

    //  LSC Manager.
    //NVRAM_CAMERA_SHADING_STRUCT    *m_pNvram_Shading;
    LscMgr2                          *m_pLscMgr;
#endif
    mutable android::Mutex  m_Lock;
    MBOOL m_bDebugEnable;
    MBOOL m_bProfileEnable;
    ESensorTG_T m_eSensorTG;
    EPCAMode_T m_ePCAMode;
    MINT32 m_i4SensorIdx;
    ESoftwareScenario m_eSoftwareScenario;
    MINT32 m_i4CamMode;
    MUINT32 m_u4SwnrEncEnableIsoThreshold;
    NS3A::AaaTimer* m_pAaaTimer;
    MINT32 m_i4FlashOnOff;
    double fColorMatrix1[3][3];
    double fForwardMatrix1[3][3];
    MBOOL  mbFlagDumpNvramMapping;
    MINT32 m_IspInterpCtrl;
};


};  //  namespace NSIspTuning
#endif // _PARAMCTRL_H_

