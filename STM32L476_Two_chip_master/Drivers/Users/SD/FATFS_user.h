#ifndef __FATFS_USER_H__
#define __FATFS_USER_H__

#include "fatfs.h"

HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd);
HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd);


#endif
