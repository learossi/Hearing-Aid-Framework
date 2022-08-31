#ifndef __MAIN_H
#define __MAIN_H

#include "Audiogram.h"
#include "Filter_Pipeline.h"
#include "InputManager.h"
#include "utilities.h"
#include "Compressor.h"

#include "dma.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <complex>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"

void Error_Handler(void);

//pulled from the STM32CubeIDE from STM
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define Green_LED_Pin GPIO_PIN_5
#define Green_LED_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
