//================ File Desciption =========================================//
//=== File name : IAAP.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "IAAP.h"
#include "string.h"
#include "lpc_uart.h"
#include "lib_fifo.h"
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

IAAP::IAAP() {
	// TODO Auto-generated constructor stub

}

IAAP::~IAAP() {
	// TODO Auto-generated destructor stub
}

PUBLIC void IAAP::Init(uint8_t addr, COM_PORT_T* port,IAP_INIT_CB_T* CbFn) {
	FIFO_ATTR_T* rxFIFO;
	FIFO_ATTR_T* txFIFO;
	uint8_t*	rxBuff;
	uint8_t* 	txBuff;
	rxFIFO = &this->phyObj.RxFifo;
	txFIFO = &this->phyObj.TxFifo;
	rxBuff = this->phyObj.RxBuff;
	txBuff = this->phyObj.TxBuff;
	this->addr = addr;
	this->phyObj.Port = *port;
	this->iap_RegReqCb = CbFn->RegReq;
	this->iap_RegIndCb = CbFn->RegInd;
	this->iap_RdRegCb = CbFn->ReadReq;
	this->iap_RdRspCb = CbFn->ReadRsp;
	this->iap_WrRegCb = CbFn->WrReq;
	this->iap_ActReqCb = CbFn->ActReq;
	this->frmState = FRM_START;
	this->alarm.Interval = 100;
	this->alarm.Count = 0;
	this->alarm.Enable = FALSE;

	FIFO_Init(rxFIFO, rxBuff, 64);
	FIFO_Init(txFIFO, txBuff, 32);

}
PUBLIC void IAAP::AlarmTick(void) {
	FIFO_ATTR_T* fifo;
	if (this->alarm.Enable) {
		// Check Time out
		if (++this->alarm.Count >= this->alarm.Interval) {
			// Process
			// Discard frame, reset state
			fifo = &this->phyObj.RxFifo;
			// Clear all data in fifo
			FIFO_Flush(fifo);
			this->frmState = FRM_START;
			this->frmLength = 0;
			this->alarm.Count = 0;
			this->alarm.Enable = FALSE;
		}
	}
}
PUBLIC void IAAP::AlarmRefesh(void) {
	this->alarm.Count = 0;
}
PUBLIC void IAAP::AlarmEnable(void) {
	if (!this->alarm.Enable) {
		this->alarm.Enable = TRUE;
		this->alarm.Count = 0;
	}
}
PUBLIC void IAAP::AlarmDisable(void) {
	this->alarm.Enable = FALSE;
}
PUBLIC Uart_type IAAP::GetPort(void) {
	return this->phyObj.Port.Port;
}
PUBLIC Bool IAAP::PendingProcess(void) {
	FIFO_ATTR_T* fifo;
	Bool result;
	fifo = &phyObj.RxFifo;
	if (FIFO_Count(fifo) != 0) {
		result = TRUE;
	}
	else {
		result = FALSE;
	}

	return result;
}
PUBLIC void IAAP::Interactive(void) {
	uint8_t frmCount;
	FIFO_ATTR_T* fifo;
	// Check State
	// Start Alarm
	if (this->alarm.Enable != TRUE) {
		AlarmEnable();
	}
	else {
		AlarmRefesh();
	}


	// Get FIFO
	fifo = &this->phyObj.RxFifo;

	// Read frame
	readFrame(fifo, frmBuffer, &frmLength, &frmCount);
	// If getting frame
	if (this->frmState == FRM_DONE) {
		processFrame();
		this->frmState = FRM_START;
		this->frmLength = 0;
		AlarmDisable();
	}
}
PUBLIC void IAAP::PhyCallback(Uart_type t, uint8_t data) {
	(void) t;
	FIFO_ATTR_T* fifo;
	fifo = &this->phyObj.RxFifo;
	FIFO_PutChar(fifo, data);
}
PUBLIC void IAAP::PhySentCallback(Uart_type t) {
	uint8_t data;
	FIFO_ATTR_T* fifo;
	fifo = &this->phyObj.TxFifo;
	if (FIFO_Count(fifo) != 0) {
		FIFO_GetChar(fifo, &data, TRUE);
		UARTSendCh(t, data);
	}
	else {
		UART_StopSent(t);
	}

}
PUBLIC uint8_t IAAP::RequestToSend(PRIM_TYPE_T type, uint8_t destAddr, uint8_t* data, uint8_t len) {
	uint8_t primCmd;
	switch (type) {
	// Use for next revision
	case PRIM_ASSOCIATION_REQ:
		break;
	case PRIM_ASSOCIATION_IND:
		break;
	case PRIM_ASSOCIATION_RSP:
		break;
	case PRIM_ASSOCIATION_CNF:
		break;

		// With DCU
	case PRIM_REGISTER_REQ:
		primCmd = PRIM_REGISTER << 4;
		primCmd |= 0;
		break;
		// With DCU and Zigbee
	case PRIM_REGISTER_IND:
		primCmd = PRIM_REGISTER << 4;
		primCmd |= 1;
		break;
		// With DCU. not use now
	case PRIM_REGISTER_RSP:
		primCmd = PRIM_REGISTER << 4;
		primCmd |= 2;
		break;
		// Not Use now
	case PRIM_REGISTER_CNF:
		primCmd = PRIM_REGISTER << 4;
		primCmd |= 3;
		break;

		// With DCU
	case PRIM_READ_REQ:
		primCmd = PRIM_READ << 4;
		primCmd |= 0;
		break;
		// With DCU
	case PRIM_READ_IND:
		primCmd = PRIM_READ << 4;
		primCmd |= 1;
		break;
	case PRIM_READ_RSP :
		primCmd = PRIM_READ << 4;
		primCmd |= 2;
		break;
		// Not Use now
	case PRIM_READ_CNF:
		primCmd = PRIM_READ << 4;
		primCmd |= 3;
		break;

		// With DCU
	case PRIM_WRITE_REQ:
		primCmd = PRIM_WRITE << 4;
		primCmd |= 0;
		break;
		// With DCU
	case PRIM_WRITE_IND:
		primCmd = PRIM_WRITE << 4;
		primCmd |= 1;
		break;
		// With DCU, Zigbee
	case PRIM_WRITE_RSP:
		primCmd = PRIM_WRITE << 4;
		primCmd |= 2;
		break;
		// Not Use now
	case PRIM_WRITE_CNF:
		primCmd = PRIM_WRITE << 4;
		primCmd |= 3;
		break;

		// With DCU
	case PRIM_ACTION_REQ:
		primCmd = PRIM_ACTION << 4;
		primCmd |= 0;
		break;
		// With DCU
	case PRIM_ACTION_IND:
		primCmd = PRIM_ACTION << 4;
		primCmd |= 1;
		break;
		// With DCU, Zigbee
	case PRIM_ACTION_RSP:
		primCmd = PRIM_ACTION << 4;
		primCmd |= 2;
		break;
		// Not Use now
	case PRIM_ACTION_CNF:
		primCmd = PRIM_ACTION << 4;
		primCmd |= 3;
		break;
	}
	sentFrame(destAddr, primCmd, data, len);
	return 1;
}

PRIVATE void IAAP::readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
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
				if(++pDecodeDataSize[0] >= 64) {
					// Fully Frame buffer
					this->frmState = FRM_START;
					pDecodeDataSize[0] = 0;
				}
				//pDecodeDataSize[0]++;
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
}
PRIVATE void IAAP::processFrame(void) {
	PRIM_REG_TYPR_T* primReg;
	PRIM_RD_TYPE_T* primRd;
	PRIM_RD_RSP_TYPE_T* primRdRsp;
	PRIM_WR_TYPE_T* primWr;
	PRIM_ACT_TYPE_T* primAct;
	PRIM_TYPE_T primType;
	uint8_t *primEntry;
	Bool frmError;
	uint8_t srcAddr;

	frmError = readToProperty(&srcAddr, &primType, &primEntry);

	if(frmError != FALSE) {
		switch (primType) {
		// Use for next revision
		case PRIM_ASSOCIATION_REQ :
			break;
		case PRIM_ASSOCIATION_IND :
			break;
		case PRIM_ASSOCIATION_RSP :
			break;
		case PRIM_ASSOCIATION_CNF :
			break;

			// With DCU
		case PRIM_REGISTER_REQ :
			// Getting request and response to DCU
			// call back to upper layer
			// primEntry is id to request
			if (this->iap_RegReqCb != NULL) {
				this->iap_RegReqCb((void*)this, srcAddr, primEntry[0]);
			}
			break;
			// With DCU and Zigbee
		case PRIM_REGISTER_IND :
			// Getting prim from Lamp
			primReg = (PRIM_REG_TYPR_T*) primEntry;
			if (this->iap_RegIndCb != NULL) {
				this->iap_RegIndCb((void*)this, srcAddr, primReg);
			}
			/*if (primReg->Type == 0) {
				// Register type is Master
				while (primReg->NumberOfNof) {
					// Getting LAN ID
					//call back to upper layer
					this->iap_RegIndCb(prim)
				}
			}
			else {
				// Register type is Lamp
				while (primReg->NumberOfNof) {
					// Getting Lamp ID
					// call back to upper layer
				}
			}*/
			break;
			// With DCU. not use now
		case PRIM_REGISTER_RSP :

			break;
			// Not Use now
		case PRIM_REGISTER_CNF :
			break;

			// With DCU
		case PRIM_READ_REQ :
			// Getting requirement
			primRd = (PRIM_RD_TYPE_T*) primEntry;
			if (this->iap_RdRegCb != NULL) {
				this->iap_RdRegCb((void*)this, srcAddr, primRd);
			}
			break;
			// With DCU
		case PRIM_READ_IND :
			break;
		case PRIM_READ_RSP :
			primRdRsp = (PRIM_RD_RSP_TYPE_T*) primEntry;
			if (this->iap_RdRspCb != NULL) {
				this->iap_RdRspCb((void*)this, srcAddr, primRdRsp);
			}
			break;
			// Not Use now
		case PRIM_READ_CNF :
			break;

			// With DCU
		case PRIM_WRITE_REQ :
			primWr = (PRIM_WR_TYPE_T*) primEntry;
			if (this->iap_WrRegCb != NULL) {
				this->iap_WrRegCb((void*)this, srcAddr, primWr);
			}
			break;
			// With DCU
		case PRIM_WRITE_IND :
			break;
			// With DCU, Zigbee
		case PRIM_WRITE_RSP :
			break;
			// Not Use now
		case PRIM_WRITE_CNF :
			break;

			// With DCU
		case PRIM_ACTION_REQ :
			primAct = (PRIM_ACT_TYPE_T*) primEntry;
			if (this->iap_ActReqCb != NULL) {
				this->iap_ActReqCb((void*)this, srcAddr, primAct);
			}
			break;
			// With DCU
		case PRIM_ACTION_IND :
			break;
			// With DCU, Zigbee
		case PRIM_ACTION_RSP :
			break;
			// Not Use now
		case PRIM_ACTION_CNF :
			break;
		}
	}
}
PRIVATE Bool IAAP::readToProperty(uint8_t* srcAddr, PRIM_TYPE_T *primType, uint8_t **property) {
	uint8_t primFrmType;
	FRM_HEADER_T *frmHeader;
	PRIM_STC_TYPE_T *prim;
	frmHeader = (FRM_HEADER_T*) this->frmBuffer;
	prim = (PRIM_STC_TYPE_T*) &frmHeader->Cmd;
	// Check FCS
	if (FCS::FcsCheck(this->frmBuffer, frmHeader->Lenght) == FALSE) {
		return FALSE;
	}
	// Check address
	if (frmHeader->Dest != this->addr) {
		return FALSE;
	}
	srcAddr[0] = frmHeader->Source;
	primFrmType = prim->Type;
	// Get Primitives
	switch (prim->CMD) {
	case 1 : // Association
		primType[0] = (PRIM_TYPE_T) (PRIM_ASSOCIATION_REQ + prim->Type);
		break;
	case 2 : // Register
		primType[0] = (PRIM_TYPE_T) (PRIM_REGISTER_REQ + prim->Type);
		break;
	case 3 : // Read
		primType[0] = (PRIM_TYPE_T) (PRIM_READ_REQ + prim->Type);
		break;
	case 4 : // Write
		primType[0] = (PRIM_TYPE_T) (PRIM_WRITE_REQ + prim->Type);
		break;
	case 5 : // Action
		primType[0] = (PRIM_TYPE_T) (PRIM_ACTION_REQ + prim->Type);
	}
	property[0] = &frmHeader->PrimitiveEntry;
	return TRUE;

}
PRIVATE void IAAP::sentFrame(uint8_t dest, uint8_t cmd, uint8_t* rspFrm, uint8_t frmSize) {
	uint8_t frm[64];
	uint8_t len;
	uint8_t pos;
	uint16_t fcs;
	FIFO_ATTR_T* pfifo;
	pos = 0;
	len = 0;
	// Start Flag
	frm[pos++] = STR_FRM;
	// Jump to destination address
	pos++;
	// Destination
	frm[pos++] = dest;
	// Source
	frm[pos++] = this->addr;
	// Command
	frm[pos++] = cmd;
	// Payload
	if(frmSize != 0) {
		memcpy(&frm[pos], rspFrm, frmSize);
	}
	len = frmSize + 6;
	frm[1] = len;
	FCS::FcsRun(&frm[1], len, &fcs);
	pos += frmSize;
	// FCS
	frm[pos++] = (fcs >> 8) & 0xFF;
	frm[pos++] = (uint8_t) fcs;
	frm[pos++] = END_FRM;
	len = pos;
	pos = 0;
	pfifo = &phyObj.TxFifo;
	// Put to FIFO
	while (len) {
		FIFO_PutChar(pfifo, frm[pos]);
		pos++;
		len--;
	}
	UART_StartSent(phyObj.Port.Port);
	PhySentCallback(phyObj.Port.Port);
}


//================ END OF FILE ==============================================//

