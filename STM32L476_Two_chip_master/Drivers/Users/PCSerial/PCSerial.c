
/*
 *!
 * \file       :PCSerial.c
 * \brief      : 
 * \version    : 
 * \date       : 2020/03/10
 * \author     : notrynohigh
 *Last modified by notrynohigh 2020/03/10
 *Copyright (c) 2020 by NOTRYNOHIGH. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include <string.h>
#include "stm32l4xx_hal.h"
#include "PCSerial.h"
#include "main.h"
#include <stdio.h>


//test=-1 Debug
//test=0 Biceps Curl
//test=1 test filter


/* \defgroup PCSerial_Private_TypesDefinitions
 * \{
 */
extern UART_HandleTypeDef huartPC;

const uint8_t OutCHs[] = OutputCH;

#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};


FILE __stdout;

void _sys_exit(int x)
{
	x=x;
}

int fputc(int ch, FILE *f)
{
	while((USART1->ISR & 0x40) == 0);
	USART1 -> TDR = (uint8_t) ch;
	return ch;
}
#endif


 /*Function used to export a float or integer to PC */
void PC_WriteHex(int16_t BytesOut)
{
	uint8_t Tranbuf;
	 
	Tranbuf=BytesOut&0xff;
	HAL_UART_Transmit(&huartPC,&Tranbuf,1,0xff);
	Tranbuf=(BytesOut>>8)&0xff;
	HAL_UART_Transmit(&huartPC,&Tranbuf,1,0xff);
	return;
}


void PC_ExprotOrig( struct EMGACCs_t *pstr)
 {
	 uint8_t hex2send[2];
	 uint16_t EMG_order[N_EMG_ch];
	 int i0;
	 
	 hex2send[0]=0x55;
	 hex2send[1]=0x50;
	 
	 HAL_UART_Transmit(&huartPC,&hex2send[0],1,0xff);
	 HAL_UART_Transmit(&huartPC,&hex2send[1],1,0xff);
	 
	 //modify the order of EMGs
	 for (int j=0; j<BlockSize; j++)
	 {
		 for (int i=0; i<Nch; i++)
		 {
			 if (i<NchMCM_Master)
			 {
				 EMG_order[j*Nch+i] = pstr->EMGs[j*NchMCM_Master+i];
			 }
			 else if(i<NchMCM_Master+NchMCM_slave)
			 {
				 EMG_order[j*Nch+i] = pstr->EMGs[BlockSize*NchMCM_Master+j*NchMCM_slave+i-NchMCM_Master];
			 }
			 else
			 {
				 i0 = i-NchMCM_Master-NchMCM_slave;
				 EMG_order[j*Nch+i] = pstr->EMGs[BlockSize*(NchMCM_Master+NchMCM_slave)+j*(NADCTDM*NchTDM)+8*(i0%NchTDM)+(i0-i0%NchTDM)/NchTDM];
			 }
		 }
	 }
	 
	 for (int i=0; i<BlockSize; i++)
	 {
		 for (int k=0; k<8; k++)
		 {
			 PC_WriteHex((EMG_order[i*Nch+OutCHs[k]-1]));
		 }
	 }
	 for(int k=0; k<10; k++)
	 {
		 PC_WriteHex(0x00);
	 }
 }
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

