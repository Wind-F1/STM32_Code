#include "MTI_interface.h"
#include "MTI_I2C_driver.h"
#include "xbusdef.h"

uint16_t XbusMessage_createRawMessage(uint8_t* dest, struct XbusMessage_t * message);

void Mtssp_interface_readProtocolInfo(uint8_t * version, uint8_t * dataReadyConfig)
{
	uint8_t buffer[2];
	Mtssp_I2C_read(XBUS_PROTOCOL_INFO, buffer, 2);
	*version = buffer[0];
	*dataReadyConfig = buffer[1];
}


void Mtssp_interface_configProtocol(uint8_t * dataReadyConfig, int size)
{
	Mtssp_I2C_write(XBUS_CONFIGURE_PROTOCOL, dataReadyConfig, size);
}


void Mtssp_interface_readPipeStatus(uint16_t* notificationMessageSize, uint16_t* measurementMessageSize)
{
	uint8_t status[4];
	Mtssp_I2C_read(XBUS_PIPE_STATUS, status, sizeof(status));
	*notificationMessageSize = status[0] | (status[1]<<8);
	*measurementMessageSize = status[2] | (status[3]<<8);	
}

void Mtssp_interface_readFromPipe(uint8_t* buffer, uint16_t size, uint8_t pipe)
{
	if (pipe == XBUS_NOTIFICATION_PIPE || pipe == XBUS_MEASUREMENT_PIPE)
	{
		Mtssp_I2C_read(pipe, buffer, size);
	}
}

void Mtssp_interface_sendXbusMessage(struct XbusMessage_t * xbusMessage)
{
	uint8_t buffer[128];
	uint16_t rawLength = XbusMessage_createRawMessage(buffer, xbusMessage);
	Mtssp_I2C_writeRaw(buffer, rawLength);
}

uint16_t XbusMessage_createRawMessage(uint8_t* dest, struct XbusMessage_t * message)
{
	uint8_t checksum;
	uint16_t length;
	uint8_t* dptr = dest;
	
	if (dest == 0)
	{
		return (message->m_length < 255) ? message->m_length+4 : message->m_length+6;
	}
	
	*dptr ++= XBUS_CONTROL_PIPE;
	
	checksum = 0;
	checksum -= XBUS_MASTERDEVICE;
	
	*dptr = message->m_mid;
	checksum -= *dptr++;
	length = message->m_length;
	
	if (length<XBUS_EXTENDED_LENGTH)
	{
		*dptr = length;
		checksum -= *dptr++;
	}
	else
	{
		*dptr = XBUS_EXTENDED_LENGTH;
		checksum -= *dptr++;
		*dptr = length >> 8;
		checksum -= *dptr++;
		*dptr = length & 0xFF;
		checksum -= *dptr++;
	}
	
	for (int n = 0; n < message->m_length; n++)
	{
		*dptr = message->m_data[n];
		checksum -= *dptr++;
	}
	
	*dptr++ = checksum;

	return dptr - dest;
}

