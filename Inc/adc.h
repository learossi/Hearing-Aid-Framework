/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

//#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

extern void Error_Handler(void);

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
//#include "stm32f4xx_hal_adc.h" //this is not very portable

extern ADC_HandleTypeDef hadc1;


/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void ADC1_Init(ADC_HandleTypeDef* hadc1, DMA_HandleTypeDef* output_stream);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

