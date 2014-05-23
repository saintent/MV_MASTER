//================ File Despciption =========================================//
//=== File name : PVSensor.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "PVSensor.h"
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

PVSensor::PVSensor() {
	// TODO Auto-generated constructor stub

}

PVSensor::~PVSensor() {
	// TODO Auto-generated destructor stub
}

void PVSensor::Init() {
	// Config pin function
	IOCON_PIO_CFG_Type adio;
	IOCON_StructInit(&adio);
	adio.type = IOCON_ADC_AD2;
	adio.pinmode = IOCON_PIO_MODE_INACTIVE;
	adio.ad = IOCON_PIO_AD_ANALOG;
	// ADC channel select function
	IOCON_SetFunc(&adio);
	adio.type = IOCON_ADC_AD3;
	IOCON_SetFunc(&adio);
	adio.type = IOCON_ADC_AD4;
	IOCON_SetFunc(&adio);

	// ADC Channel 2 for Sensor 1
	ADC_ChannelCmd(ADC_CHANNEL_2, ENABLE);
	// ADC Channel 3 for sensor 2
	ADC_ChannelCmd(ADC_CHANNEL_3, ENABLE);
	// ADC Channel 4 for sensor 3
	ADC_ChannelCmd(ADC_CHANNEL_4, ENABLE);

	ADC_IntConfig(ADC_CHANNEL_2, ENABLE);
}

Adc_Status PVSensor::Read() {
	if(!this->onRead) {
		ADC_StartCmd(ADC_START_NOW);
		this->onRead = ADC_ONREAD;
	}
	return this->onRead;
}

uint16_t PVSensor::GetValue(SensorChannal_Typdef ch) {
	uint16_t val = 0;
	if(ch != SENSOR_MAX) {
		val = Value[ch];
	}
	return val;
}

void PVSensor::ReadCallback(PVSensor* obj, SensorChannal_Typdef ch) {
	switch (ch) {
		case SENSOR_CH1:
			obj->Value[SENSOR_CH1] = (uint16_t)ADC_GetData(ADC_CHANNEL_2);
		break;
		case SENSOR_CH2:
			obj->Value[SENSOR_CH2] = (uint16_t)ADC_GetData(ADC_CHANNEL_3);
		break;
		case SENSOR_CH3:
			obj->Value[SENSOR_CH3] = (uint16_t)ADC_GetData(ADC_CHANNEL_4);
		break;
		default:
		break;
	}
	obj->onRead = ADC_FREE;
}

//================ END OF FILE ==============================================//
