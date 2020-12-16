#ifndef __ADC_H__
#define __ADC_H__

#include "stdint.h"

#define CH1A0 GPIO_PIN_RESET
#define CH1A1 GPIO_PIN_RESET
#define CH1EN GPIO_PIN_SET

#define CH2A0 GPIO_PIN_RESET
#define CH2A1 GPIO_PIN_SET
#define CH2EN GPIO_PIN_SET

#define CH3A0 GPIO_PIN_SET
#define CH3A1 GPIO_PIN_RESET
#define CH3EN GPIO_PIN_SET

#define CH4A0 GPIO_PIN_SET
#define CH4A1 GPIO_PIN_SET
#define CH4EN GPIO_PIN_SET


#define CHUN GPIO_PIN_RESET

//Multi-Channel number of master chip
#define NchMCM_Master 16

//Multi-Channel number of slave chip
#define NchMCM_slave 16

//Time Division chip number
#define NADCTDM 8

//Channel number per Time Division chip
#define NchTDM 4

//Total channe number
#define Nch (NchMCM_Master+NchMCM_slave+NADCTDM*NchTDM)

#define BlockSize 30

//Size of a frame
#define N_EMG_ch (Nch*BlockSize)

struct EMGACCs_t
{
	uint16_t EMGs[N_EMG_ch];	
	float a[4];
	float w[4];
	float Angle[4];
};

enum flagbuf {buf1 = 1,	buf2 = 2};

extern void ADCstart(void);
extern void ADCstop(void);

extern struct EMGACCs_t EMGACCs;
extern volatile int flag_EMG_TDM;
extern volatile int flag_EMG_MCM_Master;
extern volatile int flag_EMG_MCM_Slave;

void ADCstart(void);


#endif
