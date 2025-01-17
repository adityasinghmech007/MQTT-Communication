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
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  const static char *ca_cert =
"\
-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\n\
";

  const static char *client_cert =
"\
-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVALJDqaAR4fcR46oulQrfg++G+uriMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA4MTcyMDUx\n\
NThaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC5sy4v27FRXGKrVirj\n\
EwHmvslfV1oN6gUrVWEKELgKdlehXHHslcm4fnw9CJQiM7qaLEwXWBhmnWxW1DX/\n\
tn7U+kk3IU/EpA/LqO2gjsh9Twe/KsrR0NC0IW2JUe1/nzN5mTQeDk1VweOT3dDd\n\
1ebIbDCG16riwUAMzV8mYTg8bCf1ie7K2WBS11FN+2gxHKqdP4zKkrq5fhFHA0If\n\
2I8taMc0VnGfAQN/kQCiDGd/pX5QhgnMXn4sqXsl4e3BpJ3Bo7TNR6wll3Kt7M4Y\n\
Zi78Bu9+QfmOEnFPtiL+GtiMUNRJq9lgEZEm5xmobWgoObeepKIM3F/Tano1Uqla\n\
oATNAgMBAAGjYDBeMB8GA1UdIwQYMBaAFHhRLb434MKGY8y5h4/za3gYky3cMB0G\n\
A1UdDgQWBBS61R7qZbCYuslZDcCSR8NLZzBTqDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAdXE5/kRpzHeaK9C5wf7voaM0\n\
VQeSGhuoVSLx1QbuKvBNZKm0vCRRI44mJ4nLgbka9utGk65xc/2EGhemoS8TTcQx\n\
7MoZWiFnVLKcpM67wiCIvuBF5EubL4dosbCc0T6vSR3BqbfZub2f/oBp0ui2vixJ\n\
pdILkN9nqGGPltbAkAcrGYrvjlA9ZmUFn7w11Xr3pUIbjTsqSCMFpFUBtyNVJkUJ\n\
qtcrzodK+syIpVzW2C4s78QVmxj8cQ/KOYpX8L4RYiQ2fZbrGp/zXctOZ9JXgE5p\n\
SUXzEBzWqs3j1+58UHhQr54+TGqMte+MoTCaSWbs5aeTT5grTzS3zHLYRNNOHg==\n\
-----END CERTIFICATE-----\n\
";

  const static char *private_key =
"\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAubMuL9uxUVxiq1Yq4xMB5r7JX1daDeoFK1VhChC4CnZXoVxx\n\
7JXJuH58PQiUIjO6mixMF1gYZp1sVtQ1/7Z+1PpJNyFPxKQPy6jtoI7IfU8HvyrK\n\
0dDQtCFtiVHtf58zeZk0Hg5NVcHjk93Q3dXmyGwwhteq4sFADM1fJmE4PGwn9Ynu\n\
ytlgUtdRTftoMRyqnT+MypK6uX4RRwNCH9iPLWjHNFZxnwEDf5EAogxnf6V+UIYJ\n\
zF5+LKl7JeHtwaSdwaO0zUesJZdyrezOGGYu/AbvfkH5jhJxT7Yi/hrYjFDUSavZ\n\
YBGRJucZqG1oKDm3nqSiDNxf02p6NVKpWqAEzQIDAQABAoIBADpG4AzG3kvROWnZ\n\
vdXLRkJ6x0lzaWCKe5rQzYZSHt+xqeiK0XnUvBx5HVYuFgbxwvLVSf/ddrLlXnWV\n\
XWjlhcIi1c1GV/k3H3njhsdnZ8XjgmPc8w+Lf6O4Bd8dZfIyLP5RNQjlpiFumpPx\n\
/bVQhn+JbGU0030b9pTN8rlhsztKkFvjzY8ZZeXXmvJR2XsNocJRf3Ww/RDsXKct\n\
qJGydDHSeT+HP7peG0oASpXEODseWF1EoxhVvtqQ3AwPY+8CVJ+ZbWyGqgXnIPp1\n\
JOQeYT6Oa33gVdEQ9gWvdwvNq/nIhEYUTBGHKRxnsLLW0TgFT0Y8aw56DwEWjDQ3\n\
AmUy/CUCgYEA8fY7mveDz+j86yVAUt9z9LJTSNSKICOXj60LitGCVxAs6LUzi95a\n\
+n6HZc1+E3c78rzL3wxxu9pQ/Tv+9StnUTotv/0D/9IuHaT3BcqrT8H+vPkEGufH\n\
c4tP3TuCn62DVS6DbrepSh/C07x++VplkOEajZo8xVkDqNq8R9j7CtsCgYEAxHlP\n\
h3t4dBSyMo3kH7naPXR8Pn6Srsz/ngyAkBfXzrvAisopUSauAW9TArnI3frplTDo\n\
06Uj/z8X8iVXUHvGCQEeSnmn+kmdGeqTO/WDij0Pk/C/6J1fAVGrFWMkIW090ZLE\n\
g6ECycDEo/2JoqLHrPagLqzWU9OQ4qu3EmcAu3cCgYEAoju9P2ySHsdTPb8eX/Kf\n\
ZwRiP0Ty0BXNO3KFeLB9Oi8ITYehZjjSu342/pVH4Xli4qCf+Y/4ekIpZ4HgszZE\n\
8fuWTVIyakbc0NeZIXMAZlvml4JHtFqQKvLPu8B5FuNaWbT0Pzo58g9VrG6InFx2\n\
GSw1h5hRAulqQL9Y2rurp10CgYBhNQgFJzCkdizoWE2a6NAygbSLEwKt251I7DSb\n\
lkBe94O+52Dj1rhW22Z9vb/FHER/XRCEznrt6aCGXLVDi8miYNxEysYyTkOdM/Me\n\
wk0Pk0Bm2MGKbHo549c6oK+anjrsuqnh2kLP4UEpReLYBaO3TtpX3Ym5Nb7Ed2L/\n\
htjU5wKBgCH1H8RPY2X0N3fCkDRtyinue+qdt4FEtRZQtFgiEOgyYpF+fJ8dembW\n\
9nIrNYixDWgGx/zGUnE4mhqfcDiMg5baaDJyBMtGuU8wK3hMZ7KXLCBBA4szZDgK\n\
4UoUKJkA5Hc717o4fSli89/6b4U0IysMPwOLdOClsd8pjO3mcg25\n\
-----END RSA PRIVATE KEY-----\n\
";
  bool cert1_status = false;
  bool cert2_status = false;
  bool cert3_status = false;

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
  /* USER CODE BEGIN 2 */

  while (!cert1_status){
	  cert1_status =  uploadcertificate1(ca_cert);
  }
  while (!cert2_status){
	  cert2_status =  uploadcertificate2(client_cert);
  }
  while (!cert3_status){
	  cert3_status =  uploadcertificate3(private_key);
  }
  //cert2_status =  uploadcertificate2(client_cert);
  //cert3_status =  uploadcertificate3(private_key);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
