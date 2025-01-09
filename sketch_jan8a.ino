#define dtr_pin 9
#define ri_pin 8 
#define rst_pin 6
#define rts_pin 11
#define cts_pin 10
#define dcd_pin 12
#include<SoftwareSerial.h>
SoftwareSerial mySerial(2,3);//rx,tx



void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial.println("Connecting To Server........");
  delay(1000);
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+QMTOPEN=0,\"test.mosquitto.org\",1883");//open a connection with mosquitto server on port 1883//
  delay(1000);
  mySerial.println("AT+QMTCONN=0,\"ec200ucntest\",\"\",\"\"");//connect using connection id username id, username and password//
  delay(1000);
  Serial.println("Done");
  // put your setup code here, to run once:
}

void loop() {
  //Serial.println(randNumber);
  mySerial.println("AT+QMTPUB=0,1,1,0,\"topicees\"");
  mySerial.println("My name is \"Aditya\" and I like coffee\x1A");
  //Serial.println("MessageSent");





  
  //Serial.println(randNumber);

  //publishing MQTT Messages to Subscribers 

  // put your main code here, to run repeatedly:

}
