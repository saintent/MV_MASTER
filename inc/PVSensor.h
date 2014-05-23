//================ File Despciption =========================================//
//=== File name : PVSensor.h
//===========================================================================//

#ifndef PVSENSOR_H_
#define PVSENSOR_H_

//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "lpc12xx_adc.h"
#include "lpc12xx_iocon.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	ADC_FREE = 0,
	ADC_ONREAD
}Adc_Status;

typedef enum {
	SENSOR_CH1 = 0,
	SENSOR_CH2,
	SENSOR_CH3,
	SENSOR_MAX
}SensorChannal_Typdef;
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

class PVSensor {
public:
	PVSensor();
	virtual ~PVSensor();
	static void ReadCallback(PVSensor* obj, SensorChannal_Typdef ch);
	void Init();
	Adc_Status Read();
	uint16_t GetValue(SensorChannal_Typdef ch);
private :
	Adc_Status onRead;
	uint16_t Value[SENSOR_MAX];
};

//================ END OF FILE ==============================================//
#endif /* PVSENSOR_H_ */
