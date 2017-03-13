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
#ifndef _HWSYNC_DRV_H_
#define _HWSYNC_DRV_H_

typedef enum{
    E_SMI_LOW = 0,  //dvfs at low
    E_SMI_LEVEL_1 = 1,
    E_SMI_LEVEL_2 = 2,
    E_SMI_HIGH =3,  // dvfs at high
}E_SMI_LEVEL;


/******************************************************************************
 *
 * @enum HW_SYNC_CMD_ENUM
 * @brief cmd enum for sendCommand.
 * @details
 *
 ******************************************************************************/
typedef enum{
    HW_SYNC_CMD_SET_MODE        = 0x01,         //sensor mode(by resolution and fps)
    HW_SYNC_CMD_DISABLE_SYNC    = 0x02,          //stop hwsync syncing precedure
    HW_SYNC_CMD_ENABLE_SYNC     = 0x03,         //start hwsync syncing precedure
    HW_SYNC_CMD_GET_FRAME_TIME  = 0x04,         // AE set AE cycle period
    HW_SYNC_CMD_SET_PARA        = 0x05,         //set hwsync init para, need to send before hwsync enable
}HW_SYNC_CMD_ENUM;

/******************************************************************************
 *
 * @enum HW_SYNC_USER_ENUM
 * @brief the user enum for using hwsync driver.
 * @details
 *
 ******************************************************************************/
typedef enum{
        HW_SYNC_USER_HALSENSOR                  = 0x00,
        HW_SYNC_USER_AE                         = 0x01,
}HW_SYNC_USER_ENUM;

typedef enum{
    HW_SYNC_BLOCKING_EXE    =   0x0,
    HW_SYNC_NON_BLOCKING_EXE=   0x1,
}ENUM_HW_SYNC_EXE;


/******************************************************************************
 *
 * @class HWSyncDrv BaseClass
 * @brief Driver to sync VDs.
 * @details
 *
 ******************************************************************************/
class HWSyncDrv
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Instantiation.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:
    HWSyncDrv(){};
    virtual ~HWSyncDrv() {};
//
public:
    /**
     * @brief Create the instance
     *
     * @details
     *
     * @note
     *
     * @return
     * An instance to this class.
     *
     */
    static HWSyncDrv*   createInstance();
    /**
     * @brief destroy the pipe wrapper instance
     *
     * @details
     *
     * @note
     */
    virtual void        destroyInstance(void) = 0;
    virtual MBOOL       init(HW_SYNC_USER_ENUM user, MUINT32 sensorIdx, MINT32 aeCyclePeriod = -1) = 0;
    virtual MBOOL       uninit(HW_SYNC_USER_ENUM user, MUINT32 sensorIdx) = 0;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  General Function.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /**
        cmd:HW_SYNC_CMD_SET_PARA
            @in
            arg1: sensor's devID
            arg2: dvfs level
            arg3: sensor's shutter speed
        cmd:HW_SYNC_CMD_ENABLE_SYNC
            @in
            arg2: sensor's devID
            arg3: HW_SYNC_BLOCKING_EXE->blocked enable flow.  HW_SYNC_NON_BLOCKING_EXE->non-blocked enable flow.
        cmd:HW_SYNC_CMD_DISABLE_SYNC
            @in
            arg1: 0->blocked disable flow.  1->non-blocked disable flow.
        cmd:HW_SYNC_CMD_GET_FRAME_TIME
            @in
            arg1: current sensor's devID
            arg2: 2 sensors' shutter speed , order is decided by arg1.
            @out
            arg3: 2 sensors' frame length, order is decided by arg1.
        cmd:HW_SYNC_CMD_SET_MODE
            @in:
            arg1: sensor's devID
            arg2: sensor's scenarioID
            arg3: sensor's fps
    */
    virtual MINT32      sendCommand(HW_SYNC_CMD_ENUM cmd,MUINT32 arg1,MUINTPTR arg2, MUINTPTR arg3=HW_SYNC_BLOCKING_EXE)=0;
};
//----------------------------------------------------------------------------
#endif  // _HWSYNC_DRV_H_


