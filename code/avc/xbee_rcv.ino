// code for receiving data from XBee radio
// xBee is connected to pins 4 and 5


#include <SoftwareSerial.h>

#define zb_txPin 5
#define zb_rxPin 4

SoftwareSerial xBeeSerial(zb_rxPin, zb_txPin); // RX, TX

void xbeeSetup(){
  xBeeSerial.begin(9600);
  }

bool xbeeAvailible(){
	if (xBeeSerial.available()){
		return true;
	}
	else{
		return false;
	}
}

void readXbee(char frame[]){
  int count = 0;
  byte remoteDirection;;
  byte remoteTrim;
  
  if ( xBeeSerial.available() > 0){
   val = xBeeSerial.read();

    if(val ==  0xAA){
    remoteDirection = xBeeSerial.read();
    //Serial.println(inByte,HEX);
    remoteTrim = xBeeSerial.read();    
    //Serial.println(inByte2, HEX);
    }
  }


    frame[count] = xBeeSerial.read();
    count = count+1;
  }
}
	

