#ifndef MQTT_H
#define MQTT_H
#include <stdbool.h>
#include "stm32g0xx_hal.h"

extern UART_HandleTypeDef huart3;
extern uint8_t buffer[400];
bool initializeMQTT(void);
bool connectMQTT(void);
bool checkConnection(void);
bool publishToTopic(uint8_t *data, int data_length);
bool TransmitDataMqtt(uint8_t *data, int data_length,uint8_t *x);
bool disconnectMQTT(void);
void SIMTransmit(const char *cmd,uint32_t rcv_len,uint32_t rcv_timeout);
bool uploadcertificate1(const char *cert);
bool uploadcertificate2(const char *cert);
bool uploadcertificate3(const char *cert);
bool assignPDPcontext(void);
bool Quectelon(void) ;
bool Quecteloff(void);
void toggleBuzzer(void);
bool openMQTT(void);
bool receiveMessage(uint32_t rcv_len , uint32_t rcv_timeout);
bool Subscribetotopic(const char *topic);
bool UnsubscribefromTopic(const char *topic);
char* add_serial_to_json( char* json_string, const char* serial_number);
#endif /* MQTT_H */
