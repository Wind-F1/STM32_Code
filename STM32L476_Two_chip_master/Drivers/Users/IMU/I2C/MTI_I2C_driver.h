#ifndef __MTI_I2C_DRIVER_H__
#define __MTI_I2C_DRIVER_H__
//#include "I2C_soft_driver.h"
#include "string.h"
#include "main.h"
#include "xbusdef.h"

void Mtssp_I2C_write(uint8_t opcode, uint8_t const* data, int dataLength);
void Mtssp_I2C_read(uint8_t opcode, uint8_t* dest, int dataLength);
void Mtssp_I2C_writeRaw(uint8_t const* data, uint16_t dataLength);
#endif
