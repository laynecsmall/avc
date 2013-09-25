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
  while ( xBeeSerial.available() > 0)
  {

    frame[count] = xBeeSerial.read();
    count = count+1;
  }
}

void parseFrame(String moveBuffer[],char frame[]){
	//converts the input frame to something useful and adds it to the moveBuffer
	String parsedFrame = "s000";
	for (int i=0; i < sizeof(moveBuffer)/sizeof(moveBuffer[0]);i++){
		if ((moveBuffer[i] == (String) "\0") && (i < 100)){
		//move the null terminator to the next cell
		moveBuffer[i+1] = (String) "\0";
		moveBuffer = &parsedFrame;
		}

	}
}
	

