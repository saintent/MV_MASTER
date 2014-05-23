//================ File Despciption =========================================//
//=== File name : lpc_gpio.c
//
//===========================================================================//



//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "lpc_gpio.h"
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
void GPIO_SetOut(LPC_GPIO_TypeDef* pGPIO, uint32_t value) {
	pGPIO->OUT = value;
}
void GPIO_SetHigh(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi) {
	pGPIO->SET = 1 << bitPosi;
}
void GPIO_SetLow(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi) {
	pGPIO->CLR = 1 << bitPosi;
}
void GPIO_Toggle(LPC_GPIO_TypeDef* pGPIO, GPIO_Pin_Typedef bitPosi) {
	pGPIO->NOT = 1 << bitPosi;
}
void GPIO_SetDir(LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi, GPIO_Dir_Typedef value) {
	pGPIO->DIR &= ~(1 << bitPosi);
	pGPIO->DIR |= value << bitPosi;
}
uint32_t GPIO_Get(LPC_GPIO_TypeDef* pGPIO) {
	return pGPIO->PIN;
}
uint32_t GPIO_GetPin( LPC_GPIO_TypeDef* pGPIO, uint8_t bitPosi) {
	return ((GPIO_Get(pGPIO) >> bitPosi) & 0x01);
}


//================ END OF FILE ==============================================//
