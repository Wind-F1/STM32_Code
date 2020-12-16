#ifndef __MTI_INTERFACE_H__
#define __MTI_INTERFACE_H__
#include "stdint.h"

struct XbusMessage_t
{
	uint8_t m_mid;
	uint8_t m_length;
	uint8_t* m_data;
};

void Mtssp_interface_readProtocolInfo(uint8_t * version, uint8_t * dataReadyConfig);
void Mtssp_interface_configProtocol(uint8_t * dataReadyConfig, int size);
void Mtssp_interface_readPipeStatus(uint16_t* notificationMessageSize, uint16_t* measurementMessageSize);
void Mtssp_interface_readFromPipe(uint8_t* buffer, uint16_t size, uint8_t pipe);
void Mtssp_interface_sendXbusMessage(struct XbusMessage_t* xbusMessage);

#endif
