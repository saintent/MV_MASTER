//================ File Despciption =========================================//
//=== File name : LedControl.h
//===========================================================================//

#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_

//================ Include Header ===========================================//
#include "lpc122x.h"
#include "lpc_types.h"
#include "lpc12xx_iocon.h"
#include "lpc12xx_timer16.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
/*typedef enum {
	LED_OFF = 0,
	LED_ON
}LED_status;*/

typedef enum {
	PWM1 = 0,
	PWM2
}PWM_Channel;
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

class LedControl {
public:
	LedControl();
	virtual ~LedControl();

	void Init(void);
	//LED_status LampControl(LED_status sta);
	void Dimimg(uint8_t lv);
	uint8_t GetDimLevel(void);
private :
	uint8_t DimLevel;
	uint32_t RawValue;
};

//================ END OF FILE ==============================================//
#endif /* LEDCONTROL_H_ */
