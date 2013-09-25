// code for transmiting to XBee radio with particular addaress
// pins 4 and 5 are used for communication between Arduino and 
// XBee

#include <SoftwareSerial.h>

#define zb_txPin 5
#define zb_rxPin 4
#define N_ZB_FRAME 124    //  maximum size of the frame
int n_frame_to_ZB;        //  counter for the frame 
SoftwareSerial xBeeSerial(zb_rxPin, zb_txPin); // XBee constructor


char frame_to_ZB[N_ZB_FRAME]; // frame declaration
int button1;
int hex1 = 0;
int hex2 = 0;
int hex3 = 0;
int hex4 = 0;


void xbee_setup()
{
  Serial.begin(9600);
  xBeeSerial.begin(9600);
  pinMode(8,INPUT);
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
}


void xbee_loop()
{
  byte inByte;
  button3=digitalRead(13);
  button2=digitalRead(12);
  button1=digitalRead(11);
  button0=digitalRead(10);

  
  if(button0 == 1){
hex1 = 255;
  while ( xBeeSerial.available() > 0) // 
  {
    inByte = -1;
    inByte = xBeeSerial.read();
    Serial.print("rcvd: ");
    Serial.println(inByte,HEX);
  }
  delay(200);
  SendToZB0();
  hex1 = 0;
}}

 if(button1 == 1){
hex2 = 255;
  while ( xBeeSerial.available() > 0) // 
  {
    inByte = -1;
    inByte = xBeeSerial.read();
    Serial.print("rcvd: ");
    Serial.println(inByte,HEX);
  }
  delay(500);
  SendToZB1();
  hex2 = 0;
}}

 if(button2 == 1){
hex3 = 255;
  while ( xBeeSerial.available() > 0) // 
  {
    inByte = -1;
    inByte = xBeeSerial.read();
    Serial.print("rcvd: ");
    Serial.println(inByte,HEX);
  }
  delay(500);
  SendToZB2();
  hex3 = 0;
}}

 if(button3 == 1){
hex4 = 255;
  while ( xBeeSerial.available() > 0) // 
  {
    inByte = -1;
    inByte = xBeeSerial.read();
    Serial.print("rcvd: ");
    Serial.println(inByte,HEX);
  }
  delay(500);
  SendToZB3();
  hex4 = 0;
}}


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
  frame_to_ZB[17] = hex1;
  frame_to_ZB[18] = hex1;
  frame_to_ZB[19] = hex2;
  frame_to_ZB[20] = hex2;
  frame_to_ZB[21] = hex3;
  frame_to_ZB[22] = hex3;
  frame_to_ZB[23] = hex4;
  frame_to_ZB[24] = hex4;
  
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

