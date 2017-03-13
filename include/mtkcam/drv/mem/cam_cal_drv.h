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
/*
** $Log: cam_cal_drv.h $
 *
 *
*/

#ifndef _CAM_CAL_DRV_H_
#define _CAM_CAL_DRV_H_


//#include "camera_custom_cam_cal.h"
#define DRV_CAM_CAL_SUPPORT (0)

//#include "camera_custom_nvram.h" 160707 dream for no nvram
#ifndef USING_MTK_LDVT
    #define CAM_CAL_SUPPORT
#endif



/*****************************************************************************
 * Marco
 *****************************************************************************/
typedef signed int			MINT32;

#define CAM_CAL_MAX_LSC_SIZE 0x840 //(2112) //6589 Ver
#define MAX_SENSOR_CAL_SIZE             (2048) //Byte
#define CAM_CAL_SINGLE_LSC_SIZE 0x74c //(1868)
#define MAX_MTK_SHADING_SLIM_TABLE_SIZE (2048)//(16*16*2*4)
#define MAX_SENSOR_SHADING_TALE_SIZE MAX_SENSOR_CAL_SIZE//(2048)//(16*16*2*4)
#define CAM_CAL_N3D_NUM 2
#define CAM_CAL_N3D_LSC_SIZE CAM_CAL_MAX_LSC_SIZE
#define CAM_CAL_PDAF_SIZE 0x800 //(2048)


/*****************************************************************************
 * Enums
 *****************************************************************************/
/** @defgroup cam_cal_enum Enum
 *	@{
 */

typedef enum
{
    CAMERA_CAM_CAL_DATA_MODULE_VERSION=0,  // it's for user to get info. of single module or N3D module
    CAMERA_CAM_CAL_DATA_PART_NUMBER,       // return 5x4 byes gulPartNumberRegCamCal[5]
    CAMERA_CAM_CAL_DATA_SHADING_TABLE,     // return SingleLsc or N3DLsc
    CAMERA_CAM_CAL_DATA_3A_GAIN,           // return Single2A or N3D3A
    CAMERA_CAM_CAL_DATA_3D_GEO,            // return none or N3D3D
    CAMERA_CAM_CAL_DATA_PDAF,              // return none or PDAF
    CAMERA_CAM_CAL_DATA_LIST
}CAMERA_CAM_CAL_TYPE_ENUM;




typedef enum
{
    CAM_CAL_SINGLE_EEPROM_DATA,
    CAM_CAL_DOUBLE_EEPROM_DATA,
    CAM_CAL_SINGLE_OTP_DATA,
    CAM_CAL_N3D_DATA,
    CAM_CAL_TYPE_NUM
} CAM_CAL_DATA_VER_ENUM;


/**
* @}
*/


/*****************************************************************************
 * Structures
 *****************************************************************************/
/** @defgroup cam_cal_hanele_struct Struct
 *  @{
 */


/** @brief This Structure defines the CAM_CAL_N3D_3D_STRUCT.  */
#define CAM_CAL_N3D_3D_SIZE 2200 //6589 Ver
typedef struct
{
    unsigned char Data[CAM_CAL_N3D_NUM][CAM_CAL_N3D_3D_SIZE];
}CAM_CAL_N3D_3D_STRUCT, *PCAM_CAL_N3D_3D_STRUCT;


/** @brief This Structure defines the CAM_CAL_N3D_3A_STRUCT.  */

#define CAM_CAL_N3D_3A_SIZE 128 
typedef struct
{
    unsigned char Data[CAM_CAL_N3D_NUM][CAM_CAL_N3D_3A_SIZE];
}CAM_CAL_N3D_3A_STRUCT, *PCAM_CAL_N3D_3A_STRUCT;

	
/** @brief This Structure defines the LSC Table.  */

typedef struct {
	unsigned char  MtkLscType; //LSC Table type	"[0]sensor[1]MTK"	1
	unsigned char  PixId; //0,1,2,3: B,Gb,Gr,R
	unsigned short TableSize; //table size (2108 Bytes )		2
	unsigned int   SlimLscType; //00A0	FF 00 02 01 (4 bytes)		4
	unsigned int   PreviewWH; //00A4	Preview Width (2 bytes) Preview Height (2 bytes)	2
	unsigned int   PreviewOffSet; //00A6	Preview Offset X (2 bytes)	Preview Offset Y (2 bytes)	2
	unsigned int   CaptureWH; //00A8	Capture Width (2 bytes) Capture Height (2 bytes)	2
	unsigned int   CaptureOffSet; //00AA	Capture Offset X (2 bytes)	Capture Offfset Y (2 bytes) 2
	unsigned int   PreviewTblSize; //00AC	Preview Shading Table Size (4 bytes)		4
	unsigned int   CaptureTblSize; //00B0	Capture Shading Table Size (4 bytes)		4
	unsigned int   PvIspReg[5]; //00B4	Preview Shading Register Setting (5x4 bytes)		20
	unsigned int   CapIspReg[5]; //00C8 Capture Shading Register Setting (5x4 bytes)		20
	unsigned char  CapTable[MAX_MTK_SHADING_SLIM_TABLE_SIZE]; //00DC	Capture Shading Table (16 X 16 X 2 X 4 bytes)		2048
}CAM_CAL_LSC_MTK_TYPE;
typedef struct {
	unsigned char	MtkLscType; //LSC Table type	"[0]sensor[1]MTK"	1
	unsigned char	PixId; //0,1,2,3: B,Gb,Gr,R
	unsigned short  TableSize; //TABLE SIZE		2
	unsigned char	SensorTable[MAX_SENSOR_SHADING_TALE_SIZE]; //LSC Data (Max 2048)		2048
	unsigned char	Reserve[CAM_CAL_MAX_LSC_SIZE-sizeof(char)-sizeof(char)-sizeof(short)-(sizeof(char)*MAX_SENSOR_SHADING_TALE_SIZE)]; //
}CAM_CAL_LSC_SENSOR_TYPE;

typedef union {
		unsigned char Data[CAM_CAL_MAX_LSC_SIZE];
		CAM_CAL_LSC_MTK_TYPE	   MtkLcsData;
		CAM_CAL_LSC_SENSOR_TYPE SensorLcsData;
} CAM_CAL_LSC_DATA;



typedef struct
{
    unsigned int TableRotation;            // 1:180, 0:0 To let drv know the table convert , if mirror/Flip it's 2/4
    CAM_CAL_LSC_DATA LscTable;
}CAM_CAL_SINGLE_LSC_STRUCT, *PCAM_CAL_SINGLE_LSC_STRUCT;

/** @brief This Structure defines the 2A Table.  */

typedef struct
{
    unsigned int rGoldGainu4R;
    unsigned int rGoldGainu4G;
    unsigned int rGoldGainu4B;
    unsigned int rUnitGainu4R;
    unsigned int rUnitGainu4G;
    unsigned int rUnitGainu4B;
}CAM_CAL_PREGAIN_STRUCT, *PCAM_CAL_PREGAIN_STRUCT;

#define CAM_CAL_AWB_BITEN (0x01<<0)
#define CAM_CAL_AF_BITEN (0x01<<1)
#define CAM_CAL_NONE_BITEN (0x00)
typedef struct
{
    unsigned char  S2aVer; //verson : 01
    unsigned char  S2aBitEn; //bit enable: 03 Bit0: AF Bit1: WB
    unsigned char  S2aAfBitflagEn; //Bit: step 0(inf.), 1(marco), 2, 3, 4,5,6,7
    unsigned short S2aAf[8];      //0x012c
    CAM_CAL_PREGAIN_STRUCT S2aAwb; //0x012c
}CAM_CAL_SINGLE_2A_STRUCT, *PCAM_CAL_SINGLE_2A_STRUCT;


/** @brief This Structure defines the CAM_CAL_N3D_LSC Table.  */
typedef struct
{
    //MUINT8 Data[CAM_CAL_N3D_NUM][CAM_CAL_MAX_LSC_SIZE];
    CAM_CAL_SINGLE_LSC_STRUCT Data[CAM_CAL_N3D_NUM];
}CAM_CAL_N3D_LSC_STRUCT, *PCAM_CAL_N3D_LSC_STRUCT;



/** @brief This enum defines the CAM_CAL Table.  */

typedef struct
{
    unsigned char Data[CAM_CAL_PDAF_SIZE];
}CAM_CAL_PDAF_STRUCT, *PCAM_CAL_PDAF_STRUCT;


/** @brief This enum defines the CAM_CAL Table.  */

typedef struct
{
    CAMERA_CAM_CAL_TYPE_ENUM Command;   //it tells cam_cal driver to return what users want. dream CAMERA_CAM_CAL_TYPE_ENUM-> int
    CAM_CAL_DATA_VER_ENUM DataVer;
    unsigned char PartNumber[24];
    unsigned int  sensorID;
    unsigned int  deviceID;
    CAM_CAL_SINGLE_LSC_STRUCT	SingleLsc;
    CAM_CAL_SINGLE_2A_STRUCT	Single2A;
    CAM_CAL_N3D_LSC_STRUCT		N3DLsc;
    CAM_CAL_N3D_3A_STRUCT		N3D3A;
    CAM_CAL_N3D_3D_STRUCT		N3D3D;
    CAM_CAL_PDAF_STRUCT			PDAF;
}CAM_CAL_DATA_STRUCT, *PCAM_CAL_DATA_STRUCT;


/**
 * @}
 */

/*****************************************************************************
 * Arrary
 *****************************************************************************/
/** @defgroup cam_cal_hanele_struct Struct
 *  @{
 */

#define CAM_CAL_ERR_NO_ERR          0x00000000
#define CAM_CAL_ERR_NO_VERSION  	0x00000001
#define CAM_CAL_ERR_NO_PARTNO   	0x00000010
#define CAM_CAL_ERR_NO_SHADING 		0x00000100
#define CAM_CAL_ERR_NO_3A_GAIN 		0x00001000
#define CAM_CAL_ERR_NO_3D_GEO   	0x00010000
#define CAM_CAL_ERR_NO_PDAF   		0x00100000

#define CamCalReturnErr_MAX 6
const unsigned int CamCalReturnErr[CamCalReturnErr_MAX]= {
                                                    CAM_CAL_ERR_NO_VERSION,
                                                    CAM_CAL_ERR_NO_PARTNO,
                                                    CAM_CAL_ERR_NO_SHADING,
                                                    CAM_CAL_ERR_NO_3A_GAIN,
                                                    CAM_CAL_ERR_NO_3D_GEO};

const char CamCalErrString[CamCalReturnErr_MAX][24]={
                                                  {"ERR_NO_VERSION"},
                                                  {"ERR_NO_PARTNO"},
                                                  {"ERR_NO_SHADING"},
                                                  {"ERR_NO_3A_GAIN"},
                                                  {"ERR_NO_3D_GEO"}
                                                  };
/**
 * @}
 */



/*******************************************************************************
*
********************************************************************************/

class CamCalDrvBase {

public:
    /////////////////////////////////////////////////////////////////////////
    //
    // createInstance () -
    //! \brief create instance
    //
    /////////////////////////////////////////////////////////////////////////
    static CamCalDrvBase* createInstance();

    /////////////////////////////////////////////////////////////////////////
    //
    // destroyInstance () -
    //! \brief destroy instance
    //
    /////////////////////////////////////////////////////////////////////////
    virtual void destroyInstance() = 0;

    /////////////////////////////////////////////////////////////////////////
    //
    // readCamCal () -
    //! \brief
    //
    /////////////////////////////////////////////////////////////////////////
	virtual int GetCamCalCalData(unsigned long i4SensorDevId,
                          /*unsigned long u4SensorID,*/
                          CAMERA_CAM_CAL_TYPE_ENUM a_eCamCalDataType,
	                      void *a_pCamCalData) = 0;

protected:
    /////////////////////////////////////////////////////////////////////////
    //
    // ~CamCalDrvBase () -
    //! \brief descontrustor
    //
    /////////////////////////////////////////////////////////////////////////
    virtual ~CamCalDrvBase() {}

private:
	
};





#endif  

