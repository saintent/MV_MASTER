//================ File Description =========================================//
//=== File name : IAAP.h
//===========================================================================//

#ifndef IAAP_H_
#define IAAP_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
//#include "lpc12xx_libcfg.h"
#include "lpc_uart.h"
#include "lib_fifo.h"
#include "fcs.h"
//================ PULBIC DEFINE ============================================//
#define STR_FRM			0xFE
#define END_FRM			0xFC
#define RS485_PORT		UART0
#define RS485_LOC		UART_LOC_0
#define ZB_PORT			UART1
#define ZB_LOC			UART_LOC_0
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	PHY_RS485 = 0,
	PHY_ZB,
	PHY_MAX
}PHY_OBJ_T;

typedef enum {
	FRM_START = 0,
	FRM_READ,
	FRM_DONE
}FRM_STATE_T;

typedef enum {
	PRIM_ASSOCIATION_REQ = 0,
	PRIM_ASSOCIATION_IND,
	PRIM_ASSOCIATION_RSP,
	PRIM_ASSOCIATION_CNF,

	PRIM_REGISTER_REQ,
	PRIM_REGISTER_IND,
	PRIM_REGISTER_RSP,
	PRIM_REGISTER_CNF,

	PRIM_READ_REQ,
	PRIM_READ_IND,
	PRIM_READ_RSP,
	PRIM_READ_CNF,

	PRIM_WRITE_REQ,
	PRIM_WRITE_IND,
	PRIM_WRITE_RSP,
	PRIM_WRITE_CNF,

	PRIM_ACTION_REQ,
	PRIM_ACTION_IND,
	PRIM_ACTION_RSP,
	PRIM_ACTION_CNF,

}PRIM_TYPE_T;

typedef enum {
	PRIM_ASSOCIATION = 1,
	PRIM_REGISTER,
	PRIM_READ,
	PRIM_WRITE,
	PRIM_ACTION
}PRIM_CMD_TYPE_T;

typedef enum {
	ASS_LOGIN = 0x01,
	ASS_DIS = 0x02
}ASSOCIATION_TYPE_T;


typedef enum {
	REG_MOTOR = 0x00,
	REG_VALVE,
	REG_HEATER,
	REG_TEMPSENSOR
}IAAR_REGISTER_T;

//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct {
	Uart_type 	Port;
	UART_LOC_T	Loc;
	uint16_t	rsv;
	uint32_t	Speed;
}COM_PORT_T;

typedef struct {
	COM_PORT_T	Port;				// 8 btye
	uint8_t		TxBuff[32];			// 32 byte
	uint8_t		RxBuff[64];			// 32 byte
	FIFO_ATTR_T	TxFifo;				// 16 byte
	FIFO_ATTR_T	RxFifo;				// 16 byte
	//uint8_t		rsv[6];				// Reserve 20 byte
				// Over all			80 byte
}PHY_OBJECT_T;

typedef struct {
	uint8_t		Lenght;
	uint8_t		Dest;
	uint8_t		Source;
	uint8_t		Cmd;
	uint8_t		PrimitiveEntry;
}FRM_HEADER_T;

typedef struct {
	uint8_t		Type:3;
	uint8_t		PF:1;
	uint8_t		CMD:4;
}PRIM_STC_TYPE_T;

typedef struct {
	uint8_t		isReponseReq;
}IAP_STATE_TYPE_T;

typedef struct {
	uint8_t		NumberOfNof;
	uint8_t		Type;
	uint8_t		Entry;
}PRIM_REG_TYPR_T;

typedef struct {
	uint8_t		Id;
	uint8_t		Type;
	uint8_t		Data;
}PRIM_RD_TYPE_T;

typedef struct {
	uint8_t		Id;
	uint8_t 	Type;
	uint8_t		Data;
}PRIM_RD_RSP_TYPE_T;

typedef struct {
	uint8_t		Id;
	uint8_t		Data;
}PRIM_WR_TYPE_T;

typedef struct {
	uint8_t 	Id;
	uint8_t 	Type;
	uint8_t		Data;
}PRIM_ACT_TYPE_T;

typedef struct {
	uint8_t 	Enable;
	uint8_t		Count;
	uint8_t 	Interval;
	uint8_t		rsv;
}IAP_ALARM_TYPE_T;

typedef void IAP_RegReq_Cb_t(void* obj, uint8_t addr, uint8_t id);
typedef void IAP_RegInd_Cb_t(void* obj, uint8_t addr, PRIM_REG_TYPR_T* prim);
typedef void IAP_ReadReq_Cb_t(void* obj, uint8_t addr, PRIM_RD_TYPE_T* prim);
typedef void IAP_ReadRsp_Cb_t(void* obj, uint8_t addr, PRIM_RD_RSP_TYPE_T* prim);
typedef void IAP_WriteReq_Cb_t(void* obj, uint8_t addr, PRIM_WR_TYPE_T* prim);
typedef void IAP_ActionReq_Cb_t(void* obj, uint8_t addr, PRIM_ACT_TYPE_T* prim);

typedef struct {
	IAP_RegReq_Cb_t* 	RegReq;
	IAP_RegInd_Cb_t* 	RegInd;
	IAP_ReadReq_Cb_t* 	ReadReq;
	IAP_ReadRsp_Cb_t* 	ReadRsp;
	IAP_WriteReq_Cb_t*	 WrReq;
	IAP_ActionReq_Cb_t*	 ActReq;
}IAP_INIT_CB_T;
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ CLASS DECLARATION ========================================//

class IAAP {
public:

public:
	IAAP();
	virtual ~IAAP();
 	virtual void Init(uint8_t addr, COM_PORT_T* port,IAP_INIT_CB_T* CbFn);
	void AlarmTick(void);
	void AlarmRefesh(void);
	void AlarmEnable(void);
	void AlarmDisable(void);
	Uart_type GetPort();
	Bool PendingProcess(void);
	void Interactive(void);
	void PhyCallback(Uart_type t, uint8_t data);
	void PhySentCallback(Uart_type t);
	void SentByte(void);
	void SetAddress(uint8_t addr);
	uint8_t RequestToSend(PRIM_TYPE_T type, uint8_t destAddr, uint8_t* data, uint8_t len);
private :
	IAP_RegReq_Cb_t* iap_RegReqCb;
	IAP_RegInd_Cb_t* iap_RegIndCb;
	IAP_ReadReq_Cb_t* iap_RdRegCb;
	IAP_ReadRsp_Cb_t* iap_RdRspCb;
	IAP_ActionReq_Cb_t* iap_ActReqCb;
	IAP_WriteReq_Cb_t* iap_WrRegCb;

protected :
	IAP_ALARM_TYPE_T alarm;
	PHY_OBJECT_T 	phyObj;
	uint8_t 		addr;
	uint8_t 		frmBuffer[32];
	uint8_t 		frmLength;
	FRM_STATE_T 	frmState;


private :

	void sentFrame(uint8_t dest, uint8_t cmd, uint8_t* rspFrm, uint8_t frmSize);
	Bool readToProperty(uint8_t* srcAddr, PRIM_TYPE_T* primType, uint8_t** property);

protected :
	virtual void readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
			uint8_t* pDecodeDataSize, uint8_t* pFrameCount);
	virtual void processFrame(void);
};

//================ END OF FILE ==============================================//
#endif /* IAAP_H_ */
