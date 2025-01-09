
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <M5Core2.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "SHT3X.h"
SHT3X sht30;
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;


// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "aws/smartkit001/test"
#define AWS_IOT_SUBSCRIBE_TOPIC "aws/smartkit001/test"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);


void messageHandler(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}


  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(a1eznyoivi8qv-ats.iot.ap-south-1.amazonaws.com, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{

  pressure = bme.readPressure();
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure);
  delay(100);

  StaticJsonDocument<200> doc;
  doc["tmp"] = tmp;
  doc["hum"] = hum;
  doc["pressure"] = pressure;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  M5.begin();
  Wire.begin();
  bme.begin(0x76);
  connectAWS();
}

void loop() {
  publishMessage();
  client.loop();
  delay(1000);
}

