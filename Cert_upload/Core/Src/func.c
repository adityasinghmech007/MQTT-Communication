/*
 * func.c
 *
 *  Created on: Aug 29, 2024
 *      Author: SEIL
 */
#include "main.h" // Include any necessary headers for HAL functions and GPIO pins
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stm32g0xx_hal_conf.h>
extern UART_HandleTypeDef huart3;

const uint32_t conn_timeout = 40000;
char ATcommand[2048];

uint8_t buffer[2048] = { 0 };
uint8_t ATisOK = 0;
uint8_t CGREGisOK = 0;
uint32_t previousTick;
uint16_t readValue;
char charData[15];
int count = 0;


// Function to transmit commands to the SIM module
//The SIMTransmit function takes several arguments i.e. cmd,rcv_len and rcv_timeout.
//The cmd argument consists of the AT command that is to be transmitted.
//The rcv_len and rcv_timeout functions are specific to the HAL_UART_Receive function for defining variable rcv_len and rcv_timeouts for different AT commands.
//The timeouts and reception length for different ATcommands used in the code are defined on observing the module responses with 2G and 4G networks.
//The timeouts and reception length can be optimized further while testing.

void SIMTransmit(const char *cmd, uint32_t rcv_len, uint32_t rcv_timeout) {
	//  memset(ATcommand,0,sizeof(ATcommand));
	memset(buffer, 0, sizeof(buffer));
	HAL_StatusTypeDef error_tr;
	HAL_StatusTypeDef error_rc;
	//error_tr = HAL_UART_Transmit_IT(&huart3, (uint8_t *)cmd, strlen(cmd));
	//error_tr = HAL_UART_Transmit(&huart3, (uint8_t*) cmd, strlen(cmd), HAL_MAX_DELAY);
	error_tr = HAL_UART_Transmit_IT(&huart3, (uint8_t *)cmd, strlen(cmd));
	//  HAL_Delay(2000);
	error_rc = HAL_UART_Receive(&huart3, buffer, rcv_len, rcv_timeout);


}
bool uploadcertificate1(const char *cert)
{
    uint32_t cert_len = strlen(cert);
    SIMTransmit("AT+QFUPL=\"UFS:root\",1187,5\r\n", 80, 3000);

    if (strstr((char*) buffer, "CONNECT"))
    {
        SIMTransmit((char*)cert, cert_len, 3000);
        HAL_Delay(2000);

        if (strstr((char*) buffer, "+QFUPL: 1187,2d19"))
        {
            return true;
        }
        return false;
    }
    else if (strstr((char*) buffer, "+CME ERROR: 407"))
    {
        SIMTransmit("AT+QFDEL=\"UFS:root\"\r\n", 50, 2000);
        if (strstr((char*) buffer, "OK"))
        {
            return uploadcertificate1(cert);  // Fixed to return the recursive call result
        }
    }
    return false;
}



bool uploadcertificate2(const char *cert) {
    uint32_t cert_len = strlen(cert);
    SIMTransmit("AT+QFUPL=\"UFS:cert\",1224,7\r\n", 80, 3000);

    if (strstr((char*) buffer, "CONNECT")) {
    	SIMTransmit((char*)cert, cert_len, 3000);
    	HAL_Delay(2000);

        if (strstr((char*) buffer, "+QFUPL: 1224,5977")) {
            return true;
        }
        return false;
    }else if (strstr((char*) buffer, "+CME ERROR: 407")) {
        SIMTransmit("AT+QFDEL=\"UFS:cert\"\r\n", 50, 2000);
        if (strstr((char*) buffer, "OK")) {
            return uploadcertificate2(cert);  // Fixed to return the recursive call result
        }
    }
    return false;
}


bool uploadcertificate3(const char *cert) {
    uint32_t cert_len = strlen(cert);
    SIMTransmit("AT+QFUPL=\"UFS:key\",1675,7\r\n", 80, 3000);

    if (strstr((char*) buffer, "CONNECT")) {
    	SIMTransmit((char*)cert, cert_len, 3000);
    	HAL_Delay(2000);

        if (strstr((char*) buffer, "+QFUPL: 1675,526f")) {
            return true;
        }
        return false;
    }else if (strstr((char*) buffer, "+CME ERROR: 407")) {
        SIMTransmit("AT+QFDEL=\"UFS:key\"\r\n", 50, 2000);
        if (strstr((char*) buffer, "OK")) {
            return uploadcertificate3(cert);  // Fixed to return the recursive call result
        }
    }
    return false;
}






