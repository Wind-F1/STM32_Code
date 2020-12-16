#include "FATFS_user.h"
#include "stm32l4xx_hal.h"


extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;
extern SD_HandleTypeDef hsd1;

HAL_StatusTypeDef SD_DMAConfigRx(SD_HandleTypeDef *hsd)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	
	hdma_sdmmc1_rx.Instance = DMA2_Channel4;
	hdma_sdmmc1_rx.Init.Request = DMA_REQUEST_7;
	hdma_sdmmc1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_sdmmc1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_sdmmc1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_sdmmc1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_sdmmc1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_sdmmc1_rx.Init.Mode = DMA_NORMAL;
  hdma_sdmmc1_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	
	__HAL_LINKDMA(hsd, hdmarx, hdma_sdmmc1_rx);
	
	status = HAL_DMA_Abort(&hdma_sdmmc1_rx);
	status = HAL_DMA_DeInit(&hdma_sdmmc1_tx);
	
	status = HAL_DMA_Init(&hdma_sdmmc1_rx);
	
	return (status);
}

HAL_StatusTypeDef SD_DMAConfigTx(SD_HandleTypeDef *hsd)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	
	hdma_sdmmc1_tx.Instance = DMA2_Channel5;
	hdma_sdmmc1_tx.Init.Request = DMA_REQUEST_7;
	hdma_sdmmc1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_sdmmc1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_sdmmc1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_sdmmc1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_sdmmc1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_sdmmc1_tx.Init.Mode = DMA_NORMAL;
	hdma_sdmmc1_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	
	__HAL_LINKDMA(hsd, hdmatx, hdma_sdmmc1_tx);
	
	status = HAL_DMA_Abort(&hdma_sdmmc1_tx);
	status = HAL_DMA_DeInit(&hdma_sdmmc1_rx);
	
	status = HAL_DMA_Init(&hdma_sdmmc1_tx);
	
	return status;
	
}



uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;
	
	//invalidate the dma tx handle
	hsd1.hdmatx = NULL;
	
	//prepare the dma channel for a read operation
	sd_state = SD_DMAConfigRx(&hsd1);
	
  
  // Read block(s) in DMA transfer mode 
  if (HAL_SD_ReadBlocks_DMA(&hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }
  
  return sd_state; 
}


uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
{
  uint8_t sd_state = MSD_OK;
	
	//invalidate the dma tx handle
	hsd1.hdmarx = NULL;
	
	//prepare the dma channel for a read operation
	sd_state = SD_DMAConfigTx(&hsd1);
  
  // Write block(s) in DMA transfer mode 
  if (HAL_SD_WriteBlocks_DMA(&hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }
  
  return sd_state; 
}



