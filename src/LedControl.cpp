//================ File Despciption =========================================//
//=== File name : LedControl.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "LedControl.h"
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
const uint16_t PWM_PORT[2] = {IOCON_CT16_B0_MAT0_LOC1 , IOCON_CT16_B0_MAT1_LOC1 };
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

LedControl::LedControl() {
	// TODO Auto-generated constructor stub

}

LedControl::~LedControl() {
	// TODO Auto-generated destructor stub
}

void LedControl::Init(void) {
	IOCON_PIO_CFG_Type pwmIO;
	TIM16_InitTypeDef timInit;
	TIM16_PWMTypeDef pwmCfg;
	IOCON_StructInit(&pwmIO);
	pwmIO.type = PWM_PORT[PWM1];
	IOCON_SetFunc(&pwmIO);
	pwmIO.type = PWM_PORT[PWM2];
	IOCON_SetFunc(&pwmIO);


	//TIM16_StructInit(TIM16_TIMER_MODE, &timInit);
	timInit.PrescaleOption = TIM16_PRESCALE_TICKVAL;
	timInit.PrescaleValue =
	TIM16_Init(LPC_CT16B0, TIM16_TIMER_MODE, &timInit);

	pwmCfg.MatchChannel = TIM16_PWM_CH0;
	pwmCfg.IntOnMatch = DISABLE;
	pwmCfg.StopOnMatch = DISABLE;
	pwmCfg.ResetOnMatch = DISABLE;
	pwmCfg.ExtPWMOutput = ENABLE;
	pwmCfg.MatchValue = 0;
	// Channel 1
	TIM16_ConfigPWM(LPC_CT16B0, &pwmCfg);
	pwmCfg.MatchChannel = TIM16_PWM_CH1;
	TIM16_ConfigPWM(LPC_CT16B0, &pwmCfg);
}

//================ END OF FILE ==============================================//
