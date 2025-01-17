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
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
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

    // Use the defined macros to initialize variables
//    uint8_t data[PACKET_DATA_LENGTH] = {
//    	      0x67, 0xA4, 0x0A, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x08, 0x47,
//    	      0x00, 0x0D, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x42, 0x00, 0x01, 0x05,
//    	      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//    	      0x00, 0x00, 0x00, 0x00, 0x00, 0x69, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x11, 0x22,
//    	      0x33, 0x44, 0x55, 0x66, 0x77, 0x88
//    };
    uint8_t txData[] = TX_DATA;
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
  const char  *ca_cert =
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

  const char  *client_cert =
"\
-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAJsyykVtj5qYpVPC9n6fc3HxdcgfMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDEwMDcwNTQ5\n\
NDJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDAsjhFS+YTA8rVT0eE\n\
L4P7QGbtMxVvq8rt+laddFJ+r9f43Y7bU+xN0wglEBPi+PWfk34baMowSvj3LEut\n\
zUEL91w0xY5v24gjUY+FjSIL9/t1O/1iSlWz+WzB7HRylByMuvAyHnWO7bMIlqDR\n\
quJ4om3m5M8dTDVxdBaph2s+HaQr63uI6B2BHXqQkkSCsfjFeELrXMFlpizSFLbB\n\
ynkJIkh802wg+uGhYLS5gVCCStajGo/JDY67B3IjD0NrVqELJrsuKMOcuPf6Ipae\n\
gUPwRlKTtnrheBrJdWyN/Mao4VJ1cgmgXIWlNkOxy2yk/zkvONwc6jhXzXdmDkoy\n\
bIVRAgMBAAGjYDBeMB8GA1UdIwQYMBaAFEWMNml3DLxZ381c9yKxnYNFdliEMB0G\n\
A1UdDgQWBBQWC7r7+l6ym+dzyxEoeyQnr2YCYDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEALZQs7wAcG3JoMG1uqOxTc4kf\n\
xU/tLZkDrvsoqxBhp3nH2yWYf3IdbDjIk5Bc8ll+PbeWPUCU8r7zds/Ml+3AvG6i\n\
vlkgigDu3OIpBim7122nWAefoNg/YK2Y1usbVKpcGAHfVNH6ZugXJKaIhNvlOvDR\n\
oyD29wpebj9Wyned5cZLapvV+dY2owBRZO/v3bbrd3yROPlHVnRS2WVUVrz8/ttX\n\
AW7mwLHkSQd+xcyqqwrW4c9rEQMCraplvkp+ljOvD0+g+KGHe7o90v3tOnQJElYV\n\
cjntaA9a/XOTIpejsIaU4LKMwU086QJhF726cB3QXCcBBpw4xgRfLxBKAiq+0Q==\n\
-----END CERTIFICATE-----\n\
";

  const char  *private_key =
"\
-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAwLI4RUvmEwPK1U9HhC+D+0Bm7TMVb6vK7fpWnXRSfq/X+N2O\n\
21PsTdMIJRAT4vj1n5N+G2jKMEr49yxLrc1BC/dcNMWOb9uII1GPhY0iC/f7dTv9\n\
YkpVs/lswex0cpQcjLrwMh51ju2zCJag0arieKJt5uTPHUw1cXQWqYdrPh2kK+t7\n\
iOgdgR16kJJEgrH4xXhC61zBZaYs0hS2wcp5CSJIfNNsIPrhoWC0uYFQgkrWoxqP\n\
yQ2OuwdyIw9Da1ahCya7LijDnLj3+iKWnoFD8EZSk7Z64XgayXVsjfzGqOFSdXIJ\n\
oFyFpTZDsctspP85LzjcHOo4V813Zg5KMmyFUQIDAQABAoIBAH92pEA6Z3NWg9o7\n\
+euXXq9ZN3xZ2SkoMzDd8mNu6Y7BlJQbQn8UcXA9UtrpWq49zoQDFVzeITh2r18Y\n\
3uqVphxbQJPrxj95sQDj15MY+1d9bslqqD5XO3+0UtuoJJMSJBx3H6GXgl3MICxj\n\
LvfiGLKBhz8/RT11BrkIeGEloavQ2q/067cz+X0tzxPj2+cgTUfHjm1qo9le4UZN\n\
hprSnlCqbZSoKBWtlOMfTChiOuoc+4BUNYopEkYSlkCK5FpyxsifjXNR4KsC/cx5\n\
0hPzKpr1tpeN+88PS9gSq8ZUqilk7NXvp1s72lKts0LDWs0aVKPk9Ke5yUyROKHw\n\
28v8yb0CgYEA8kq1xxVILSMLg76HEvhKFHsiZAF/RwQw+tTOnqgjqL8D8pK6Hqjl\n\
qzWXv9o6fhDKlNumW8NNV+B5nWF8u2LQF36nmcAitWZi+4nONEA7H809Gh7OuFCh\n\
Y/09TWGJxNt6C19CKO7kXKK1hJQvqGhLVad+h0Xey+muS8eOa4E7ly8CgYEAy5kt\n\
73rrbA8DrYCTJW3SssAeB3cc/mStgpnvsEV8N863n5+rkPSlHvwV3wfI3E2b+oev\n\
OK33FWfElr/4oOL6nbmNTAN3fX35k03i56/0Vn8S6261D54f/8vzUHWJTvLaru0P\n\
SmYELVdTh7cnb10ZKnsZTYf55YKRlYeK8aAai38CgYB8CAzIDjf1i0QgvA8PgfUC\n\
SQqQ4UdThmzAauIey/n9L1OJAN2wRkvT+kpllwRsdpIN4+dkgDT+sFXxX+9nSa02\n\
6cpIZdgevnwbQi9CBkN+AyWtUmoGZphHxGJ2U2EgWi4+1hPbOnfCiDSd2vIOpt0x\n\
yt11yCnGfyiPeGeK1ww3VwKBgAuySB5K7H1vu2FbUpjC32jfepopMKQ4unMXTtrM\n\
kx9No+TqsluIgnfuOX9vng1x8/UHmHxkGh7DsFqMusbbY1RuVXy5ckLmWtEol1GC\n\
HUJe2IJoAMTd666xU9AagIHll8SW39sBU1obezP/xKc0LP7wxXpbxnJ19tElUUki\n\
ToQXAoGBAMEf61tI5knwjkOpHMlNX+RHOxx5xBE4TbivsbObK4Sarw2Jcj/LWkuF\n\
B3bMD2dRRSDQe/r6LJjLERLEZGbLIdMQGPUg8uhrAs7T4MYTN9JIsr0/ehIjqM8v\n\
HbD9l70i/VH23a3tett8GUqErMAkQJMh+Yh40Y6pkNJC3QNJpmA3\n\
-----END RSA PRIVATE KEY-----\n\
";
  bool cert1_status = false;
  bool cert2_status = false;
  bool cert3_status = false;
  /* USER CODE BEGIN 2 */
  //uint32_t data_to_write = 0x12345678;


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

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
 while(1)
 {
  /* USER CODE END WHILE */

//  bool TransmitDataMqtt_flag = false;
//  HAL_StatusTypeDef Inverter_receive_status=HAL_TIMEOUT;
  bool rcvmessage = false;
  while(!rcvmessage)
  {
	  rcvmessage = Subscribetotopic("testpub1",1600,3000);
  }
  // Declare variables for start and end indices
  uint32_t start_idx, end_idx;

  // Assign values to start and end indices at runtime
  start_idx = 95;  // These values can be changed at runtime
  end_idx = 1600;    // This can also be user-defined or come from another source

      // Extract values from start_idx to end_idx and copy to reqcert buffer
      Extract_Values(buffer, reqcert, start_idx, end_idx);



  Write_Flash(FLASH_USER_START_ADDR,reqcert,end_idx-start_idx);

      // Now reqcert contains the values between buffer[start_idx] and buffer[end_idx - 1]
  UnsubscribefromTopic("testpub1");


  // Try to receive the inverter data
  // The  TransmitDataMqtt will not be called until data is received
//  while(Inverter_receive_status!=HAL_OK)
//  {
//	  HAL_UART_Transmit_IT(&huart4, txData, sizeof(txData));
//  	  Inverter_receive_status=HAL_UART_Receive(&huart4, data, PACKET_DATA_LENGTH,2000);
////  	  HAL_Delay(2000);
//  	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//  }

    // Transmit Data via MQTT through Quectel
//  uint8_t debug = 1 ;
  //SIMTransmit("AT+QICSGP=1,1,\"airtelgprs.com\",\"\",\"\",0",40,3000);

//    TransmitDataMqtt_flag = TransmitDataMqtt(data,PACKET_DATA_LENGTH,&debug);

//  After the execution of TransmitDataMqtt function the TransmitDataMqtt_flag may either be true or false.
//  For debugging purposes if the TransmitDataMqtt_flag is true then the LED blinks at a faster rate(Less Delay).
//  And for unsuccessful transmission the LED blinks slower rate( High Delay).
//  The LED Blinks until the timer exceeds the MINUTES_INTERVAL duration.

//    uint32_t last_tick = HAL_GetTick(); // Initialize the last tick time
//    while (((HAL_GetTick() - last_tick)) < MINUTES_INTERVAL)
//    {
//      if (TransmitDataMqtt_flag)
//      {
//        // Transmission Successful
//        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//        HAL_Delay(500);
//        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//        HAL_Delay(500);
//      }
//      else
//      {
//        // Transmission Failed
//        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//        HAL_Delay(1500);
//        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//        HAL_Delay(1500);
//      }
//    }
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

  /*Configure GPIO pins : PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
