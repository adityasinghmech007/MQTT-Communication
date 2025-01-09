/*
 * error_codes.h
 *
 *  Created on: Sep 19, 2024
 *      Author: SEIL
 */

#ifndef INC_ERROR_CODES_H_
#define INC_ERROR_CODES_H_


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


#endif /* INC_ERROR_CODES_H_ */
