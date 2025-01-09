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
#include <config.h>
#include <mqtt.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
uint8_t txData[] = TX_DATA;
uint8_t data[70] = {0};
HAL_StatusTypeDef Inverter_receive_status ;

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
//  uint8_t data[PACKET_DATA_LENGTH] = {
//  	      0x67, 0xA4, 0x0A, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x08, 0x47,
//  	      0x00, 0x0D, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x42, 0x00, 0x01, 0x05,
//  	      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  	      0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22,
//  	      0x33, 0x44, 0x55, 0x66, 0x77, 0x88
//  };
  const char static *ca_cert =
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

  const char static *client_cert =
"\
-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAIA8A7zHYbDu5KbIVfmvcL1SkCnDMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA5MjUwODA2\n\
MTJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCWo14QKB32Gq0SftMD\n\
1yjGHJiboqGZOtc0xl99qqigIygAHr6dXTrKA7Mc/MIpOtRs0u/iFpAzKIRO70iG\n\
BHvTMkyjfMqIvkjGX894TTKFta8Xnjnc+l9WHK2rcRrH9TsV4HVmDfKKwHrDMlSw\n\
o5UYvjJgeP+ip24ZDciCGw5XdToZcgXuYoUo4EqRGJAm17VREOeV5eM8vXW+GZM6\n\
tDu6HW/i6i7e+S6MUgwD0le9tLh7p4COLDE7u0d/iXw9lqMRJ4OkVDArIeaa1Omc\n\
ZmulyU5gZUILg7ld3PNo6W9qNqdj3GdMuo8UuloH1jxOCxV10O4kQ63+kn3SEHGk\n\
tQrFAgMBAAGjYDBeMB8GA1UdIwQYMBaAFIp6qbehQFNJ/U305+QhnnufqTo1MB0G\n\
A1UdDgQWBBTPObeEr7PK8ewAUD4aeZTp7bDGLzAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAT0H/aDIh61kuFnFJieVYyHok\n\
W+n/eiUivX5/xnPuwyvzQngoWrHeMzgZWGGjJ/FJhA1eVwSSQ7rX5ZmM6WwSBSd+\n\
oiJLwcMq+MmCCmnrzFiEh1dpdb1y9RHOVTJ1tWgQhcy3A3LTYIMaBwfK9sXKbLOt\n\
cUfeN+Uotbc7PP7DV8mKJq3laKngSt+gfNmbWKmEq+IAOOn45VMezgveWU7ywMaW\n\
+VVFIfQ6egWI/LIM40qMaHNHy0Jsa9WsKTDNuk339jN3dyOtJzps09nxRV+mmDOs\n\
87Zw2WpPlM+EtKwnOK2yeUd19HGlw0nev4moExWsbGGww0ZzDiiv5UKQO9a9Qg==\n\
-----END CERTIFICATE-----\n\
";

  const char static *private_key =
"\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAlqNeECgd9hqtEn7TA9coxhyYm6KhmTrXNMZffaqooCMoAB6+\n\
nV06ygOzHPzCKTrUbNLv4haQMyiETu9IhgR70zJMo3zKiL5Ixl/PeE0yhbWvF545\n\
3PpfVhytq3Eax/U7FeB1Zg3yisB6wzJUsKOVGL4yYHj/oqduGQ3IghsOV3U6GXIF\n\
7mKFKOBKkRiQJte1URDnleXjPL11vhmTOrQ7uh1v4uou3vkujFIMA9JXvbS4e6eA\n\
jiwxO7tHf4l8PZajESeDpFQwKyHmmtTpnGZrpclOYGVCC4O5XdzzaOlvajanY9xn\n\
TLqPFLpaB9Y8TgsVddDuJEOt/pJ90hBxpLUKxQIDAQABAoIBAB8vHKbdly4+YXvF\n\
s2tFHFS+0LzPUwLEko6DTntWW+givwIfWzh6d1QgpfCnRo6Adj88/5nfL+TXS2SH\n\
1oyf2v5imEjz6X2bXrdqWCnAWWgir2eFQtdDIX9TGoS2OmQQmTWzLLp+wqoEpKVK\n\
Aww6IQBpR0Z/ZfRHEtyu5MZCUZq7AYi34rwwVJiS71L0xNCxg9+R9s8KWCjWjn50\n\
5WzeXZinisNB8oyt7H6dexl5oSOjR4Xwmp+7eZ3WlBj0YBsjlenWmgzyzAOXDKrx\n\
gSOcCQUal3vhOUitAd1iF4iN6bRqL8aABnzE4Pl+l9R5TCGSSLef2dHO6aKRNUzG\n\
Q0D/X1UCgYEAxMJz9WFy6jdYClyScQZbY/M0FN5YRqxib5DVjoY71Vq3olase5hq\n\
hLqhzDjNTexveL8WfnOHpcFeFVe5ee4ErthJwKiqNS/x5y5txOFWp1Tkeyrs9P/W\n\
tsxdldYK5WpxE2Jpn9iPoUgDSCVEoL8Nl+I1QZ9p8a6UAYbeYhszVqMCgYEAw/4I\n\
2W6OHI8BaaAYm8AAJaUUFY1SHOQCMzLhotC9kOdZNY8r1a5xWySLSiJe8GU8W8Wy\n\
YJ03b5VORYFgUgYi9rRzVz9tM/pkzt9yJIo0pP2EhhmWKwHbuGK9fCE618Wxr7Fe\n\
6PNgsUKGP7LO4j7YKV1USGx1IomRPBTvhuZGd3cCgYEAlGVkN6lrliJvkEEJYZ09\n\
yEBhA2w+f+77vtEpf/jVn7Bv10wvgngNvlVUOkaM8TxlRlg4it5vbCBO6SHkjYVq\n\
msQdu+R5muuyMn3d5CNfy4xHVm2lkZJzSiMQYjWwuh5xcgFDUH9wtF8vunYBMItn\n\
oSnGqNumd9UHY69oge2p5qkCgYBQUIF/2HYT7eY+SoTLDlsBEiywyECl5imgoXEl\n\
LzT/dE5yTEFCR0Hcf5U0w/DzwgiB8vmt7b8R0rVIdkPqWOlq/3dkgXFWo3076QT1\n\
RE1ebyUnbsTCyvgnDw9nFpbuZOzzelPSUimFeiVZZQSyYd5i1QP8ybUWehkx7cJa\n\
HXMy8QKBgALoOyjA4OM6E/addbhYgr7NfizU2M/7LU73Z3YRHqvzbT1mibHfQYp+\n\
ODuWXHRRTonXJLqVnRSsnszcqzeOkzZcleI4/0p7oWW5RWqEyHsa7OSwGqH9T0Cf\n\
L3hf30HuwtvmHlz/86avjZWig+Ww78pdo9jnsT8pXIOkH3Aj+Q+r\n\
-----END RSA PRIVATE KEY-----\n\
";
  bool cert1_status = false;
  bool cert2_status = false;
  bool cert3_status = false;
  //Quecteloff();
  Quectelon();



  while (!cert1_status){
	  cert1_status =  uploadcertificate1(ca_cert);
  }
  while (!cert2_status){
	  cert2_status =  uploadcertificate2(client_cert);
  }
  while (!cert3_status){
	  cert3_status =  uploadcertificate3(private_key);
  }
  assignPDPcontext();
  checkConnection();
  initializeMQTT();




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  bool rcvmessage = false;
//	  Subscribetotopic("testpub1");
//	  memset(buffer, 0, sizeof(buffer));
//	  while(!rcvmessage)
//	  {
//		  rcvmessage = receiveMessage(400, 30000);
//		  HAL_Delay(30000);
//	  }
//	  UnsubscribefromTopic("testpub1");

	  bool TransmitDataMqtt_flag = false;

//
	  HAL_StatusTypeDef Inverter_receive_status=HAL_TIMEOUT;
	  while(Inverter_receive_status!=HAL_OK){
		  HAL_UART_Transmit_IT(&huart4, txData, sizeof(txData));
	  	  Inverter_receive_status=HAL_UART_Receive(&huart4, data, 70,3000);
	  	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  }
	    // Transmit Data via MQTT through Quectel
	  uint8_t debug = 1;

	    TransmitDataMqtt_flag = TransmitDataMqtt(data,PACKET_DATA_LENGTH,&debug);


	    uint32_t last_tick = HAL_GetTick(); // Initialize the last tick time
	    while (((HAL_GetTick() - last_tick)) < MINUTES_INTERVAL)
	    {
	      if (TransmitDataMqtt_flag)
	      {

	        // Transmission Successful
	        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	        HAL_Delay(500);
	        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	        HAL_Delay(500);
	      }
	      else
	      {
	        // Transmission Failed
	        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	        HAL_Delay(1500);
	        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	        HAL_Delay(1500);
	      }
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB7 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
