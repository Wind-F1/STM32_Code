#ifndef __SDSAVE_H__
#define __SDSAVE_H__
#include "fatfs.h"
#include "ADC.h"
//path to save curl data

void SD_init(void);
void Creat_File_in_SD( void);
void Write_Node_in_SD(struct EMGACCs_t *pstr);
void Close_File_in_SD(void);
void Delete_File_in_SD(void);

#endif



