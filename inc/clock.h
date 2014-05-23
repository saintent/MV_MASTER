//================ File Despciption =========================================//
//=== File name : clock.h
//===========================================================================//

#ifndef CLOCK_H_
#define CLOCK_H_

//================ Include Header ===========================================//
#include "lpc12xx.h"
#include "stdint.h"
#include "lpc_types.h"
#include "lpc12xx_rtc.h"
//================ PULBIC DEFINE ============================================//
//
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct {
	uint8_t Sec;
	uint8_t Min;
	uint8_t Hour;
	uint8_t reserve;
}Time_t;

typedef struct {
	uint8_t Day;
	uint8_t Month;
	uint16_t Year;
	//uint8_t reserve;
}Date_t;

typedef struct {
	Time_t Time;
	Date_t Date;
}DateTime_t;
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

class Clock {
public:
	Clock();
	virtual ~Clock();

	void Init(DateTime_t DateTime);
	void UpdateClock(void);
	void GetClock(DateTime_t* dt);

private :
	DateTime_t dateTime;

};

//================ END OF FILE ==============================================//
#endif /* CLOCK_H_ */
