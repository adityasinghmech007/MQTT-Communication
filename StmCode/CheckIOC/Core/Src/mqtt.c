#include "mqtt.h"
#include "config.h"
#include "main.h" // Include any necessary headers for HAL functions and GPIO pins
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
// #include <ArduinoJson.h>
//  Add any necessary global variables or constants here

//static char endpoint[] = "a2wusdljipwadj-ats.iot.ap-south-1.amazonaws.com";
//static char thingname[] = "test1";

const uint32_t conn_timeout = 40000;
char ATcommand[400];

uint8_t buffer[400] = { 0 };
uint8_t ATisOK = 0;
uint8_t CGREGisOK = 0;
uint32_t previousTick;
uint16_t readValue;
char charData[15];
int count = 0;

// Define the initialization commands for MQTT
//These commands are taken from the Ec200 MQTT application note.
//These commands are configurable and can be changed according to the requirements of the application.

const char *commands[] = {
		"AT+QSSLCFG=\"cacert\",2,\"UFS:root\"\r\n",
		"AT+QSSLCFG=\"clientcert\",2,\"UFS:cert\"\r\n",
		"AT+QSSLCFG=\"clientkey\",2,\"UFS:key\"\r\n",
		"AT+QSSLCFG=\"seclevel\",2,2\r\n",
		"AT+QSSLCFG=\"sslversion\",2,4\r\n",
		"AT+QSSLCFG=\"ciphersuite\",2,0xFFFF\r\n",
		"AT+QSSLCFG=\"ignorelocaltime\",2,1\r\n",
		"AT+QMTCFG=\"SSL\",0,1,2\r\n",
		"AT+QMTCFG=\"version\",0,4\r\n",
		"AT+QMTCFG=\"recv/mode\",0,0,1\r\n"
};
//#define BUZZER_Pin GPIO_PIN_X  // Replace X with your chosen pin number
//#define BUZZER_GPIO_Port GPIOA  // Replace GPIOA with the actual port of your chosen pin

// Define the MQTT connection commands

// Define the payload and topic for publishing
// uint8_t hex_data[] = {0x67, 0xA4, 0x0A, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x08, 0x47, 0x00, 0x0D, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x42, 0x00, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x69};

// Get the JSON string from the function
char json_string[300]; // Global declaration

//The hex_to_json function converts the hexadecimal values that are being received from the display into
//a json packet consisting of an object called data that can be identified by the amplify application and this data can be pushed into the database.
void hex_to_json(uint8_t *data, int data_length)
{
	// Construct JSON string
	sprintf(json_string, "{\"data\": [");

	for (int i = 0; i < data_length; i++)
			{
		// Convert each byte from hexadecimal to decimal and add to JSON string
		sprintf(json_string + strlen(json_string), "%d", data[i]);

		if (i < data_length - 1)
				{
			strcat(json_string, ",");
		}
	}

	strcat(json_string, "]}");

	// No need to return the JSON string
}
char* add_serial_to_json( char* json_string, const char* serial_number) {
    // Construct the new JSON string with the serial number
    sprintf(json_string + strlen(json_string), "], \"serial_number\": \"%s\"}", serial_number);
    return json_string;
}

// Function to transmit commands to the SIM module
//The SIMTransmit function takes several arguments i.e. cmd,rcv_len and rcv_timeout.
//The cmd argument consists of the AT command that is to be transmitted.
//The rcv_len and rcv_timeout functions are specific to the HAL_UART_Receive function for defining variable rcv_len and rcv_timeouts for different AT commands.
//The timeouts and reception length for different ATcommands used in the code are defined on observing the module responses with 2G and 4G networks.
//The timeouts and reception length can be optimized further while testing.

void SIMTransmit(const char *cmd, uint32_t rcv_len, uint32_t rcv_timeout)
{
	//  memset(ATcommand,0,sizeof(ATcommand));

	memset(buffer, 0, sizeof(buffer));

	HAL_UART_Transmit_IT(&huart3, (uint8_t*) cmd, strlen(cmd));
	//  HAL_Delay(2000);
	HAL_UART_Receive(&huart3, buffer, rcv_len, rcv_timeout);

}

bool Subscribetotopic(const char *topic)
{

    char command[100]; // Buffer to hold the AT command
    if (checkConnection())
    	{
    		if (initializeMQTT())
    		{
    			if (openMQTT())
    			{
    				if (connectMQTT())
    				{
    				    // Format the command with the provided topic
    				    snprintf(command, sizeof(command), "AT+QMTSUB=0,1,\"%s\",0\r\n", topic);

    				    // Transmit the command
    				    SIMTransmit(command, 400, 3000);

    				    // Check for a successful subscription response
    				    if (strstr((char*) buffer, "+QMTSUB: 0,1,0,0"))
    				    {
    				        return true;
    				    }
    				    return false;
    				}
    				return false;
    			}
    			return false;
    		}
    		return false;
    	}
    return false;
}



bool UnsubscribefromTopic(const char *topic) {
    char command[100]; // Buffer to hold the AT command

    // Format the command with the provided topic
    snprintf(command, sizeof(command), "AT+QMTUNS=0,1,\"%s\"\r\n", topic);

    // Transmit the command
    SIMTransmit(command, 400, 3000);

    {    // Check for a successful unsubscribe response
    if (strstr((char*) buffer, "+QMTUNS: 0,1,0"))
    {
    	if (disconnectMQTT())
    	{
    		return true;
    	}
    	return false;
    }
        return false;
    }
    return false;
}

bool receiveMessage(uint32_t rcv_len , uint32_t rcv_timeout)
{
	HAL_UART_Receive(&huart3, buffer, rcv_len, rcv_timeout);

    if (strstr((char *)buffer, "+QMTRECV:"))
    {
        // Extract and process the message from buffer

        return true;// Example processing
    } else
    {
       return false;

    }
}



bool assignPDPcontext(void)
{
	SIMTransmit("AT+QICSGP=1,1,\"airtelgprs.com\",\"\",\"\",0\r\n", 100, 3000);
	if (strstr((char*) buffer, "OK"))
			{
		SIMTransmit("AT+QIACT=1\r\n", 100, 3000);
		if (strstr((char*) buffer, "OK"))
				{
			SIMTransmit("AT+QIACT?\r\n", 100, 3000);

			if (strstr((char*) buffer, "+QIACT: 1,1,1"))
					{
				return true;
			}
		}
	}
	else
	{
		SIMTransmit("AT+QIACT?\r\n", 100, 3000);

		if (strstr((char*) buffer, "+QIACT: 1,1,1"))
				{
			return true;
		}
		return true;
	}
	return false;
}

bool Quectelon(void)
{
    // Set PB7 high to power on the module
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    // Delay for 3 seconds (if needed for power stabilization)
    HAL_Delay(3000);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

    HAL_Delay(3000);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    HAL_Delay(2000);

    // Leave PB7 high (do not reset it) if it needs to stay high to keep the module on
    // Return true to indicate the module is powered on
    return true;
}

bool Quecteloff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

    HAL_Delay(3500);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    HAL_Delay(32000);

    return true;


}
//void toggleBuzzer(void) {
//    HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin); // Toggle buzzer state
//    HAL_Delay(500); // Delay for 500 ms
//}

bool uploadcertificate1(const char *cert)
{
	uint32_t cert_len = strlen(cert);
	SIMTransmit("AT+QFUPL=\"UFS:root\",1187,5\r\n", 80, 3000);

	if (strstr((char*) buffer, "CONNECT"))
			{
		SIMTransmit((char*) cert, cert_len, 3000);
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
			return uploadcertificate1(cert); // Fixed to return the recursive call result
		}
	}
	return false;
}

bool uploadcertificate2(const char *cert) {
	uint32_t cert_len = strlen(cert);
	SIMTransmit("AT+QFUPL=\"UFS:cert\",1224,7\r\n", 80, 3000);

	if (strstr((char*) buffer, "CONNECT")) {
		SIMTransmit((char*) cert, cert_len, 3000);

		if (strstr((char*) buffer, "+QFUPL: 1224,3977")) {
			return true;
		}
		return false;
	} else if (strstr((char*) buffer, "+CME ERROR: 407")) {
		SIMTransmit("AT+QFDEL=\"UFS:cert\"\r\n", 50, 2000);
		if (strstr((char*) buffer, "OK")) {
			return uploadcertificate2(cert); // Fixed to return the recursive call result
		}
	}
	return false;
}

bool uploadcertificate3(const char *cert) {
	uint32_t cert_len = strlen(cert);
	SIMTransmit("AT+QFUPL=\"UFS:key\",1675,7\r\n", 80, 3000);

	if (strstr((char*) buffer, "CONNECT")) {
		SIMTransmit((char*) cert, cert_len, 3000);

		if (strstr((char*) buffer, "+QFUPL: 1675,6a3b")) {
			return true;
		}
		return false;
	} else if (strstr((char*) buffer, "+CME ERROR: 407")) {
		SIMTransmit("AT+QFDEL=\"UFS:key\"\r\n", 50, 2000);
		if (strstr((char*) buffer, "OK")) {
			return uploadcertificate3(cert); // Fixed to return the recursive call result
		}
	}
	return false;
}

//Function to check connection with the Quectel Module
//The checkConnection function firstly checks the response of the module by sending an AT command continuously and waiting for either the timeout to occur or the module to response an OK response.
//After the module returns OK to the basic connection command it confirms the module's connectivity with the microController, now the function checks for the network registration of the module by the second AT command
//which is "AT+CGREG?\r\n" , the response of this command must either be "+CGREG:0,1" for local network connectivity and "+CGREG:0,5" for roaming connectivity.
//If the module don't respond or if the module is not able to find any network connection within the timeout period then the function response with a false output.
bool checkConnection(void)
{
	ATisOK = 0;
	CGREGisOK = 0;

	// Check for OK response for AT
	previousTick = HAL_GetTick();
	while (!ATisOK && previousTick + conn_timeout > HAL_GetTick())
	{
		count++;
		SIMTransmit("AT\r\n", 100, 2000);

		if (strstr((char*) buffer, "OK"))
				{
			ATisOK = 1;
		}
	}

	// Check for network registration.
	if (ATisOK)
	{
		previousTick = HAL_GetTick();
		while (!CGREGisOK && previousTick + conn_timeout > HAL_GetTick())
		{
			SIMTransmit("AT+CGREG?\r\n", 100, 2000);
			if (strstr((char*) buffer, "+CGREG: 0,1")) // Use 0,5 For Roaming
					{
				CGREGisOK = 1;
				return true;
			}
		}
	}
	return false;
}

// Function to initialize MQTT
// The initalizeMQTT function one by one initializes the defined initialization commands by sending AT commands using the SIMTransmit function.
// There is a timeout defined for each command named conn_timeout that is same as the checkConnection()function.
// Each command is repeatively send to the Quectel module for execution until the module responses with an OK response.
// If there is a timeout for any command the function will stop executing further and then return false as output.
// If all commands are executed successfully then the function returns true.
bool initializeMQTT(void)
{
	uint32_t startTime = HAL_GetTick();
	const uint32_t timeout = 10000;
	while (HAL_GetTick() - startTime < timeout)
	{
		for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
				{
			SIMTransmit(commands[i], 400, 3000);
			if (!strstr((char*) buffer, "OK"))
					{
				continue;
			}
		}
		return true;

	}
	return false;
}

// Function to connect to MQTT broker
// The connectMQTT function transmits and analyzes the responses of two commands i.e. "AT+QMTOPEN" and "AT+QMTCONN".
// The QMTOPEN command is for opening a connection with my AWS server with 8883 port for SSL based MQTT communication.
// The QMTCONN command is for connecting to a particular thing defined on the AWS server.
// Both of the commands have 2 retries for a single transmission.
// This function also handles the responses of both of the commands, the details of the responses can be seen from the MQTT application note mentioned in the start.
// If a response of "+QMTOPEN: 0,2" is received it means that either the connection is being made or already a connection is established. But as the module was not able to transmit the message,
// So the previous connection is closed using "AT+QMTCLOSE" AT command and the loop is continued to try to connect to the server again.

bool openMQTT(void)
{
	uint32_t startTime = HAL_GetTick();
	const uint32_t timeout = 100000;

	while (HAL_GetTick() - startTime < timeout)
	{
		// Send the command to open an MQTT connection
		SIMTransmit(
				"AT+QMTOPEN=0,\"a2wusdljipwadj-ats.iot.ap-south-1.amazonaws.com\",8883\r\n",
				100, 50000);

		if (strstr((char*) buffer, "+QMTOPEN: 0,0"))
				{
			// Successfully opened, proceed to connect
			return true;

		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,-1"))
				{
			// Connection failed, close and retry
			if (disconnectMQTT())
			{
				continue;
			}
			else
			{
				return connectMQTT();

			}
		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,1"))
				{
			// MQTT client identifier not valid, reinitialize and retry
			initializeMQTT();
			return openMQTT();
		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,2"))
				{
			// Server is unavailable, close and retry
			disconnectMQTT();
			return openMQTT();
		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,3"))
				{
			// Connection rejected, assign PDP context and retry
			assignPDPcontext();
			initializeMQTT();
			return openMQTT();
		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,4"))
				{
			// TCP/IP connection failure, initialize and retry
			initializeMQTT();
			return openMQTT();
		}
		else if (strstr((char*) buffer, "+QMTOPEN: 0,5"))
				{
			// Protocol not supported, check connection and retry
			checkConnection();
			assignPDPcontext();
			initializeMQTT();
			return openMQTT();
		}
		else
		{
			// Unknown error, return false
			return false;
		}

		// Delay between retries
	}
	return false;  // Timeout occurred
}
bool connectMQTT(void)
{
	uint32_t startTime = HAL_GetTick();
	const uint32_t timeout = 10000;

	while (HAL_GetTick() - startTime < timeout)
	{
		SIMTransmit("AT+QMTCONN=0,\"test2\"\r\n", 100, 3000);

		if (strstr((char*) buffer, "+QMTCONN: 0,0,0"))
				{
			return true;  // Connection successful
		}
		else if (strstr((char*) buffer, "+QMTCONN: 0,1"))
				{
			return false;

		}
	}
	return false;

}

// Function to disconnect from MQTT broker
bool disconnectMQTT(void)
{
	uint32_t startTime = HAL_GetTick();
	const uint32_t timeout = 10000; // 10 seconds timeout

	while (HAL_GetTick() - startTime < timeout)
	{
		SIMTransmit("AT+QMTCLOSE=0\r\n", 400, 2000);

		if (strstr((char*) buffer, "+QMTCLOSE: 0,0"))
				{
			// Successful disconnect
			return true;
		}
		else
		{
			// Error occurred, you might want to handle this differently
			return false;
		}
	}

	// Timeout occurred
	return false;
}

// Function to publish message to MQTT topic
// The PublishToTopic function takes two arguments first is the data from display to be transmitted and second is the data_length.
// It executes and analyze the output of the publish command i.e. "AT+QMTPUB", after the initial command is executed, if there is no errors, the module asks the user for the message to be published.
// The data is sent to the hexTojson function to convert the data in a json format that is defined in the function.
// Next the json data is sent to the module using the SimTransmit function with a terminating character '0xla' to indicate the module that the message is completed and transmit it.
// If the response is not "+QMTPUB: 0,1,0" it states that the message transmission is not successful and send a terminate character in case if the module has not terminated the message.

bool publishToTopic(uint8_t *data, int data_length)
{
	if (data == NULL || data_length == 0)
			{
		return false; // Handle invalid input
	}
	HAL_Delay(100);
	SIMTransmit("AT+QMTPUB=0,2,1,1,\"testsub\"\r\n", 400, 2000);
	HAL_Delay(1000);

	if (strstr((char*) buffer, ">"))
			{
		hex_to_json(data, data_length);
		sprintf(ATcommand, "%s%c", json_string, 0x1a);
		SIMTransmit(ATcommand, 400, 5000);
		HAL_Delay(500);
		if (strstr((char*) buffer, "+QMTPUB: 0,2,0"))
				{
			//		       disconnectMQTT();
			return true;
		}

	}

	return false;
}
bool TransmitDataMqtt(uint8_t *data, int data_length, uint8_t *a)
{
	*a = 1;
	if (checkConnection())
	{
		if (initializeMQTT())
		{
			if (openMQTT())
			{
				*a = 3;
				if (connectMQTT())
				{
					*a = 4;
					if (publishToTopic(data, data_length))
							{
						*a = 5;
						if (disconnectMQTT())
						{
							return true;

						}
						return false;
					}
					return false;
				}

				return false;
			}
			return false;
		}
		return false;
	}
	return false;
}
//bool Readrcvdata(const char *topic, uint8_t *a)
//{
//	*a = 1;
//	if (checkConnection())
//	{
//		if (initializeMQTT())
//		{
//			if (openMQTT())
//			{
//				*a = 3;
//				if (connectMQTT())
//				{
//					*a = 4;
//					if (Subscribetotopic(topic))
//					{
//
//						return true;
//					}
//					return false;
//				}
//
//				return false;
//			}
//			return false;
//		}
//		return false;
//	}
//	return false;
//}
// Function for Overall Transmission of Inverter Data via MQTT to Cloud.
// The TransmitDataMqtt function handle the overall MQTT communication using other MQTT functions i.e.checkConnection,connectMQTT,initializeMQTT,publishToTopic.
// In this function a flow is defined for the MQTT functions to be called in a sequential order, this function could be further optimized if any errors are encountered during testing.
// The TransmitDataMqtt function takes around 3-4 minutes on initial setup(when the module is turned on) after that if the module is able to open a connection with the MQTT server it takes less than 10 seconds to transmit a message, until the MQTT connection is open.
//bool TransmitDataMqtt(uint8_t *data, int data_length ,uint8_t *a)
//{
//  if (checkConnection())
//  {
//	  *a = 2;
//    if (!publishToTopic(data, data_length))
//    {
//    	 *a = 3;
//      if (!connectMQTT())
//      {
//    	  *a = 4;
//        initializeMQTT();
//        *a = 5;
//        connectMQTT();
//        *a =6;
//        if (publishToTopic(data, data_length))
//        {
//        	*a =7;
////          disconnectMQTT();
//          return true;
//        }
//      }
//      else
//      {
//    	  *a =8;
//        if (publishToTopic(data, data_length))
//        {
//        	*a =9;
////          disconnectMQTT();
//          return true;
//        }
//      }
//    }
//    else
//    {
//    	*a =10;
////      disconnectMQTT();
//      return true;
//    }
//  }
//  *a =11;
//  return false;
//}