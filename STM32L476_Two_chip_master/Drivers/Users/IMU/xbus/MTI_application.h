#ifndef __MTI_APPLICATION_H__
#define __MTI_APPLICATION_H__
#include "stdint.h"

#define saveTobuffer
#define saveToString

enum Event
{
	EVT_Start,
	EVT_GotoConfig,
	EVT_GotoMeasuring,
	EVT_Reset,
	EVT_XbusMessage,
	EVT_RequestDeviceId,
};

enum MTIState
{
	STATE_Idle,
	STATE_WaitForWakeUp,
	STATE_WaitForConfigMode,
	STATE_WaitForDeviceId,
	STATE_WaitForFirmwareRevision,
	STATE_WaitForSetOutputConfigurationAck,
	STATE_Ready,
};

int MTI_init(void);
int MTI_app_run(void);
#endif
