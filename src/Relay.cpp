//================ File Despciption =========================================//
//=== File name : Relay.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "Relay.h"
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

Relay::Relay() {

}

Relay::Relay(LPC_GPIO_TypeDef* gPIO, GPIO_Pin_Typedef pin) {
	// TODO Auto-generated constructor stub
	this->Port = gPIO;
	this->Pin = pin;
	GPIO_SetDir(gPIO, pin, GPIO_DIR_OUT);
}

Relay::~Relay() {
	// TODO Auto-generated destructor stub
}

void Relay::Init(LPC_GPIO_TypeDef* gPIO, GPIO_Pin_Typedef pin) {
	this->Port = gPIO;
	this->Pin = pin;
	GPIO_SetDir(gPIO, pin, GPIO_DIR_OUT);
}

void Relay::Open() {
	GPIO_SetHigh(this->Port, this->Pin);
	this->Status = RELAY_OPEN;
}

void Relay::Close() {
	GPIO_SetLow(this->Port, this->Pin);
	this->Status = RELAY_CLOSE;
}

Relay_Status Relay::GetState() {
	return this->Status;
}

//================ END OF FILE ==============================================//
