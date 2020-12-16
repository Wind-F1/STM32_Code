#include "stm32l4xx_hal.h"
#include "ADC.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#define SPIDELAY 40

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadcMCM;
extern ADC_HandleTypeDef hadcTDM;
extern SPI_HandleTypeDef hspi1;


static int flag_CH=(NchTDM-1);
volatile int flag_EMG_TDM=0;
volatile int flag_EMG_MCM_Master=0;
volatile int flag_EMG_MCM_Slave=0;
uint16_t watchbuf[NchMCM_slave*BlockSize];
uint16_t bufMCM_Master[NchMCM_Master*BlockSize];
uint8_t bufMCM_Slave[NchMCM_slave*BlockSize*2];
uint16_t bufTDM1[NADCTDM*NchTDM*BlockSize];

HAL_StatusTypeDef ADCstate;

volatile enum flagbuf flagbufADC;

//static uint16_t buf_adc[Nch];
struct EMGACCs_t EMGACCs;
uint8_t first_begin = 1;

void ADCstart()
{
	HAL_TIM_Base_Start(&htim2);
	ADCstate = HAL_ADC_Start_DMA(&hadcMCM, (uint32_t *)bufMCM_Master, (NchMCM_Master*BlockSize));
	HAL_Delay(5);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_4);
}


void SwitcherCtl(int CH_c)
{
	static int SPI_delay = 0;
	
	if (SPI_delay<SPIDELAY) SPI_delay++;
	
	switch (CH_c)
	{
		case 0:
			HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, CH1A0);
			HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, CH1A1);
			HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, CH1EN);
		  if (SPI_delay==SPIDELAY) HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, GPIO_PIN_SET);
			break;
		case 1: 
			HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, CH2A0);
			HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, CH2A1);
			HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, CH2EN);
			break;
		case 2: 
			HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, CH3A0);
			HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, CH3A1);
			HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, CH3EN);	
		  if (SPI_delay==SPIDELAY) HAL_GPIO_WritePin(ADC_CLK_GPIO_Port, ADC_CLK_Pin, GPIO_PIN_RESET);
			break;
		case 3: 
			HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, CH4A0);
			HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, CH4A1);
			HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, CH4EN);
			break;
	}
	

	return;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc == &hadcTDM)
	{
		HAL_ADC_Stop_DMA(&hadcTDM);
		memcpy(&(EMGACCs.EMGs[(NchMCM_Master+NchMCM_slave)*BlockSize]), bufTDM1, sizeof(bufTDM1));
		ADCstate = HAL_ADC_Start_DMA(&hadcTDM, (uint32_t *)bufTDM1 ,(NADCTDM*NchTDM*BlockSize));
		//printf("TDM:%d\r\n", flag_EMG_TDM);
		flag_EMG_TDM=1;
	}
	else if(hadc == &hadcMCM)
	{
		HAL_ADC_Stop_DMA(&hadcMCM);
		memcpy(EMGACCs.EMGs, bufMCM_Master, sizeof(bufMCM_Master));
		HAL_ADC_Start_DMA(&hadcMCM, (uint32_t *)bufMCM_Master, (NchMCM_Master*BlockSize));
		//printf("master:%d\r\n", flag_EMG_MCM_Master);
		flag_EMG_MCM_Master = 1;
	}
	
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if (first_begin == 1)
		{
			ADCstate = HAL_ADC_Start_DMA(&hadcTDM, (uint32_t *)bufTDM1 ,(NADCTDM*NchTDM*BlockSize));
			//ADCstate = HAL_ADC_Start_DMA(&hadcMCM, (uint32_t *)bufMCM_Master, (NchMCM_Master*BlockSize));
			HAL_SPI_Receive_DMA(&hspi1, bufMCM_Slave, sizeof(bufMCM_Slave));
			flagbufADC = buf1;
			first_begin = 0;
		}
		flag_CH = (flag_CH+1) % (NchTDM) ;
		SwitcherCtl(flag_CH);
	}	
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	HAL_SPI_DMAStop(&hspi1);
	memcpy(&(EMGACCs.EMGs[NchMCM_Master*BlockSize]), bufMCM_Slave, sizeof(bufMCM_Slave));
	memcpy(watchbuf, bufMCM_Slave, sizeof(bufMCM_Slave));
	HAL_SPI_Receive_DMA(&hspi1, bufMCM_Slave, sizeof(bufMCM_Slave));
//	printf("slave:%d\r\n", flag_EMG_MCM_Slave);
	flag_EMG_MCM_Slave = 1;
	
}




