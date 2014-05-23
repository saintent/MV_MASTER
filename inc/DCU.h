//================ File Despciption =========================================//
//=== File name : DCU.h
//===========================================================================//

#ifndef DCU_H_
#define DCU_H_

//================ Include Header ===========================================//
#include "stdint.h"
#include "lib_fifo.h"
//================ PULBIC DEFINE ============================================//
#define DCU_TX_BUFFER	128
#define DCU_RX_BUFFER	128
#define SLAVE_ADDR				0xA0 	// LPC122x as slave address
#define CLK_RATE				400000
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	I2C_IDLE = 0,
	I2C_STARTED,
	I2C_RESTARTED,
	I2C_REPEATED_START,
	DATA_ACK,
	DATA_NACK,
	I2C_WR_STARTED,
	I2C_RD_STARTED,
	I2C_NACK_ON_ADDRESS,
	I2C_NACK_ON_DATA,
	I2C_ARBITRATION_LOST,
	I2C_TIME_OUT,
	I2C_OK
}I2CState_t;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
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

class DCU {
public:
	DCU();
	virtual ~DCU();

	void Init();

	//static void I2CWrCallback(DCU* obj, uint8_t data);
	//static uint8_t I2CRdCallback(DCU* obj);
private :
	//uint8_t TxBuffer[DCU_TX_BUFFER];
	//uint8_t RxBuffer[DCU_RX_BUFFER];
	//FIFO_ATTR_T  TxFIFO;
	//FIFO_ATTR_T  RxFIFO;
};

//================ END OF FILE ==============================================//
#endif /* DCU_H_ */
