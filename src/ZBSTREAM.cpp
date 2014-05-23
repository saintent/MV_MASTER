//================ File Desciption =========================================//
//=== File name : ZBSTREAM.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "ZBSTREAM.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
//
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
//
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

ZBSTREAM::ZBSTREAM() {
	// TODO Auto-generated constructor stub

}

ZBSTREAM::~ZBSTREAM() {
	// TODO Auto-generated destructor stub
}

PUBLIC void ZBSTREAM::Init(COM_PORT_T* port, ZBS_DataCallBack_t* pCallBack) {
	IAP_INIT_CB_T iapCb;
	iapCb.ActReq = 0;
	iapCb.ReadReq = 0;
	iapCb.ReadRsp = 0;
	iapCb.RegInd = 0;
	iapCb.RegReq = 0;
	iapCb.WrReq = 0;
	IAAP::Init(0, port, &iapCb);
	this->pZBS_DataCallBack = pCallBack;
}

/*PRIVATE void ZBSTREAM::readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
		uint8_t* pDecodeDataSize, uint8_t* pFrameCount) {
	uint8_t u8Data;
	if (this->frmState == FRM_DONE) {
		//frmState = FRM_READ;
		pFrameCount[0] = 0;
		return;
	}
	while (FIFO_Count(pFIFO)) {
		FIFO_GetChar(pFIFO, &u8Data, TRUE);
		switch (this->frmState) {
		case FRM_DONE:
		default:
		case FRM_READ:
			switch (u8Data) {
			case END_FRM:
				// Getting end frame
				pFrameCount[0]++;
				//pDecodeData[pDecodeDataSize[0]] = u8Data;
				this->frmState = FRM_DONE;
				return;
				break;
			default:
				// Getting frame data
				pDecodeData[pDecodeDataSize[0]] = u8Data;
				pDecodeDataSize[0]++;
				break;
			}
			break;
		case FRM_START:
			// Getting start flag
			if (u8Data == STR_FRM) {
				//pDecodeData[pDecodeDataSize[0]] = u8Data;
				//pDecodeDataSize[0]++;
				this->frmState = FRM_READ;
			}
			break;
		}
	}
}*/

PRIVATE void ZBSTREAM::processFrame(void) {
	ZBStram_DataStruct_t* pData;
	pData = (ZBStram_DataStruct_t*) this->frmBuffer;
	if (this->pZBS_DataCallBack != NULL) {
		this->pZBS_DataCallBack((void*) pData);
	}
}

//================ END OF FILE ==============================================//

