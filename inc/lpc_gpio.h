//================ File Despciption =========================================//
//=== File name : lpc_gpio.h
//===========================================================================//

#ifndef LPC_GPIO_H_
#define LPC_GPIO_H_

//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "lpc_types.h"
//================ PULBIC DEFINE ============================================//
typedef enum {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1,	GPIO_PIN_2,	GPIO_PIN_3,
	GPIO_PIN_4,	GPIO_PIN_5,	GPIO_PIN_6,	GPIO_PIN_7,
	GPIO_PIN_8,	GPIO_PIN_9,	GPIO_PIN_10,GPIO_PIN_11,
	GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15,
	GPIO_PIN_16,GPIO_PIN_17,GPIO_PIN_18,GPIO_PIN_19,
	GPIO_PIN_20,GPIO_PIN_21,GPIO_PIN_22,GPIO_PIN_23,
	GPIO_PIN_24,GPIO_PIN_25,GPIO_PIN_26,GPIO_PIN_27,
	GPIO_PIN_28,GPIO_PIN_29,GPIO_PIN_30,GPIO_PIN_31,
}GPIO_Pin_Typedef;

typedef enum {
	GPIO_DIR_IN = 0,
	GPIO_DIR_OUT
}GPIO_Dir_Typedef;

//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
//================ EXTERN FUNCTION ==========================================//
#ifdef __cplusplus
extern "C"
{
#endif

uint32_t GPIO_Get(LPC_GPIO_TypeDef* pGPIO);
uint32_t GPIO_GetPin( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi);
void GPIO_SetOut(LPC_GPIO_TypeDef* pGPIO, uint32_t value);
void GPIO_SetHigh(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi);
void GPIO_SetLow(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi);
void GPIO_Toggle(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi);
void GPIO_SetDir(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, GPIO_Dir_Typedef value);
//uint32_t GPIO_GetDir( LPC_GPIO_Type* pGPIO, uint8_t bitPosi);

#ifdef __cplusplus
}
#endif
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ END OF FILE ==============================================//
#endif /* LPC_GPIO_H_ */
