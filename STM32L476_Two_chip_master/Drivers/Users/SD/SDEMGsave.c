#include "SDsave.h"
#include <stdio.h>
#include "SDEMGsave.h"
#include "main.h"
#include "ADC.h"


void EMGsave_SD(struct EMGACCs_t *pstr)
{
	static int nframe = 0;
	
	GPIO_PinState SD_EN;
	static Filestate SDFilestate;	
	
	 SD_EN = HAL_GPIO_ReadPin(SD_EN_GPIO_Port, SD_EN_Pin);
	
	//printf("EMGsave_SD: %d\r\n", SD_EN);
	 
	if (SD_EN == GPIO_PIN_RESET)
	{
		
		if (nframe == 0)
		{
				Creat_File_in_SD();
				Write_Node_in_SD(pstr);
				//printf("Creat a File\r\n");
				SDFilestate = FileOpen;
			
		}
		else if(nframe == 1999)
		{
				Write_Node_in_SD(pstr);
				Close_File_in_SD();
				//printf("Close a File\r\n");
				SDFilestate = NoFile;
			
		}
		else
		{
				Write_Node_in_SD(pstr);
			  //printf("Write a Node\r\n");
		}
		nframe = (nframe+1) % 2000;
	}
	else
	{
		if (SDFilestate == FileOpen)
		{
			Close_File_in_SD();
			//printf("Close a File\r\n");
			SDFilestate = NoFile;

		}
		nframe = 0;
		//printf("Receive\r\n");
			
	}
	
	
	
	
	
	
	
}






