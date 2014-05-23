//================ File Desciption =========================================//
//=== File name : MV.cpp
//===========================================================================//

//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "MV.h"
#include "string.h"
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

MV::MV() {
	// TODO Auto-generated constructor stub

}

MV::~MV() {
	// TODO Auto-generated destructor stub
}

PUBLIC void MV::Init(MVTimeoutCallback_t* tmCb) {
	MASTER_TYPE_T* ms;
	this->lampCount = 0;
	this->masterCount = 1;
	memset((void*) &this->lamp[0].Id, 0xFF, sizeof(LAMP_TYPE_T) * 64);
	memset((void*) &this->master[0].Id, 0xFF, sizeof(MASTER_TYPE_T) * 8);
	ms = &this->master[0];
	ms->Id = 0;
	ms->Serail[0] = 0x01;
	ms->Serail[1] = 0x00;
	ms->Serail[2] = 0x00;
	ms->Serail[3] = 0x04;
	this->alarm.Count = 0;
	this->alarm.Enable = FALSE;
	this->alarm.Interval = 3;
	this->resent = 0;
	this->readState = MV_READ_IDLE;
	this->currentLampRead = 0;
	this->timeoutCallback = tmCb;
	this->pendingRegCnf = 1;
	this->photoOV.Init(LPC_GPIO0, GPIO_PIN_29);
	this->ciOn.Init(LPC_GPIO0, GPIO_PIN_28);
	this->photoOV.Close();
	this->ciOn.Close();

#ifdef SIM_LAM
	// Register Lamp
	this->lampCount = 30;
	uint8_t count = 0;
	uint8_t l1[8] = {0, 0, 0, 0, 1, 0, 0, 0};
	//uint8_t l2[8] = {1, 0, 75, 0, 0, 0, 0, 1};
	//uint8_t l3[8] = {2, 0, 85, 0, 0, 0, 0, 2};
	//uint8_t l4[8] = {3, 0, 82, 0, 0, 0, 0, 3};
	//uint8_t l5[8] = {4, 0, 78, 0, 0, 0, 0, 4};
	for (count = 0; count < 30; count++) {
		l1[7] = count + 1;
		l1[20] = 20 + (rand() % 10);
		memcpy(&this->lamp[count], l1, 8);
	}
#endif
	for (int i = 0; i < 64; i++) {
		this->lamp[i].Lux = 0;
	}
}
PUBLIC Status MV::UpdateDatetime(DateTime_t* dt) {
	this-> dateTime = *dt;
	return SUCCESS;
}
PUBLIC Status MV::RegisterReq(uint8_t id, uint8_t type, MV_DATA_T* out) {
	// Fine serial by id
	Status result;
	uint8_t loop;
	result = ERROR;
	//  Master
	if (type == 0) {
		for (loop = 0; loop < this->masterCount; loop++) {
			if (this->master[loop].Id != 0xFF) {
				out->Data[0] = id;
				out->Data[1] = type;
				memcpy(&out->Data[2], this->master[loop].Serail, 4);
				out->len = 6;
				// if register process
				if (this->pendingRegCnf > 0) {
					MSRegeisterCnf();
				}
				result = SUCCESS;
				break;
			}
		}
	}
	// Lamp
	else {
		for (loop = 0; loop < lampCount; loop++) {
			if (this->lamp[loop].Id != 0xFF) {
				out->Data[0] = id;
				out->Data[1] = type;
				memcpy(&out->Data[2], this->lamp[loop].Serial, 4);
				out->len = 6;
				result = SUCCESS;
				break;
			}
		}
	}
	return result;
}
PUBLIC Status MV::RegisterInd(PRIM_REG_TYPR_T* prim) {
	Status result;
	uint8_t loop;
	uint8_t nofCount;
	uint8_t cach;
	uint8_t *serPtr;
	result = ERROR;
	nofCount = prim->NumberOfNof;
	serPtr = &prim->Entry;
	// Type is Master
	if (prim->Type == 0) {
		cach = 0;
		while (nofCount) {
			for (loop = cach; loop < 8; loop++) {
				if (this->master[loop].Id != 0xFF) {
					memcpy(this->master[loop].Serail, serPtr, 4);
					serPtr += 4;
					cach = loop;
					if (cach >= this->masterCount) {
						this->masterCount++;
					}
					result = SUCCESS;
					break;
				}
			}
			nofCount--;
		}
	}
	// Type is Lamp
	else {
		cach = 0;
		while (nofCount) {
			for (loop = cach; loop < 64; loop++) {
				if (this->lamp[loop].Id != 0xFF) {
					this->lamp[loop].Id = prim->Entry;
					//memcpy(this->lamp[loop].Serial, serPtr, 4);
					//serPtr += 4;
					this->lastRegLampId = prim->Entry;
					cach = loop;
					if (cach >= this->lampCount) {
						this->lampCount++;
					}
					result = SUCCESS;
					break;
				}
			}
			nofCount--;
		}
	}
	return result;
}
PUBLIC Status MV::MakeRegisterIndicator(MV_DATA_T* out) {
	out->Data[0] = this->pendingRegCnf;
	out->Data[1] = 0;
	out->Data[2] = this->master[0].Id;
	out->len = 3;
	return SUCCESS;
}
PUBLIC Status MV::ReadReq(PRIM_RD_TYPE_T* prim, MV_DATA_T* out) {
	uint8_t i;
	uint8_t pos;
	uint8_t loop;
	uint8_t lampId;
	Status result;
	LAMP_TYPE_T* lamp;
	result = ERROR;
	MASTER_TYPE_T* _master;
	for (loop = 0; loop < 8; loop++) {
		if (this->master[loop].Id == prim->Id) {
			_master = &this->master[loop];
			result = SUCCESS;
			break;
		}
	}
	out->Data[0] = prim->Id;
	out->Data[1] = prim->Type;
	pos = 2;
	switch (prim->Type) {
	case MS_LAMP_BLOCK_0:
		for (i = 0; i < 8; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_1:
		for (i = 8; i < 16; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_2:
		for (i = 16; i < 24; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_3:
		for (i = 24; i < 32; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_4:
		for (i = 32; i < 40; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_5:
		for (i = 40; i < 48; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_6:
		for (i = 48; i < 56; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_LAMP_BLOCK_7:
		for (i = 56; i < 64; i++) {
			out->Data[pos] = (uint8_t) this->lamp[i].Lux;
			pos++;
		}
		out->len = 10;
		break;
	case MS_ENERGY:
		memcpy(&out->Data[2], _master->Volt, 4);
		memcpy(&out->Data[6], _master->Current, 4);
		memcpy(&out->Data[10], _master->Power, 4);
		memcpy(&out->Data[14], _master->Energy, 4);
		memcpy(&out->Data[18], _master->PowerFactor, 4);
		out->len = 22;
		break;
	case MS_TIME:
		out->Data[2] = _master->DateTime.Time.Sec;
		out->Data[3] = _master->DateTime.Time.Min;
		out->Data[4] = _master->DateTime.Time.Hour;
		out->Data[5] = _master->DateTime.Date.Day;
		out->Data[6] = _master->DateTime.Date.Month;
		out->Data[7] = _master->DateTime.Date.Year;
		out->len = sizeof(DateTime_t) + 2;
		break;
	case MS_RELAY :
		if (photoOV.GetState() == RELAY_CLOSE) {
			out->Data[2] = RELAY_CLOSE;
		}
		else {
			out->Data[2] = RELAY_OPEN;
		}

		if (ciOn.GetState() == RELAY_CLOSE) {
			out->Data[3] = RELAY_CLOSE;
		}
		else {
			out->Data[3] = RELAY_OPEN;
		}
		out->len = 4;
		break;
	case MS_SERAIL:
		memcpy(&out->Data[2], _master->Serail, 4);
		out->len = 6;
		break;
	case MS_NOL:
		out->Data[2] = this->lampCount;
		out->len = 3;
		break;
	case MS_LAMP_ATTR:
		lampId = prim->Data;
		if (lampSearch(lampId, &lamp)) {
			out->Data[2] = (uint8_t)lamp->Id;
			out->Data[3] = (uint8_t)lamp->Lux;
			memcpy(&out->Data[4], lamp->Serial, 12);
			//out->Data[3] = (uint8_t)lamp->Lux;
			out->len = 14 + 2;
		} else {
			out->Data[2] = 0xFF;
			out->len = 3;
		}
		break;
	default:
		result = ERROR;
		break;
	}
	return result;
}
PUBLIC Status MV::ReadRsp(PRIM_RD_RSP_TYPE_T* prim) {
	// Response form lamp
	uint8_t i;
	uint8_t* data;
	//Status result;
	LAMP_TYPE_T* lamp;
	// Find lamp id
	//lampSearch(prim->Id, &lamp);
	for (i = 0; i < 64; i++) {
		if (this->lamp[i].Id == prim->Id) {
			lamp = &this->lamp[i];
			break;
		}
	}
	data = &prim->Data;
	switch (prim->Type) {
	case LAMP_ID:
		// Not use now
		break;
	case LAMP_SERIAL:
		memcpy(lamp->Serial, data, 12);
		break;
	case LAMP_SENSOR:
		lamp->Lux = prim->Data;
		break;
	case LAMP_ATTR_PENDING :
		if (prim->Data != 0xFF) {
			// getting Data
			LAMP_TYPE_T *pLamp;
			pLamp = &this->lamp[prim->Id];
			if (pLamp->Id != prim->Id) {
				this->lampCount++;
			}
			pLamp->Id = prim->Id;
			memcpy(pLamp->Serial, &prim->Data, 12);

			// Getting next lamp

		}
		else if (prim->Data == 0xFF){
			// Stop check process
			this->currentProcess = MV_IDLE_PROC;

		}
		break;
	//case LAMP_LOC:
		//memcpy((uint8_t*) lamp->Loc.Lat, data, 8);
	//	break;
	}
	// Reset State
	AlarmDisable();
	this->resent = 0;
	this->readState = MV_READ_IDLE;
	return SUCCESS;
}
PUBLIC Status MV::WriteReq(PRIM_WR_TYPE_T* prim) {
	Status result;
	Date_t* date;
	Time_t* time;
	result = SUCCESS;
	if (prim->Id == 0) {
		time = (Time_t*) &prim->Data;
		date = (Date_t*) &time->reserve;
		this->master[0].DateTime.Date = *date;
		this->master[0].DateTime.Time = *time;
	}
	return SUCCESS;
}
PUBLIC Status MV::ActionReq(PRIM_ACT_TYPE_T* prim, MV_DATA_T* out) {
	Status result;
	MASTER_TYPE_T* _master;
	uint8_t* relayOut;
	result = ERROR;
	result = masterSearch(prim->Id, &_master);
	out->len = 0;
	if (result == SUCCESS) {
		switch (prim->Type) {
		case MS_ACT_CI_CON:
			if (prim->Data == 1) {
				// NO Close
				photoOV.Close();
				// NC Open
				ciOn.Open();
			} else {
				// NO Open
				photoOV.Open();
				// NC close
				ciOn.Close();
			}
			break;
		case MS_ACT_R_CON:
			relayOut = &prim->Data;
			if (relayOut[0] == 0) {
				photoOV.Close();
			} else {
				photoOV.Open();
			}
			if (relayOut[1] == 0) {
				ciOn.Close();
			} else {
				ciOn.Open();
			}
			break;
		default:
			result = ERROR;
			break;
		}
	}
	return result;
}
/*PUBLIC Status MV::RequestToReadLamp(uint8_t id, LAMP_READ_TYPE_T type,
		MV_DATA_T* out) {
	uint8_t i;
	Status result;
	result = ERROR;
	// Check id
	for (i = 0; i < 64; i++) {
		if (this->lamp[i].Id == id) {
			out->Data[0] = id;
			out->Data[1] = type;
			out->len = 2;
			result = SUCCESS;
			this->lastIdRead = id;
			this->lastTypeRead = type;
			this->resent = 0;
			this->readState = MV_READING;
			break;
		}
	}
	return result;
}*/

PUBLIC Status MV::RequestToGetNewDevice(MV_DATA_T* out) {
	out->Data[0] = 0;
	out->Data[1] = LAMP_ATTR_PENDING;
	out->len = 2;
}



PUBLIC Status MV::RequestResent(MV_DATA_T* out) {
	Status result;
	result = ERROR;
	if (++this->resent <= 3) {
		out->Data[0] = this->lastIdRead;
		out->Data[1] = this->lastTypeRead;
		out->len = 2;
		result = SUCCESS;
	}
	return result;
}
PUBLIC void MV::AlarmTick(void) {
	//FIFO_ATTR_T* fifo;
	if (this->alarm.Enable) {
		// Check Time out
		if (++this->alarm.Count >= this->alarm.Interval) {
			// Process

			if (this->timeoutCallback != NULL ) {
				timeoutCallback();
			}

			this->alarm.Count = 0;
			this->alarm.Enable = FALSE;
		}
	}
	/*if (ciOn.GetState()) {
		ciOn.Close();
		photoOV.Close();
	}
	else {
		ciOn.Open();
		photoOV.Open();
	}*/
}
PUBLIC void MV::AlarmRefesh(void) {
	this->alarm.Count = 0;
}
PUBLIC void MV::AlarmEnable(void) {
	if (!this->alarm.Enable) {
		this->alarm.Enable = TRUE;
		this->alarm.Count = 0;
	}
}
PUBLIC void MV::AlarmDisable(void) {
	this->alarm.Enable = FALSE;
}
PUBLIC Status MV::CollectData(MV_DATA_T* out) {
	Status result;
	result = ERROR;
	if (this->currentProcess == MV_GET_DEVICE) {
		result = CheckNewDevice(out);
	}
	else if (this->currentProcess == MV_COLLECT_LIGHT_PROC) {
		//if (this->readState == MV_READ_IDLE) {
			// Getting data from next lamp
			// if had lamp in circuit
			result = ReadNextLight(out);
			/*if (this->lampCount != 0) {
				RequestToReadLamp(this->currentLampRead, LAMP_SENSOR, out);
				if (++this->currentLampRead >= this->lampCount) {
					this->currentLampRead = 0;
				}

				this->readState = MV_READING;
				AlarmEnable();
				result = SUCCESS;
			}*/
			//AlarmEnable();


		//}
	}
	else if (this->currentProcess == MV_COLLECT_LIGHT_PROC) {

	}

	return result;
}

PUBLIC Status MV::MsRegisterPending(void) {
	Status result;
	result = ERROR;
	if (this->pendingRegCnf >= 1) {
		result = SUCCESS;
	}
	return result;
}
PUBLIC Status MV::MSRegeisterCnf(void) {
	this->pendingRegCnf--;
	return SUCCESS;
}

PUBLIC Status MV::CheckNewDevice(MV_DATA_T *out) {
	this->currentProcess = MV_GET_DEVICE;
	out->Data[0] = 0;
	out->Data[1] = LAMP_ATTR_PENDING;
	out->len = 2;
	return SUCCESS;
}

PUBLIC Status MV::StartCollectNewDevice(void) {
	this->currentProcess = MV_GET_DEVICE;
	return SUCCESS;
}

PUBLIC Status MV::StartCollectLightSensor(void) {
	this->currentProcess = MV_COLLECT_LIGHT_PROC;
	this->readCount = 0;
	return SUCCESS;
}

PUBLIC Status MV::ReadNextLight(MV_DATA_T *out) {
	Status result;
	uint8_t currentPos;
	uint8_t setPoint;
	uint8_t getDev;
	LAMP_TYPE_T *pLamp;
	result = ERROR;
	currentPos = this->currentLampRead;
	getDev = FALSE;
	setPoint = 0;
	while (!getDev) {
		if (++currentPos > MAX_LAMP) {
			currentPos = 0;
		}
		pLamp = &this->lamp[currentPos];
		if (pLamp->Id != 0xFFFF) {
			this->currentLampRead = currentPos;
			getDev = TRUE;
			result = SUCCESS;
		}

		if (++setPoint > MAX_LAMP) {
			// No register lamp
			this->currentProcess = MV_IDLE_PROC;
			break;
		}

	}
	if (++this->readCount >= this->lampCount) {
		this->currentProcess = MV_IDLE_PROC;
	}
	out->Data[0] = pLamp->Id;
	out->Data[1] = LAMP_SENSOR;
	out->len = 2;

	this->readState = MV_READING;

	return result;

}

PRIVATE Status MV::lampSearch(uint8_t id, LAMP_TYPE_T** ppLamp) {
	Status st;
	uint8_t i;
	st = ERROR;
	// Find lamp id
	for (i = 0; i < 64; i++) {
		if (this->lamp[i].Id == id) {
			*ppLamp = &this->lamp[i];
			st = SUCCESS;
			break;
		}
	}
	return st;
}
PRIVATE Status MV::masterSearch(uint8_t id, MASTER_TYPE_T** ppMaster) {
	Status st;
	uint8_t i;
	st = ERROR;
	// Find lamp id
	for (i = 0; i < 8; i++) {
		if (this->master[i].Id == id) {
			*ppMaster = &this->master[i];
			st = SUCCESS;
			break;
		}
	}
	return st;
}


//================ END OF FILE ==============================================//

