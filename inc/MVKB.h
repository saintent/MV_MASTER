//================ File Description =========================================//
//=== File name : MVKB.h
//===========================================================================//

#ifndef MVKB_H_
#define MVKB_H_

#include "MV.h"

//================ Include Header ===========================================//
//#include "stdint.h"
#include "string.h"
#include "lpc_types.h"
#include "lpc_gpio.h"
#include "iaap.h"
#include "clock.h"
#include "relay.h"
#include "MV.h"
#include "ZBSTREAM.h"
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

class MVKB: public MV {
public:
	MVKB();
	virtual ~MVKB();

	void AlarmTick(void);
	void Init(uint8_t updateExpire);
	void UpdateData(ZBStram_DataStruct_t* obj);
private :
	uint8_t dataIsTick[8];
	MV_ALARM_TYPE_T upDateExpire;
};

//================ END OF FILE ==============================================//
#endif /* MVKB_H_ */
