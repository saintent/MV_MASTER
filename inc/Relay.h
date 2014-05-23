//================ File Despciption =========================================//
//=== File name : Relay.h
//===========================================================================//

#ifndef RELAY_H_
#define RELAY_H_

//================ Include Header ===========================================//
#include "lpc_gpio.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	RELAY_CLOSE = 0,
	RELAY_OPEN
}Relay_Status;
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

class Relay {
public:
	Relay();
	Relay(LPC_GPIO_TypeDef* gPIO, GPIO_Pin_Typedef pin);
	virtual ~Relay();
	void Init(LPC_GPIO_TypeDef* gPIO, GPIO_Pin_Typedef pin);
	void Open();
	void Close();
	Relay_Status GetState();
private :
	Relay_Status Status;
	LPC_GPIO_TypeDef* Port;
	GPIO_Pin_Typedef Pin;
};

//================ END OF FILE ==============================================//
#endif /* RELAY_H_ */
