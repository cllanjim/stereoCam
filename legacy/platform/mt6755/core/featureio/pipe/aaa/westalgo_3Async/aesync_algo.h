#ifdef __cplusplus             
extern "C"{ 
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
//the process of ae sync
//@params AEUpdate :ae update info pointer for qcom structure
/////////////////////////////////////////////////////////////////////////////////////////////////
extern MUINT32 dcsAESyncProcess(SYNC_AE_OUTPUT_STRUCT *m_rAeSyncOutput, AE_MODE_CFG_T  *rAEInfoSlave);
/////////////////////////////////////////////////////////////////////////////////////////////////

//getting exposure table
//@params table :the exposure table from chromatix
/////////////////////////////////////////////////////////////////////////////////////////////////
extern MUINT32 dcsAESyncExposureTable(strAETable *preTable, strAETable *snaTable);

/////////////////////////////////////////////////////////////////////////////////////////////////
//the ae_sync's init function
//@params af_obj :NA
/////////////////////////////////////////////////////////////////////////////////////////////////
extern MUINT32 dcsAESyncInit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////
//the ae_sync's destory function
//@params af_obj :NA
/////////////////////////////////////////////////////////////////////////////////////////////////
extern MUINT32 dcsAESyncUnInit(void);

#ifdef __cplusplus
}
#endif


