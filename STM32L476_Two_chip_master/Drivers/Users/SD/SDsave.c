#include "SDsave.h"
#include "stm32l4xx_hal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "math.h"

TCHAR const Data_path_name[]="Data";
TCHAR Data_path[10];
BYTE work[_MAX_SS];
FILINFO fno;
DIR dir;
TCHAR wtext[100];
TCHAR filename[20];

int Data_file_num=0;
void SD_Initialize(void);

//static uint8_t isInitialized = 0;
/*void SD_Initialize(void)
{
	if (isInitialized == 0)
	{
		if (BSP_SD_Init() == MSD_OK)
		{
			BSP_SD_ITConfig();
			isInitialized = 1;
			printf("BSP_SD_initialize OK\r\n");
		}
		else
		{
			printf("BSP_SD_initialize Error\r\n");
			while(BSP_SD_IsDetected() != SD_PRESENT);
		}
	}
}*/

void SD_init(void)
{
	//char* pstr;
	//char fnum[6];
	//SD_Initialize();
	/*mount the sd card*/
	retSD = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	
	/*creat filesystem if not exist*/
	if (retSD == FR_NO_FILESYSTEM)
	{
		retSD = f_mkfs((TCHAR const* )SDPath, FM_ANY, 0, work, sizeof work);
		retSD = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
	}
	
	/*check if director exist, creat if not*/
	sprintf(Data_path, "/%s/", Data_path_name);	
	retSD = f_stat(Data_path, &fno);
	if (retSD == FR_NO_PATH)
	{
		retSD = f_mkdir(Data_path_name);
	}
	
	/* open the director, check number of exist file*/
	retSD = f_opendir(&dir, Data_path_name);
	
	Data_file_num = 0;
	while(f_readdir(&dir, &fno)==FR_OK && fno.fname[0] != 0 )
	{
		/*count the number of files*/
		Data_file_num = Data_file_num % 2000 +1;		
	}
	
	//printf("     %d\r\n", Data_file_num);
}

void 	Creat_File_in_SD( void)
{
	
	Data_file_num = (Data_file_num) % 2000+1;
	sprintf(filename, "/%s/%d", Data_path_name , Data_file_num);
	retSD = f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
}


void Write_Node_in_SD(struct EMGACCs_t *pstr)
{
	uint32_t byteswritten;
	uint16_t EMGACCframe[N_EMG_ch+9+2+5];
	int16_t buf;
	int i0;
//	uint16_t buf;
	EMGACCframe[0] = 0x5050;
	EMGACCframe[1] = 0x5555;
	
		for (int j=0; j<BlockSize; j++)
	 {
		 for (int i=0; i<Nch; i++)
		 {
			 if (i<NchMCM_Master)
			 {
				 EMGACCframe[j*Nch+i+2] = pstr->EMGs[j*NchMCM_Master+i];
			 }
			 else if(i<NchMCM_Master+NchMCM_slave)
			 {
				 EMGACCframe[j*Nch+i+2] = pstr->EMGs[BlockSize*NchMCM_Master+j*NchMCM_slave+i-NchMCM_Master];
			 }
			 else
			 {
				 i0 = i-NchMCM_Master-NchMCM_slave;
				 EMGACCframe[j*Nch+i+2] = pstr->EMGs[BlockSize*(NchMCM_Master+NchMCM_slave)+j*(NADCTDM*NchTDM)+8*(i0%NchTDM)+(i0-i0%NchTDM)/NchTDM];
			 }
		 }
	 }
	 
	 for (int i=0; i<3;i++)
	 {
		 buf = (int16_t)(pstr->a[i]*32768.0f/16.0f);
		 memcpy(&(EMGACCframe[N_EMG_ch+i+2]), &buf, 4); 
	 }
	 for (int i=0; i<3;i++)
	 {
		 buf = (int16_t)(pstr->w[i]*32768.0f/636.6197724f);
		 memcpy(&(EMGACCframe[N_EMG_ch+3+i+2]), &buf, 4); 
	 }
	 for (int i=0; i<3;i++)
	 {
		 buf = (int16_t)(pstr->Angle[i] * 32768.0f/180.0f);
		 memcpy(&(EMGACCframe[N_EMG_ch+6+i+2]), &buf, 4); 
	 }
	 
	 for (int i=0; i<5; i++)
	 {
		 EMGACCframe[N_EMG_ch+9+2+i] = 0x0000;
	 }
	
//	 for (int i=0; i<N_EMG_ch+9; i++)
//	 {
//		 EMGACCframe[i] = 0x006D;
//	 }	 
	
	retSD = retSD || f_write(&SDFile, EMGACCframe, sizeof(EMGACCframe), (void*)&byteswritten);
	
//	for (int k =0; k<N_EMG_ch; k++)
//	{
//		buf = EMGs[k];
//		sprintf(wtext, "%d,", buf);
//		
//	}
	
}

void Close_File_in_SD()
{
	retSD = retSD || f_close(&SDFile);
	if (retSD != FR_OK)
	{
		f_unlink(filename);
		Data_file_num--;
	}
}

void Delete_File_in_SD()
{
	f_unlink(filename);
}






