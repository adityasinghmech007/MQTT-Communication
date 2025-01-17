#ifndef MQTT_H
#define MQTT_H
#include <stdbool.h>
#include "stm32g0xx_hal.h"
#include "error_codes.h"

extern UART_HandleTypeDef huart3;
ErrorCode initializeMQTT(void);
ErrorCode connectMQTT(void);
bool checkConnection(void);
bool publishToTopic(uint8_t *data, int data_length);
bool TransmitDataMqtt(uint8_t *data, int data_length,uint8_t *x);
bool disconnectMQTT(void);
void SIMTransmit(char *cmd,uint32_t rcv_len,uint32_t rcv_timeout);
bool uploadcertificate1(const char *cert);
bool uploadcertificate2(const char *cert);
bool uploadcertificate3(const char *cert);
bool assignPDPcontext(void);
bool Quectelon(void) ;

extern uint8_t trx_buffer[400];
#endif /* MQTT_H */
