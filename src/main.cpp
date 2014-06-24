/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC12xx.h"
#endif

#include <cr_section_macros.h>
#include "lpc12xx_sysctrl.h"
#include "lpc_gpio.h"
#include "lpc_uart.h"
//#include "clock.h"
#include "relay.h"
#include "pvsensor.h"
#include "IAAP.h"
#include "MV.h"
#include "TempSensor.h"
#include "TTable.h"
//#include "MVKB.h"
//#include "ZBSTREAM.h"

struct {
	uint8_t Tick10ms;
	uint8_t Tick100ms;
	uint8_t Tick1sec;
	uint8_t Tick15sec;
	//uint8_t rsv;
}TickTime;
typedef struct {
	uint8_t ms1Tick;
	uint8_t ms10Tick;
	uint8_t ms100Tick;
	uint8_t secTick;
	uint8_t sec5Tick;
	uint8_t sec10Tick;
	uint8_t sec15Tick;
}ObjectTick_t;

ObjectTick_t ObjectTick;
//PVSensor Sensor;
TempSensor 		pTempSensor;
//Clock 	ckObject;
IAAP	pDcuPort;
IAAP 	pZigbeePort;
//ZBSTREAM pZigbeePort;
MV 		pMV;
//MVKB		pMV;
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void IAPReadRequestCallback(void* obj, uint8_t addr, PRIM_RD_TYPE_T* prim) {
	uint8_t dOut[64];
	//uint8_t type;
	IAAP* iaap;
	MV_DATA_T mvData;
	mvData.Data = dOut;
	iaap = (IAAP*) obj;
	pMV.ReadReq(prim, &mvData);
	iaap->RequestToSend(PRIM_READ_RSP, addr, mvData.Data,
			(uint8_t) mvData.len);
}
void IAPReadResponseCallback(void* obj, uint8_t addr, PRIM_RD_RSP_TYPE_T* prim) {
	//uint8_t dOut[64];
	//uint8_t type;
	//IAAP* iaap;
	//MV_DATA_T mvData;
	//mvData.Data = dOut;
	//iaap = (IAAP*) obj;
	pMV.ReadRsp(prim);
}
void IAPWriteRequestCallBack(void* obj, uint8_t addr, PRIM_WR_TYPE_T* prim) {
	uint8_t dOut[64];
	//uint8_t type;
	IAAP* iaap;
	MV_DATA_T mvData;
	mvData.Data = dOut;
	iaap = (IAAP*) obj;
	pMV.WriteReq(prim);
	iaap->RequestToSend(PRIM_WRITE_CNF, addr, mvData.Data, (uint8_t) mvData.len);
}
void IAPActionRequestCallBack(void* obj, uint8_t addr, PRIM_ACT_TYPE_T* prim) {
	uint8_t dOut[64];
	uint8_t type;
	IAAP* iaap;
	MV_DATA_T mvData;
	mvData.Data = dOut;
	iaap = (IAAP*) obj;
#ifndef USE_ZB
	if (iaap->GetPort() == UART1) {
#else
	if (iaap->GetPort() == UART0) {
#endif
		type = 0;
	} else {
		type = 1;
	}

	pMV.ActionReq(prim, &mvData);
	if (mvData.len != 0) {
	//if (type == 0) {
		pZigbeePort.RequestToSend(PRIM_ACTION_REQ, 1, mvData.Data,
						(uint8_t) mvData.len);
	}
	//else {
	iaap->RequestToSend(PRIM_ACTION_CNF, addr, 0, 0);
	//}
}
void MvAlarmCallback(void) {
	uint8_t dOut[8];
	Status result;
	MV_DATA_T data;
	data.Data = dOut;
	result = ERROR;
	//result = pMV.RequestResent(&data);
	if (result == SUCCESS) {
#ifdef USE_ZB
		pZigbeePort.RequestToSend(PRIM_READ_REQ, 0x01, data.Data, data.len);
#endif
		pMV.AlarmRefesh();
	}
	else {
		pMV.AlarmDisable();
	}
}
void UartIncommingCallback(Uart_type t, uint8_t data) {
#ifndef USE_ZB
	if (t == UART1) {
#else
	if (t == UART0) {
#endif

		pDcuPort.PhyCallback(t, data);
	}
#ifdef USE_ZB
	else {
		pZigbeePort.PhyCallback(t, data);
	}
#endif
}
void UartSendChcmpCallback(Uart_type t) {
#ifndef USE_ZB
	if (t == UART1) {
#else
	if (t == UART0) {
#endif

		pDcuPort.PhySentCallback(t);
	}
#ifdef USE_ZB
	else {
		pZigbeePort.PhySentCallback(t);
	}
#endif
}

void CoreInit(void) {
	//LPC_SYSCON->SYSOSCCTRL = 0x01;
	SystemCoreClockUpdate();
	// Config PIO 12 as MCLK Out
	LPC_IOCON->PIO0_12 &= ~0x07;
	LPC_IOCON->PIO0_12 |= 0x02;
	SYS_ConfigCLKOUTCLK(SYS_MAINCLKOUT, 100);
	//Select PLL clock source as System ocsillator
	//SYS_SelectSystemPLLCLKSource(SYS_SYSOSC);
	// Select main clock source as System oscillator
	// Operate as 24 Mhz
	//SYS_SelectMainCLKSource(SYS_SYSPLLCLKIN);
	SysTick_Config(SystemCoreClock / (1000 - 1));
	// ADC run as 1 MHz
	//ADC_Init(1000000UL);
	// Enable ADC interrupt
	//NVIC_EnableIRQ(ADC_IRQn);
}
void ObjectInit(void) {
	IAP_INIT_CB_T dIAPCb, zIAPCb;
	COM_PORT_T	dPort, zPort;
	// Init object
/*	Relay PhotoOV(LPC_GPIO0, GPIO_PIN_29);
	Relay CiCon(LPC_GPIO0, GPIO_PIN_28);
	Relay Lamp(LPC_GPIO0, GPIO_PIN_27);*/

	dIAPCb.RegReq = 0;
	dIAPCb.RegInd = 0;
	dIAPCb.ReadReq = IAPReadRequestCallback;
	dIAPCb.ReadRsp = 0;
	dIAPCb.WrReq = IAPWriteRequestCallBack;
	dIAPCb.ActReq = IAPActionRequestCallBack;
#ifndef USE_ZB
	dPort.Port = UART1;
#else
	dPort.Port = UART0;
#endif

	dPort.Speed = 19200;
	dPort.Loc = UART_LOC_0;

	pDcuPort.Init(0x01, &dPort, &dIAPCb);

#ifdef USE_ZB
	zIAPCb.RegReq = 0;
	zIAPCb.RegInd = 0;
	zIAPCb.ReadReq = 0;
	zIAPCb.ReadRsp = IAPReadResponseCallback;
	zIAPCb.WrReq = 0;
	zIAPCb.ActReq = 0;

	zPort.Port = UART1;
	zPort.Speed = 38400;
	zPort.Loc = UART_LOC_0;

	pZigbeePort.Init(0x00, &zPort, &zIAPCb);
	//pZigbeePort.Init(&zPort, ZigBeeDataStreamCallBack);
#endif


	pMV.Init(MvAlarmCallback);
	//pMV.Init(60);
	// Init uart for DCU
	UARTInit(dPort.Port, dPort.Speed, (uint8_t) dPort.Loc);
	UART_RS485Init();
	UARTRegSendCmp(dPort.Port, UartSendChcmpCallback);
	UARTRegDataCb(dPort.Port, UartIncommingCallback);
	// Init uart for Zigbee
#ifdef USE_ZB
	UARTInit(zPort.Port, zPort.Speed, (uint8_t) zPort.Loc);
	UARTRegSendCmp(UART1, UartSendChcmpCallback);
	UARTRegDataCb(UART1, UartIncommingCallback);
#endif
	// Register call for DCU
	// ADC Test
	//Sensor.Init();
	// Temperature initialized hardware
	pTempSensor.Init(tTable, 101);
	// Clock object
	/*DateTime_t dt = { { 0, 42, 13 }, { 17, 3, 2014 } };
	ckObject.Init(dt);*/

	GPIO_SetDir(LPC_GPIO0, GPIO_PIN_10, GPIO_DIR_OUT);
}
int main(void) {
    // TODO: insert code here
	uint8_t tempBuff[16];
	Status result;
	MV_DATA_T data;
	data.Data = tempBuff;
    // Force the counter to be placed into memory
    LPC_SYSCON->SYSAHBCLKCTRL |= 0xE001001FUL;
    CoreInit();
    ObjectInit();
    // Enter an infinite loop, just incrementing a counter
    while(1) {
		if (ObjectTick.ms1Tick) {
			// 1 ms routine
			pDcuPort.AlarmTick();
#ifdef USE_ZB
			pZigbeePort.AlarmTick();
#endif
			ObjectTick.ms1Tick = FALSE;
		}

		if (ObjectTick.ms10Tick) {
			// 10 ms routine
			pTempSensor.Read();
			ObjectTick.ms10Tick = FALSE;
		}

		if (ObjectTick.ms100Tick) {

			// 100 ms routine
			// Collect Data from lamp
			result = pMV.CollectData(&data);
			if (result == SUCCESS) {
#ifdef USE_ZB
				pZigbeePort.RequestToSend(PRIM_READ_REQ, 0x01, data.Data, data.len);
#endif
			}
			/*result = ERROR;
			// master pending register check
			result = pMV.MsRegisterPending() ;
			if (result == SUCCESS) {
				pMV.MakeRegisterIndicator(&data);
				pDcuPort.RequestToSend(PRIM_REGISTER_IND, 0x00, data.Data, data.len);
				GPIO_SetHigh(LPC_GPIO0, GPIO_PIN_10);
			}*/

			pMV.UpdateTemp(pTempSensor.GetTemp());
			ObjectTick.ms100Tick = FALSE;
		}

		if (ObjectTick.secTick) {
			// 1 sec routine
			//UARTSendCh(UART0, 'U');
			/*DateTime_t dt;
			ckObject.UpdateClock();
			ckObject.GetClock(&dt);
			pMV.UpdateDatetime(&dt);*/
			pMV.AlarmTick();
			ObjectTick.secTick = FALSE;
		}

		if (ObjectTick.sec5Tick) {
			// check for new device
			pMV.StartCollectNewDevice();
			ObjectTick.sec5Tick = FALSE;
		}

		if (ObjectTick.sec10Tick) {
			// collect data from coord
			pMV.StartCollectLightSensor();
			ObjectTick.sec10Tick = FALSE;
		}

		if (ObjectTick.sec15Tick) {
			pMV.StartCollectLQI();
			ObjectTick.sec15Tick = FALSE;
		}

		if (pDcuPort.PendingProcess()) {
			pDcuPort.Interactive();
		}
#ifdef USE_ZB
		if (pZigbeePort.PendingProcess()) {
			pZigbeePort.Interactive();
		}
#endif
    }
    return 0 ;
}

void SysTick_Routine(void) {
	// 1 ms Routine
		ObjectTick.ms1Tick = TRUE;
		if (++TickTime.Tick10ms >= 10) {
			// 10 ms Routine
			ObjectTick.ms10Tick = TRUE;
			if (++TickTime.Tick100ms >= 10) {
				// 100 ms Routing
				ObjectTick.ms100Tick = TRUE;
				if (++TickTime.Tick1sec >= 10) {
					// 1 sec Routing

					if (++TickTime.Tick15sec >= 15) {
						ObjectTick.sec15Tick = TRUE;
						TickTime.Tick15sec = 0;
					}

					if (TickTime.Tick15sec == 5) {
						ObjectTick.sec5Tick = TRUE;
					}

					if (TickTime.Tick15sec == 10) {
						ObjectTick.sec10Tick = TRUE;
					}

					ObjectTick.secTick = TRUE;

					TickTime.Tick1sec = 0;
				}
				TickTime.Tick100ms = 0;
			}
			TickTime.Tick10ms = 0;
		}
}

/*void RTC_IRQHandler(void)
{
	RTC_ClearIntPendingBit();
	//LPC_RTC->ICR = 0x01; /* Clear RTC interrupt /

	LPC_RTC->DR;
	LPC_RTC->MR = LPC_RTC->DR + 1;	/* Set match value /

	ckObject.UpdateClock();
}*/


void ADC_IRQHandler (void) {
	uint32_t reg;
	uint16_t adcVal;
	reg = LPC_ADC->GDR;		//Get Status
	reg = LPC_ADC->STAT;
	// Interrupt flag
	if(reg & ADC_ADINT) {
		// Channel 0
		adcVal = ADC_GetData(ADC_CHANNEL_2);
		TempSensor::ReadCallback(&pTempSensor, adcVal);
	}
}
