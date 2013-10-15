// code for transmiting to XBee radio with particular addaress
// pins 4 and 5 are used for communication between Arduino and 
// XBee

#include <SoftwareSerial.h>

#define zb_txPin 5
#define zb_rxPin 4
#define N_ZB_FRAME 124    //  maximum size of the frame
int n_frame_to_ZB;        //  counter for the frame 
SoftwareSerial xBeeSerial(zb_rxPin, zb_txPin); // XBee constructor


char frame_to_ZB[N_ZB_FRAME]; // frame declaration
//set button names
int trimin; //analog values exceed hex value (hence trimin and trim)
int reset;
int forward;
int backward;
int left;
int right;
int trim;
int trigger = right + left + forward + backward;
int lastTrigger = 0;
int moveNumb;
int resetTrig = reset;
int lastResetTrig;
int trimReset;

void setup()
{
  Serial.begin(9600);
  xBeeSerial.begin(9600);
  pinMode(13, INPUT);//button imputs
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
}


void loop()
{
  byte inByte;
  right=digitalRead(13);
  left=digitalRead(12);
  backward=digitalRead(11);//which button is which
  forward=digitalRead(10);
  reset = digitalRead(9);
  trimin = analogRead(A0); 
  trigger = right + left + forward + backward;
  trim = trimin / 4; //because reads all the way to 1024
  
  if (right == 1)
  {
    moveNumb = 4;
  }
  else if (left == 1)
  {
    moveNumb = 3;
  }
  else if (backward == 1)
  {
    moveNumb = 2;
  }
  else if (forward == 1)
  {
    moveNumb = 1;
  }
  else if (reset == 1)
  {
    moveNumb = 6;
  }
  else
  {
    moveNumb = 0;
  } 
  
  //move send stuff
   if (trigger!= lastTrigger){
     if (trigger == 1) {
      while ( xBeeSerial.available() > 0)
    {
       inByte = -1;
       inByte = xBeeSerial.read();
       Serial.print("rcvd: ");
       Serial.println(inByte,HEX);
       break;
    }
    SendToZB0();
    }
    else{
          while ( xBeeSerial.available() > 0)
    {
       inByte = -1;
       inByte = xBeeSerial.read();
       Serial.print("rcvd: ");
       Serial.println(inByte,HEX);
       break;
    }
    SendToZB0();
}
lastTrigger = trigger;
   }
   
   //reset code
      if (resetTrig!= lastResetTrig){
     if (resetTrig == 1) {
    if (trimReset == trim) {
       trim = 0;
    }   
      while ( xBeeSerial.available() > 0)
    {
       inByte = -1;
       inByte = xBeeSerial.read();
       Serial.print("rcvd: ");
       Serial.println(inByte,HEX);
       break;
    }
    SendToZB0();
    }
lastResetTrig = resetTrig;
trimReset = trim;
   }}
//
// function to send data through XBee
void SendToZB0(){
  
  
  int j;
  unsigned int check_sum_total = 0;
  unsigned int CRC = 0;
  
   // delimiter
  frame_to_ZB[0] = 0x7E;
  // length
  frame_to_ZB[1] = 0x00;
  frame_to_ZB[2] = 0x16; // length
  //API ID
  frame_to_ZB[3] = 0x10; // tramsmission frame indicator
  frame_to_ZB[4] = 0x00;
  // destination 64 bit address - broadcast
  frame_to_ZB[5] = 0x00;
  frame_to_ZB[6] = 0x13;
  frame_to_ZB[7] = 0xA2;
  frame_to_ZB[8] = 0x00;
  frame_to_ZB[9] = 0x40;
  frame_to_ZB[10] = 0xA2;
  frame_to_ZB[11] = 0x65;
  frame_to_ZB[12] = 0x5A;
  // destination 16 bit address - broadcast
  frame_to_ZB[13] = 0xFF;
  frame_to_ZB[14] = 0xFE;
  // number of hops
  frame_to_ZB[15] = 0x00;
  //option 
  frame_to_ZB[16] = 0x01;
  //data
  frame_to_ZB[17] = 0xAA;
  frame_to_ZB[18] = moveNumb;
  frame_to_ZB[19] = trim;
  frame_to_ZB[20] = 0x00;
  frame_to_ZB[21] = 0x00;
  frame_to_ZB[22] = 0x00;
  frame_to_ZB[23] = 0x00;
  frame_to_ZB[24] = 0xBB;
  
  n_frame_to_ZB = 25;

  // add CRC to the packet
  for ( j = 3 ; j < n_frame_to_ZB ; j++){
    check_sum_total = check_sum_total + frame_to_ZB[j];
  }
  check_sum_total = check_sum_total & 0xFF;
  CRC = 0xFF - check_sum_total;
  //Serial.println(CRC);
  frame_to_ZB[n_frame_to_ZB] = CRC;
  n_frame_to_ZB = n_frame_to_ZB + 1;
  
  Serial.println("transmitting...");
  j = 0;
  while ( j < n_frame_to_ZB){
    xBeeSerial.write(frame_to_ZB[j]);
    Serial.print(frame_to_ZB[j],HEX);
    Serial.print(" ");
    j++;
  }
  Serial.println(" ");
  Serial.println("finished transmitting");

}
