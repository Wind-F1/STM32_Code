#include "MTI_I2C_driver.h"
#include "stdio.h"

#define TIMEOUT_MS 100

extern I2C_HandleTypeDef hi2cMTI;
uint16_t m_deviceAddress = MTI_I2C_DEVICE_ADDRESS;

void Mtssp_I2C_writeRaw(uint8_t const* data, uint16_t dataLength)
{
	HAL_I2C_Master_Transmit(&hi2cMTI, (m_deviceAddress << 1), (uint8_t*)data, dataLength, TIMEOUT_MS);
}

void Mtssp_I2C_write(uint8_t opcode, uint8_t const* data, int dataLength)
{
	uint8_t transferBuffer[8];
	
	if (dataLength < sizeof(transferBuffer))
	{
		transferBuffer[0] = opcode;
		memcpy(&transferBuffer[1], data, dataLength);
		/*Use hardware I2C*/
		HAL_I2C_Master_Transmit(&hi2cMTI, (m_deviceAddress << 1), transferBuffer, 1 + dataLength, TIMEOUT_MS);
	}
}


void Mtssp_I2C_read(uint8_t opcode, uint8_t* dest, int dataLength)
{
	HAL_StatusTypeDef i2cstate;
	i2cstate = HAL_I2C_Mem_Read(&hi2cMTI, (m_deviceAddress << 1), opcode, 1, dest, dataLength, TIMEOUT_MS);
	//printf("i2cstate:%d\r\n", i2cstate);
}


