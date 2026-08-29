/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

/* USER CODE BEGIN PV */

#define greenmap 1
#define redmap 2
#define whitemap 3

/*
#define greenmap 2
#define redmap 3
#define whitemap 1
 */


uint8_t shift1[8] = {2, 3, 1, 2, 3, 1, 2, 3};
uint8_t shift2[8] = {3, 1, 2, 3, 1, 2, 3, 1};
//uint8_t shift3[8] = {3, 1, 2, 3, 3, 1, 2, 3};
uint8_t shift3[8] = {2, 3, 1, 2, 2, 3, 1, 2};
uint8_t shift4[8] = {3, 1, 2, 3, 1, 2, 3, 1};
uint8_t shift5[8] = {1, 2, 3, 1, 2, 3, 1, 2};

uint8_t if_red = 0;
uint8_t if_green = 0;
uint8_t if_white = 0;

uint16_t timeval;

uint32_t old_dance_time;

uint8_t order = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define SER_PIN GPIO_PIN_7
#define SER_PORT GPIOA

#define SRCLK_PIN GPIO_PIN_6
#define SRCLK_PORT GPIOA

#define RCLK_PIN1 GPIO_PIN_3
#define RCLK_PORT1 GPIOA

#define RCLK_PIN2 GPIO_PIN_5
#define RCLK_PORT2 GPIOA

#define RCLK_PIN3 GPIO_PIN_9
#define RCLK_PORT3 GPIOA

#define RCLK_PIN4 GPIO_PIN_10
#define RCLK_PORT4 GPIOA

#define RCLK_PIN5 GPIO_PIN_4
#define RCLK_PORT5 GPIOA

uint8_t currentVal =  0b00000000;

void HC595write(uint8_t latch)
{
    for(int i=0; i<8; i++)
    {
        if(currentVal & (1<<i))
        {
        	HAL_GPIO_WritePin(SER_PORT, SER_PIN, GPIO_PIN_SET);

        }
        else
        {
           HAL_GPIO_WritePin(SER_PORT, SER_PIN, GPIO_PIN_RESET);
        }
        HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);


    }
    if(latch == 1){
    HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_SET);
    }
    if(latch == 2){
    HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_SET);
    }
    if(latch == 3){
    HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_SET);
    }
    if(latch == 4){
    HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_SET);
    }
    if(latch == 5){
    HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_SET);
    }

}

void HC595writePin(uint8_t pin, uint8_t value, uint8_t latch)
{
      if (value == 0)
      {
          currentVal &= ~(1 << pin);
      }
      else
      {
          currentVal |= (1 << pin);
      }
      HC595write(latch);
}



void LED_Manager(uint8_t red, uint8_t green, uint8_t white){
	for(int i = 0; i < 8; i++){
			  if(shift1[i] == greenmap){
			  HAL_GPIO_WritePin(SER_PORT, SER_PIN, green);
			  }
			  if(shift1[i] == redmap){
			  HAL_GPIO_WritePin(SER_PORT, SER_PIN, red);
			  }
			  if(shift1[i] == whitemap){
			  HAL_GPIO_WritePin(SER_PORT, SER_PIN, white);
			  }
			  //HAL_Delay(1);
			   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
			   //HAL_Delay(1);
			   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
		}
			   HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_RESET);
			   HAL_Delay(1);
			   HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_SET);


				for(int i = 0; i < 8; i++){
						  if(shift2[i] == greenmap){
						  HAL_GPIO_WritePin(SER_PORT, SER_PIN, green);
						  }
						  if(shift2[i] == redmap){
						  HAL_GPIO_WritePin(SER_PORT, SER_PIN, red);
						  }
						  if(shift2[i] == whitemap){
						  HAL_GPIO_WritePin(SER_PORT, SER_PIN, white);
						  }
						  //HAL_Delay(1);
						   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
						   //HAL_Delay(1);
						   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
					}
						   HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_RESET);
						   HAL_Delay(1);
						   HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_SET);

						   /**/
							for(int i = 0; i < 8; i++){
									  if(shift3[i] == greenmap){
									  HAL_GPIO_WritePin(SER_PORT, SER_PIN, green);
									  }
									  if(shift3[i] == redmap){
									  HAL_GPIO_WritePin(SER_PORT, SER_PIN, red);
									  }
									  if(shift3[i] == whitemap){
									  HAL_GPIO_WritePin(SER_PORT, SER_PIN, white);
									  }
									  //HAL_Delay(1);
									   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
									   //HAL_Delay(1);
									   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
								}
									   HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_RESET);
									   HAL_Delay(1);
									   HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_SET);



										for(int i = 0; i < 8; i++){
												  if(shift4[i] == greenmap){
												  HAL_GPIO_WritePin(SER_PORT, SER_PIN, green);
												  }
												  if(shift4[i] == redmap){
												  HAL_GPIO_WritePin(SER_PORT, SER_PIN, red);
												  }
												  if(shift4[i] == whitemap){
												  HAL_GPIO_WritePin(SER_PORT, SER_PIN, white);
												  }
												  //HAL_Delay(1);
												   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
												   //HAL_Delay(1);
												   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
											}
												   HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_RESET);
												   HAL_Delay(1);
												   HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_SET);

													for(int i = 0; i < 8; i++){
															  if(shift5[i] == greenmap){
															  HAL_GPIO_WritePin(SER_PORT, SER_PIN, green);
															  }
															  if(shift5[i] == redmap){
															  HAL_GPIO_WritePin(SER_PORT, SER_PIN, red);
															  }
															  if(shift5[i] == whitemap){
															  HAL_GPIO_WritePin(SER_PORT, SER_PIN, white);
															  }
															  //HAL_Delay(1);
															   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
															   //HAL_Delay(1);
															   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
														}
															   HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_RESET);
															   HAL_Delay(1);
															   HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_SET);




}


void LED_DANCE(){


	if(HAL_GetTick() - old_dance_time >= timeval && order == 0){
				  		LED_Manager(1, 0, 0);
				  		order++;
				  		old_dance_time = HAL_GetTick();
	}
	if(HAL_GetTick() - old_dance_time >= timeval && order == 1){
						LED_Manager(0, 1, 0);
				  		order++;
				  		old_dance_time = HAL_GetTick();
	}
	if(HAL_GetTick() - old_dance_time >= timeval && order == 2){
						LED_Manager(0, 0, 1);
				  		order = 0;
				  		old_dance_time = HAL_GetTick();
	}

	/*
	if(HAL_GetTick() - old_dance_time >= timeval && order == 3){
		HC595writePin(1, 1, 1);
  		order++;
  		old_dance_time = HAL_GetTick();
	}
	if(HAL_GetTick() - old_dance_time >= timeval && order == 4){
		HC595writePin(2, 1, 1);
  		order++;
  		old_dance_time = HAL_GetTick();
	}
	if(HAL_GetTick() - old_dance_time >= timeval && order == 5){
		HC595writePin(3, 1, 1);
  		order++;
  		old_dance_time = HAL_GetTick();
	}

*/





}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
timeval = HAL_ADC_GetValue(hadc);

HAL_ADC_Start_IT(hadc);

}


/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{


	  	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0){
	  	if_green = 1;
		}
	  	else {
	  	if_green = 0;
	  	}

		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0){
			if_red = 1;
		}
	  	else {
	  		if_red = 0;
	  	}

		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0){
	  		if_white = 1;
		}
	  	else {
	  		if_white = 0;
	  	}



	  	if(GPIO_Pin == GPIO_PIN_0){
	  	if_green = 1;
		}
	  	else {
	  	if_green = 0;
	  	}

		if(GPIO_Pin == GPIO_PIN_1){
			if_red = 1;
		}
	  	else {
	  		if_red = 0;
	  	}

		if(GPIO_Pin == GPIO_PIN_2){
	  		if_white = 1;
		}
	  	else {
	  		if_white = 0;
	  	}


		LED_Manager(if_red, if_green, if_white);
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	timeval = HAL_ADC_GetValue(&hadc);


}
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//HAL_Delay(100);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
//HAL_Delay(100);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */

  //HAL_Delay(100);


  //HAL_GPIO_WritePin(RCLK_PORT, RCLK_PIN, GPIO_PIN_SET);

/**/
  for(int i = 0; i < 8; i++){
	  HAL_GPIO_WritePin(SER_PORT, SER_PIN, GPIO_PIN_RESET);
	  //HAL_Delay(1);
	   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_SET);
	   //HAL_Delay(1);
	   HAL_GPIO_WritePin(SRCLK_PORT, SRCLK_PIN, GPIO_PIN_RESET);
	   //HAL_Delay(1);

}

	   HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_RESET);
	   HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_RESET);

	   //HAL_Delay(1);
	   HAL_GPIO_WritePin(RCLK_PORT1, RCLK_PIN1, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(RCLK_PORT2, RCLK_PIN2, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(RCLK_PORT3, RCLK_PIN3, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(RCLK_PORT4, RCLK_PIN4, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(RCLK_PORT5, RCLK_PIN5, GPIO_PIN_SET);
	   HAL_Delay(1);


HAL_ADCEx_Calibration_Start(&hadc);
HAL_ADC_Start_IT(&hadc);

	   old_dance_time = HAL_GetTick();





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0){
	  	  	if_green = 1;
	  	  	order = 0;
	  		}
	  	  	else {
	  	  	if_green = 0;
	  	  	}

	  		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0){
	  			if_red = 1;
	  			order = 0;
	  		}
	  	  	else {
	  	  		if_red = 0;
	  	  	}

	  		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0){
	  	  		if_white = 1;
	  	  	order = 0;
	  		}
	  	  	else {
	  	  		if_white = 0;
	  	  	}
	  		if(if_green == 1 || if_white == 1 || if_red == 1){
	  		LED_Manager(if_red, if_green, if_white);
	  		}

	  		if(if_green == 0 && if_white == 0 && if_red == 0){
	  LED_DANCE();
	  		}







  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_SET);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA6
                           PA7 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
