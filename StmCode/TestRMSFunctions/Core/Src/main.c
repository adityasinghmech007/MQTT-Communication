/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g0xx_hal.h"
#include <string.h>
#include <string.h>
#include <stdbool.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    ERROR_NONE = 0,
    ERROR_UART_TRANSMIT,
    ERROR_UART_RECEIVE,
    ERROR_MQTT_INIT,
    ERROR_MQTT_CONNECT,
    ERROR_MQTT_PUBLISH,
    ERROR_CERT_UPLOAD,
	ERROR_PDP_INIT,
	ERROR_UNKOWN,
	ERROR_QUEC_ON,
	ERROR_QUEC_OFF,
	ERROR_TIMEOUT,
	ERROR_RECV_TIMEOUT,
	ERROR_TRANS_TIMEOUT,

    // Add more error codes as needed
} ErrorCode;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
uint8_t buffer[400];  // Buffer for receiving data
volatile uint8_t uart_tx_complete = 0;  // Flag to indicate transmission completion
volatile uint8_t uart_rx_complete = 0;  // Flag to indicate reception completion// Timeout in milliseconds
ErrorCode SIMTransmit(char *cmd, uint32_t rcv_len, uint32_t rcv_timeout);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART4_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool Quectelon(void)
{
    // Set PB7 high to power on the module
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    // Delay for 3 seconds (if needed for power stabilization)
    HAL_Delay(3000);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

    // Leave PB7 high (do not reset it) if it needs to stay high to keep the module on
    // Return true to indicate the module is powered on
    return true;
}
void logError(ErrorCode code) {
    switch (code) {
        case ERROR_UART_TRANSMIT:
            printf("Error: UART transmission failed\n");
            break;
        case ERROR_UART_RECEIVE:
            printf("Error: UART reception failed\n");
            break;
        case ERROR_TRANS_TIMEOUT:
            printf("Error: Transmission timed out\n");
            break;
        case ERROR_RECV_TIMEOUT:
            printf("Error: Reception timed out\n");
            break;
        default:
            printf("Unknown error occurred\n");
            break;
    }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        uart_tx_complete = 1;  // Set flag when transmission is complete
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
    	HAL_UART_Receive_IT(&huart3, buffer, 400);
        uart_rx_complete = 1;  // Set flag when reception is complete
    }
}

ErrorCode SIMTransmit(char *cmd, uint32_t rcv_len, uint32_t rcv_timeout) {
    memset(buffer, 0, sizeof(buffer));
    uart_tx_complete = 0;
    uart_rx_complete = 0;

    // Start non-blocking transmission
    if (HAL_UART_Transmit_IT(&huart3, (uint8_t *)cmd, strlen(cmd)) != HAL_OK) {
        return ERROR_UART_TRANSMIT;
    }

    // Wait for transmission to complete with timeout
    uint32_t txstart = HAL_GetTick();
    const uint32_t txtimeout = 5000; // Timeout in milliseconds
    while ((!uart_tx_complete) && ((HAL_GetTick() - txstart < txtimeout))) {
        // Optionally perform other tasks or enter low-power mode
    }

    if (!uart_tx_complete) {
        return ERROR_TRANS_TIMEOUT;
    }

    // Start non-blocking reception
    if (HAL_UART_Receive_IT(&huart3, buffer, rcv_len) != HAL_OK) {
        return ERROR_UART_RECEIVE;
    }

    // Wait for reception to complete or timeout
    uint32_t rx_start = HAL_GetTick();
    while (!uart_rx_complete && (HAL_GetTick() - rx_start < rcv_timeout)) {//when I put a break point here the code performs unexpectedly
        // Optionally handle other tasks or sleep
    }

    if (!uart_rx_complete) {
        return ERROR_RECV_TIMEOUT;
    }

    return ERROR_NONE; // Return success if no errors occurred
}


ErrorCode assignPDPcontext(void)
{
	SIMTransmit("AT+QICSGP=1,1,\"airtelgprs.com\",\"\",\"\",0\r\n",100,3000);
	HAL_Delay(1000);
	if (strstr((char*) buffer, "OK"))
	{
		SIMTransmit("AT+QIACT=1\r\n", 100, 3000);
		HAL_Delay(1000);
		if (strstr((char*) buffer, "OK"))
		{
			SIMTransmit("AT+QIACT?\r\n", 100, 3000);
			HAL_Delay(1000);
			if (strstr((char*) buffer, "+QIACT: 1,1,1"))
			{
				return ERROR_NONE ;
			}
		}
	}
	else
	{
		SIMTransmit("AT+QIACT?\r\n", 100, 3000);
		HAL_Delay(1000);
		if (strstr((char*) buffer, "+QIACT: 1,1,1"))
		{
			return ERROR_NONE ;
		}
		return ERROR_NONE;
	}
	return ERROR_PDP_INIT;
}

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  SIMTransmit("AT\r\n", 400, 3000);
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART4_UART_Init(void)
{

  /* USER CODE BEGIN USART4_Init 0 */

  /* USER CODE END USART4_Init 0 */

  /* USER CODE BEGIN USART4_Init 1 */

  /* USER CODE END USART4_Init 1 */
  huart4.Instance = USART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART4_Init 2 */

  /* USER CODE END USART4_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

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

#ifdef  USE_FULL_ASSERT
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
