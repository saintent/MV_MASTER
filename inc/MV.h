//================ File Description =========================================//
//=== File name : MV.h
//===========================================================================//

#ifndef MV_H_
#define MV_H_

//================ Include Header ===========================================//
#include "stdint.h"
#include "string.h"
#include "lpc_types.h"
#include "lpc_gpio.h"
#include "iaap.h"
#include "clock.h"
#include "relay.h"
//================ PULBIC DEFINE ============================================//
#define MAX_LAMP	64
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum {
	MS_LAMP_BLOCK_0 = 0,
	MS_LAMP_BLOCK_1,
	MS_LAMP_BLOCK_2,
	MS_LAMP_BLOCK_3,
	MS_LAMP_BLOCK_4,
	MS_LAMP_BLOCK_5,
	MS_LAMP_BLOCK_6,
	MS_LAMP_BLOCK_7,
	//MS_ENERGY = 16,
	MS_TIME = 32,
	MS_RELAY,
	MS_TEMP,
	MS_SERAIL = 48,
	MS_NOL,
	MS_LAMP_ATTR
} MS_READ_TYPE_T;

typedef enum {
	MV_COLLECT_LIGHT_PROC,
	MV_COLLECT_LQI_PROC,
	MV_GET_LAMP_COUNT,
	MV_GET_DEVICE,
	MV_IDLE_PROC
}MV_PROC_T;

typedef enum {
	MS_ACT_CI_CON = 0,
	MS_ACT_R_CON,
	MV_ACT_LIGHT_CONTROL
} MS_ACT_TYPT_Y;

typedef enum {
	LAMP_ID = 0,
	LAMP_SERIAL,
	LAMP_SENSOR,
	LAMP_LQI,
	LAMP_ADDR,
	LAMP_NOL,
	LAMP_ATTR_PENDING
} LAMP_READ_TYPE_T;

typedef enum {
	LAMP_ACT_SWITCH = 0,
	LMAP_ACT_DIM
}LAMP_ACT_TYPE_T;

typedef enum {
	MV_READ_IDLE = 0, MV_READING, MV_RETIRING, MV_READ_DONE
} MV_READ_STATE_TYPT_T;

//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
typedef void MVTimeoutCallback_t(void);
//================ TYPEDEF STRUCT/UNION =====================================//

/*
 * SIZE = 16 byte
 */
typedef struct {
	uint16_t Id;
	uint16_t Lux;
	uint8_t LQI;
	uint8_t Serial[12];
} LAMP_TYPE_T;

typedef struct {
	uint32_t Id;
	uint8_t Serail[4];
	uint8_t Temperature;
	//uint8_t Volt[4];
	//uint8_t Current[4];
	//uint8_t Power[4];
	//uint8_t Energy[4];
	//uint8_t PowerFactor[4];
	DateTime_t DateTime;
} MASTER_TYPE_T;

/*
 *
 */
typedef struct {
	uint8_t* Data;
	uint32_t len;
} MV_DATA_T;

typedef struct {
	uint8_t Enable;
	uint8_t Count;
	uint8_t Interval;
	uint8_t rsv;
} MV_ALARM_TYPE_T;
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

class MV {
public:
	MV();
	virtual ~MV();

	virtual void Init(MVTimeoutCallback_t* tmCb);
	virtual void AlarmTick(void);
	void AlarmRefesh(void);
	void AlarmEnable(void);
	void AlarmDisable(void);

	Status UpdateDatetime(DateTime_t* dt);
	//Status RegisterReq(uint8_t id, uint8_t type, MV_DATA_T* out);
	//Status RegisterInd(PRIM_REG_TYPR_T* prim);
	//Status MakeRegisterIndicator(MV_DATA_T* out);
	Status ReadReq(PRIM_RD_TYPE_T* prim, MV_DATA_T* out);
	Status ReadRsp(PRIM_RD_RSP_TYPE_T* prim);
	Status WriteReq(PRIM_WR_TYPE_T* prim);
	Status ActionReq(PRIM_ACT_TYPE_T* prim, MV_DATA_T* out);
	//Status RequestToReadLamp(uint8_t id, LAMP_READ_TYPE_T type, MV_DATA_T* out);
	//Status RequestResent(MV_DATA_T* out);
	Status CollectData(MV_DATA_T* out);
	Status MsRegisterPending(void);
	Status MSRegeisterCnf(void);
	Status CheckNewDevice(MV_DATA_T* out);
	Status ReadNextLight(MV_DATA_T* out);
	Status RequestToGetNewDevice(MV_DATA_T* out);
	Status StartCollectNewDevice(void);
	Status StartCollectLightSensor(void);
	Status UpdateTemp(uint8_t temp);


private:
	// Attribute for MV
	//uint8_t masterCount;
	MASTER_TYPE_T master;
	uint8_t	readCount;
	uint8_t currentLampRead;
	//uint8_t lastRegLampId;
	//uint8_t lastIdRead;
	//uint8_t lastTypeRead;
	uint8_t resent;
	uint8_t pendingRegCnf;


	MV_READ_STATE_TYPT_T readState;
	MV_PROC_T currentProcess;
	Relay photoOV;
	Relay ciOn;
	Relay uility;
	DateTime_t dateTime;

protected:
	MV_ALARM_TYPE_T alarm;
	uint8_t lampCount;
	LAMP_TYPE_T lamp[64];
	MVTimeoutCallback_t* timeoutCallback;
	Status lampSearch(uint8_t id, LAMP_TYPE_T** ppLamp);
	//Status masterSearch(uint8_t id, MASTER_TYPE_T** ppMaster);

};

//================ END OF FILE ==============================================//
#endif /* MV_H_ */
