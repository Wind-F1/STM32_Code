#ifndef __I2CDRIVER_H__
#define __I2CDRIVER_H__
#include "main.h"

uint8_t IICWriteData(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IICReadData(uint8_t dev, uint8_t reg, uint8_t readLength, uint8_t *tempBuf);

#endif
