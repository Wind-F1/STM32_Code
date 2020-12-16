#include "MTI_interface.h"
#include "xbusdef.h"
#include "MTI_application.h"
#include "stdio.h"
#include "xbusdef.h"
#include "xbushelpers.h"
#include "xbusmessageid.h"
#include "xbustostring.h"
#include "main.h"

#define MTI_Ready() (HAL_GPIO_ReadPin(MTI_DRDY_GPIO_Port, MTI_DRDY_Pin)==GPIO_PIN_SET)
uint8_t m_dataBuffer[256];

int MTI_app_readDataFromDevice(void);
int MTI_app_handleEvent(enum Event event, uint8_t* data);
void MTI_app_resetDevice(void);

int MTI_init()
{
	int flagMTI = 0;
	MTI_app_handleEvent(EVT_Start, NULL);
	while (flagMTI != 1)
	{
		flagMTI = MTI_app_run();
	}

	return 1;
}

int MTI_app_run()
{
	int isData = 0;
	if (MTI_Ready())
	{
		isData = MTI_app_readDataFromDevice();
	}
	return isData;
	
}


int MTI_app_readDataFromDevice()
{
	int state_MTI;
	uint16_t notificationMessageSize;
	uint16_t measurementMessageSize;
	
	Mtssp_interface_readPipeStatus(&notificationMessageSize, &measurementMessageSize);
	
	m_dataBuffer[0] = XBUS_PREAMBLE;
	m_dataBuffer[1] = XBUS_MASTERDEVICE;
	
	if (notificationMessageSize && notificationMessageSize < sizeof(m_dataBuffer))
	{
		//#ifdef saveToString
		Mtssp_interface_readFromPipe(&m_dataBuffer[2], notificationMessageSize, XBUS_NOTIFICATION_PIPE);
		state_MTI = MTI_app_handleEvent(EVT_XbusMessage, m_dataBuffer);
		//#endif
	}

	if (measurementMessageSize && measurementMessageSize < sizeof(m_dataBuffer))
	{
		Mtssp_interface_readFromPipe(&m_dataBuffer[2], measurementMessageSize, XBUS_MEASUREMENT_PIPE);
		state_MTI = MTI_app_handleEvent(EVT_XbusMessage, m_dataBuffer);
	}
	return state_MTI;
}


int MTI_app_handleEvent(enum Event event, uint8_t* data)
{
	static enum MTIState m_state = STATE_Idle;
	struct XbusMessage_t msg = {.m_length = 0, .m_data = NULL};
	int isData = 0;
	
	switch (m_state)
	{
		case STATE_Idle:
		{
			if (event == EVT_Start)
			{
				#ifdef saveToString
				printf("Resetting the device\r\n");
				#endif
				MTI_app_resetDevice();
				m_state = STATE_WaitForWakeUp;
			}
		} break;

		case STATE_WaitForWakeUp:
		{
			if (event == EVT_XbusMessage && getMessageId(data) == XMID_Wakeup)
			{
				#ifdef saveToString
				printf("Got Wake-up from device\r\n");
				#endif
				msg.m_mid = XMID_GotoConfig;
				//msg.m_mid = XMID_WakeupAck;
				Mtssp_interface_sendXbusMessage(&msg);
				m_state = STATE_WaitForConfigMode;
			}
		} break;

		case STATE_WaitForConfigMode:
		{
			msg.m_mid = XMID_ReqDid;
			Mtssp_interface_sendXbusMessage(&msg);
			m_state = STATE_WaitForDeviceId;
		} break;

		case STATE_WaitForDeviceId:
		{
			if (event == EVT_XbusMessage && getMessageId(data) == XMID_DeviceId)
			{
				#ifdef saveToString
				printf("Got DeviceId\r\n");
				#endif
				msg.m_mid = XMID_ReqFirmwareRevision;
				Mtssp_interface_sendXbusMessage(&msg);
				m_state = STATE_WaitForFirmwareRevision;
			}
		} break;

		case STATE_WaitForFirmwareRevision:
		{
			if (event == EVT_XbusMessage && getMessageId(data) == XMID_FirmwareRevision)
			{
				#ifdef saveToString
				printf("Got firmware revision\r\n");
				#endif
				uint8_t xbusData[] = {0x20, 0x30, 0x00, 0x64, 0x80, 0x20, 0x00, 0x64};	// (Output mode: Euler angles (0x2030) at 10 Hz)
				msg.m_mid = XMID_SetOutputConfig;
				msg.m_length = sizeof(xbusData);
				msg.m_data = xbusData;
				Mtssp_interface_sendXbusMessage(&msg);
				m_state = STATE_WaitForSetOutputConfigurationAck;
			}
		} break;

		case STATE_WaitForSetOutputConfigurationAck:
		{
			if (event == EVT_XbusMessage && getMessageId(data) == XMID_OutputConfig)
			{
				#ifdef saveToString
				printf("Output configuration written to device\r\n");
				#endif
				msg.m_mid = XMID_GotoMeasurement;
				Mtssp_interface_sendXbusMessage(&msg);
				//printHelpText();
				m_state = STATE_Ready;
			}
		} break;

		case STATE_Ready:
		{
//			if (event == EVT_GotoConfig)
//			{
//				msg.m_mid = XMID_GotoConfig;
//				Mtssp_interface_sendXbusMessage(&msg);
//			}

//			if (event == EVT_GotoMeasuring)
//			{
//				msg.m_mid = XMID_GotoMeasurement;
//				Mtssp_interface_sendXbusMessage(&msg);
//			}

//			if (event == EVT_RequestDeviceId)
//			{
//				msg.m_mid = XMID_ReqDid;
//				Mtssp_interface_sendXbusMessage(&msg);
//			}
			if (event == EVT_XbusMessage)
			{
				
				#ifdef saveTobuffer
				isData = xbusTobuffer(data);
				#endif
				
				
				#ifdef saveToString
				const char* s = xbusToString(data);
				printf("%s\r\n", s);
				#endif
				//return isData;
			}
		} break;
	}
	return isData;
}

void MTI_app_resetDevice()
{
	HAL_GPIO_WritePin(MTI_Reset_GPIO_Port, MTI_Reset_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(MTI_Reset_GPIO_Port, MTI_Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(MTI_Reset_GPIO_Port, MTI_Reset_Pin, GPIO_PIN_SET);
}


