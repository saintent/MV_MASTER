//================ File Desciption =========================================//
//=== File name : MVKB.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "MVKB.h"
#include "stdlib.h"
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

MVKB::MVKB() {
	// TODO Auto-generated constructor stub

}

MVKB::~MVKB() {
	// TODO Auto-generated destructor stub
}

void MVKB::Init(uint8_t updateExpire) {
	MV::Init(NULL);
	this->upDateExpire.Interval = updateExpire;
	this->upDateExpire.Enable = TRUE;
	this->upDateExpire.Count = 0;
}

void MVKB::AlarmTick(void) {
	uint8_t *pTick;
	uint8_t	i, j;
	uint8_t isTick;
	uint8_t lampLose;
	LAMP_TYPE_T *pLamp;
	MV::AlarmTick();
	if (++this->upDateExpire.Count >= this->upDateExpire.Interval) {
		// Check update
		pTick = this->dataIsTick;
		for (i = 0; i < 8; i++) {
			for(j = 0; j < 8; j++) {
				isTick = (pTick[0] >> j) & 1;
				if (!isTick) {
					// Clear lamp
					pLamp = &this->lamp[(i*8) + j];
					// If lamp is register. un-register
					if (pLamp->Serial[0] != 0xFF) {
						memset(pLamp, 0xFF, sizeof(LAMP_TYPE_T));
						//memset(pLamp->Serial, 0xFF, 4);
						this->lampCount--;
					}
				}
			}
			pTick++;
		}
		memset(this->dataIsTick, 0, 8);
		this->upDateExpire.Count = 0;
	}
	/*uint8_t count;
	pLamp = &this->lamp[0];
	for (count = 0; count < 30; count++) {
		pLamp->Lux = 20 + (rand() % 10);
		pLamp++;
	}*/
}

void MVKB::UpdateData(ZBStram_DataStruct_t* obj) {
	uint8_t loop;
	uint8_t cmpResult;
	uint8_t isLampRegistered;
	uint8_t lampId;
	uint8_t tickSlot;
	LAMP_TYPE_T* pLamp;
	// Search serial of lamp
	pLamp = this->lamp;
	isLampRegistered = FALSE;
	for (loop = 0; loop < 64; loop++) {
		cmpResult = memcmp(pLamp->Serial, obj->Serail, 4);
		// Match
		if(cmpResult == 0) {
			lampId = loop;
			isLampRegistered = TRUE;
			break;
		}
		pLamp++;
	}

	if(isLampRegistered == TRUE) {
		pLamp->Lux = obj->LightLevel;
		this->dataIsTick[lampId / 8] |= (1 << (lampId % 8));
	}
	else {
		// Fine available slot
		for(loop = 0; loop < 64; loop++) {
			if (this->lamp[loop].Id == 0xFFFF) {
				this->lamp[loop].Id = loop;
				memcpy(this->lamp[loop].Serial, obj->Serail, 4);
				this->lamp[loop].Lux = obj->LightLevel;
				tickSlot = loop/8;
				this->dataIsTick[tickSlot] |= (1 << (loop%8));
				this->lampCount++;
				break;
			}
		}
	}
}

//================ END OF FILE ==============================================//

