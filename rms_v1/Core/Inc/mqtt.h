#ifndef MQTT_H
#define MQTT_H
#include <stdbool.h>
#include "stm32g0xx_hal.h"

extern UART_HandleTypeDef huart3;
extern uint8_t buffer[1600];
extern uint8_t reqcert[1600];
#define FLASH_USER_START_ADDR   0x0801F800  // Address to start writing (example)
#define FLASH_USER_END_ADDR     0x0801FFFF  // End address of the user flash area



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
bool Subscribetotopic(const char *topic,uint32_t rcv_len, uint32_t rcv_timeout);
bool UnsubscribefromTopic(const char *topic);
void Extract_Values(uint8_t *src_buffer, uint8_t *dest_buffer, uint32_t start_idx, uint32_t end_idx);
void Write_Flash(uint32_t address, uint8_t *data, uint32_t length);
extern uint8_t trx_buffer[400];
#endif /* MQTT_H */
