#include "SD_ADCtest.h"
#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "ADC.h"
#include "PCSerial.h"
#include "SDsave.h"

void testADC_SD(void)
{
	static int flag_SD = 0;
	//static int NUMlim = 0;
	
		//PC_ExprotOrig( &EMGACCs);
		

			if (flag_SD == 0)
			{
				Creat_File_in_SD();
			}
			else if (flag_SD ==5)
			{
				//Write_Node_in_SD(&EMGACCs);			
			}
			else if (flag_SD == 10 )
			{
				Close_File_in_SD();
			}
			else if (flag_SD == 50)
			{
				Delete_File_in_SD();
			}
			flag_SD = (flag_SD+1)%100;

}





