// code for receiving data from XBee radio
// xBee is connected to pins 4 and 5


#include <SoftwareSerial.h>

#define zb_txPin 5
#define zb_rxPin 4

SoftwareSerial xBeeSerial(zb_rxPin, zb_txPin); // RX, TX

void setup()
{
  Serial.begin(9600);
  xBeeSerial.begin(9600);
  pinMode(13,OUTPUT);
}


void loop()
{
  byte inByte;
  while ( xBeeSerial.available() > 0)
  {
    digitalWrite(13,HIGH);
    inByte = xBeeSerial.read();
    Serial.print("rcvd: ");
    Serial.println(inByte,HEX);
  }
  digitalWrite(13,LOW);
}

