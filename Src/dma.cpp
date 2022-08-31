/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "dma.h"

void DMA_Init(DMA_HandleTypeDef* dma_hand)
{

  /* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	DMA_InitTypeDef dma_config;

	dma_config.Channel = DMA_CHANNEL_0;
	dma_config.Direction = DMA_PERIPH_TO_MEMORY;
	dma_config.PeriphInc = DMA_PINC_DISABLE;
	dma_config.MemInc = DMA_MINC_ENABLE; //this smells alls sorts of unsafe to me
	dma_config.PeriphDataAlignment = DMA_PDATAALIGN_WORD; //check this once I've sorted out the adc
	dma_config.MemDataAlignment = DMA_MDATAALIGN_WORD;
	dma_config.Mode = DMA_NORMAL;
	dma_config.Priority = DMA_PRIORITY_VERY_HIGH;
	dma_config.FIFOMode = DMA_FIFOMODE_ENABLE;
	dma_config.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	dma_config.MemBurst = DMA_MBURST_INC16;
	dma_config.PeriphBurst = DMA_PBURST_INC16;


	dma_hand ->Instance = DMA1_Stream0;
	dma_hand ->Init  = dma_config;

	HAL_DMA_Init(dma_hand);

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

