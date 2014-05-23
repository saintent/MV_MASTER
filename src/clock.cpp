//================ File Despciption =========================================//
//=== File name : clock.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "time.h"
#include "clock.h"
#include "lpc12xx_sysctrl.h"
#include "lpc12xx_rtc.h"
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
/*****************************************************************************
** Function name:		RTC_IRQHandler
**
** Descriptions:		Real Time Clock Interrupt Handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/

Clock::Clock() {
	// TODO Auto-generated constructor stub

}

Clock::~Clock() {
	// TODO Auto-generated destructor stub
}

void Clock::Init(DateTime_t DateTime) {
	struct tm timeInfo;
	time_t timeSource;
	// Enable RTC Clock
	SYS_ConfigAHBCLK(SYS_AHBCLKCTRL_RTC, ENABLE);

	// RTC init
	RTC_Init(RTC_CLKSRC_1HZ, 0);

	this->dateTime = DateTime;

	timeInfo.tm_min = this->dateTime.Time.Min;
	timeInfo.tm_hour = this->dateTime.Time.Hour;
	timeInfo.tm_sec = this->dateTime.Time.Sec;

	timeInfo.tm_mday = this->dateTime.Date.Day;
	timeInfo.tm_mon = this->dateTime.Date.Month;
	timeInfo.tm_year = this->dateTime.Date.Year - 1900;

	timeSource = mktime(&timeInfo);

	RTC_SetLoad(timeSource);
	RTC_SetMatch(timeSource + 1);
	RTC_IntConfig(ENABLE);
	NVIC_EnableIRQ(RTC_IRQn);
	RTC_Cmd(ENABLE);

}

void Clock::UpdateClock(void) {
	time_t data;
	struct tm * currenttime;

	data = (time_t)RTC_GetData();

	currenttime = localtime(&data);

	this->dateTime.Time.Min = currenttime->tm_min;
	this->dateTime.Time.Hour = currenttime->tm_hour;
	this->dateTime.Time.Sec = currenttime->tm_sec;

	this->dateTime.Date.Day = currenttime->tm_mday;
	this->dateTime.Date.Month = currenttime->tm_mon;
	this->dateTime.Date.Year = currenttime->tm_year + 1900;

}
void Clock::GetClock(DateTime_t * dt) {
	*dt = this->dateTime;
}



//================ END OF FILE ==============================================//
