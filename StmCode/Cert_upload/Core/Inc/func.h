/*
 * func.h
 *
 *  Created on: Aug 29, 2024
 *      Author: SEIL
 */

#ifndef INC_FUNC_H_
#define INC_FUNC_H_
#include <stdbool.h>
#include "stm32g0xx_hal_conf.h"
extern UART_HandleTypeDef huart3;

void SIMTransmit(char *cmd,uint32_t rcv_len,uint32_t rcv_timeout);
bool uploadcertificate1(const char *cert);
bool uploadcertificate2(const char *cert);
bool uploadcertificate3(const char *cert);

#endif /* INC_FUNC_H_ */
