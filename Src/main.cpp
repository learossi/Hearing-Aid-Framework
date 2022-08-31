//See the ReadMe for a breakdown of all the stuff here

#include "main.h"

//Objects for the initialization of the logic
typedef struct{
	FilterBank<FIRLength, samples_per_cycle, FilterBank_channels> FB;
	Audiogram AG = Audiogram();
	Input_Manager<FFTLength> m;
	Filter_Pipeline pipeline;
	Compressor<samples_per_cycle> c = Compressor<samples_per_cycle>(0,0,0,0,0,0);
}HAid_Init_struct;

HAid_Init_struct h {};
void HAid_init(HAid_Init_struct& init);


//STM32-Nucleo HAL structs & functions
 UART_HandleTypeDef huart2;
 DMA_HandleTypeDef dma_hand;
 ADC_HandleTypeDef hadc;
 TIM_HandleTypeDef tim2_hand;
 ADC_HandleTypeDef adc1_hand;

//functions that come packaged with the STM32CubeIDE
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);


int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  SystemClock_Config();

  //Set peripherals parameters in registers
  DMA_Init(&dma_hand);
  TIM2_Init(&tim2_hand);
  HAL_TIM_Base_MspInit(&tim2_hand);
  ADC1_Init(&adc1_hand, &dma_hand);

  //these MX denoted function come from the IDE
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  //initialize the logic
  HAid_init(h);

  //initialize the timer denoted TIM2
  HAL_TIM_Base_Start(&tim2_hand);

  //initialize hardware resources
  HAL_ADC_MspInit(&hadc);





  //The ADC is started here with the call to the DMA using hadc as the ADC parameters
  HAL_ADC_Start_DMA(&hadc, reinterpret_cast<uint32_t*>(h.m.get_current_vals()), samples_per_cycle);





  h.pipeline.add_task([](){h.pipeline.get_data(h.m.get_current_vals());}, false);
  h.pipeline.add_task([](){h.pipeline.initialize_filter_bank();}, false);
  h.pipeline.add_task([](){
	  for(int i = 0; i < FilterBank_channels; ++i){
		 h.AG.interpolate_gain(i, h.FB.get_ppn_vals(i));
	  }
  }, false);
  h.pipeline.add_task([](){h.pipeline.terminate_filter_bank();},false);

  while(1){

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
/*
void MX_USART2_UART_Init(void)
{

   USER CODE BEGIN USART2_Init 0

   USER CODE END USART2_Init 0

   USER CODE BEGIN USART2_Init 1

   USER CODE END USART2_Init 1
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
   USER CODE BEGIN USART2_Init 2

   USER CODE END USART2_Init 2

}
*/




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

//This is the callback that actually recurs the logic. Anytime the DMA finishes, this gets called
//and restarts the DMA as well as updating the data pointer and calling the pipeline to execute again.
//Here is where calls to output would go as well.
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	std::array<complex_type, FFTLength>* current_vals = h.m.get_current_vals();
	h.m.next_input();
	HAL_ADC_Start_DMA(hadc, reinterpret_cast<uint32_t*>(current_vals), samples_per_cycle);
	h.pipeline.get_data(current_vals);
	h.pipeline.execute_tasks();

}

void HAid_init(HAid_Init_struct& init){
	//I should probably create an init struct and pass that in and assign the pointers that way

	//I might need to pass in the DMA pointer and assign it to some pipeline type structure
	init.m = Input_Manager<FFTLength>();

	//how am I exposing this to main? Am I forward declaring a pointer and allocating it here?
	//Also, do I really want to have the filter bank own the array of vals? that seems bad
	 init.FB = FilterBank<FIRLength, samples_per_cycle, FilterBank_channels>();


	 //find a better place for this later
	 const float audiogram_gain[] = {2,3,2,1,1,3,7,2,1,2};

	 init.AG = Audiogram(audiogram_gain, 10);



}
