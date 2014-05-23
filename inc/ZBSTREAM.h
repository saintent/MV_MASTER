//================ File Description =========================================//
//=== File name : ZBSTREAM.h
//===========================================================================//

#ifndef ZBSTREAM_H_
#define ZBSTREAM_H_

//================ Include Header ===========================================//
#include "lpc_types.h"
//#include "lpc12xx_libcfg.h"
#include "lpc_uart.h"
#include "lib_fifo.h"
#include "IAAP.h"
//================ PULBIC DEFINE ============================================//
#define ZB_STX		0x02
#define ZB_ETX		0x03
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
typedef void ZBS_DataCallBack_t(void* obj);
//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct {
	uint8_t		Serail[4];
	uint8_t		LightLevel;
}ZBStram_DataStruct_t;
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

class ZBSTREAM : public IAAP {
private :
	ZBS_DataCallBack_t *pZBS_DataCallBack;
public:
	ZBSTREAM();
	virtual ~ZBSTREAM();

	void Init(COM_PORT_T* port, ZBS_DataCallBack_t* pCallBack);

private :
	/*void readFrame(FIFO_ATTR_T* pFIFO, uint8_t* pDecodeData,
			uint8_t* pDecodeDataSize, uint8_t* pFrameCount);*/
	void processFrame(void);

};

//================ END OF FILE ==============================================//
#endif /* ZBSTREAM_H_ */
