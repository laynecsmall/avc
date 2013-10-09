#include <stdarg.h>
#include <stdio.h>
#include <TimerOne.h>
//grabbed from http://playground.arduino.cc/Main/Printf
//provides printf functionality
static FILE uartout = {0} ;
// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar (char c, FILE *stream)
{
    Serial.write(c) ;
    return 0 ;
}

void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
        Serial.print(tmp);
}
//printf stuff ends




//define constants
//	define modes
const int TEST = 0;
const int PROGRAMMED = 1;
const int REMOTE = 2 ;
const int LINE = 3;
const int MAZE = 4;
//	define movement commands
const int STOP = 0;
const int FOREWARD = 1;
const int BACKWARD = 2;
const int LEFT = 3;
const int RIGHT = 4;
const int TRIM = 5;
const int RESET = 6;
const int CALIBRATE = 7;

//motor trim value
int controlTrim = 0;
int zeroTrim = 0;
//which mode are we in?
int mode = PROGRAMMED;
//sample move commands, should be overwritten by real ones in relevant modes.
String moveBuffer[100];
//wheel counts counting pulses of the opto interupts. Current is the one currently being incremented.
int wheelCount[2] = {0,0};
int currentWheelCount[2] = {0,0};
int timer;

int pwm_a = 10;
int pwm_b = 11;
int dir_a = 12;
int dir_b = 13;

void setup(){
   // More printf stuff
   // fill in the UART file descriptor with pointer to writer.
   fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);

   // The uart is the standard output device STDOUT.
   stdout = &uartout ;
   //printf stuff ends
	
  pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);
  Serial.begin(9600);

  //set interupts
  Timer1.initialize(200000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(timeout);  // attaches callback() as a timer overflow interrupt
  

  //figure out what mode we're in/what we're doing
  switch (mode){
  	case REMOTE:
		{
		int frame[25];
		xbeeSetup();
		break;
		}

	case PROGRAMMED:
		{
		String newBuff[] =  {"f255", "b255","l128","r128"};
		for (int i = 0; i < sizeof(newBuff)/sizeof(newBuff[0])-1;i++){
			moveBuffer[i] = newBuff[i];
		}
		moveBuffer[sizeof(newBuff)/sizeof(newBuff[0])] = "\0";
		//String moveBuffer[] = {"f255"};
		break;
		}
	}
}

void timeout(){
	//timeout callback from timed interrupt. should be called every .2 seconds 
	
	//push wheel counts to store, reset current counter
	if (timer % 5 == 0){
		//one second has passed, push totals to array
		wheelCount[0] = currentWheelCount[0];
		wheelCount[1] = currentWheelCount[1];
		timer++;
	}


}

void leftWheelHit(){
	//callback from interupt - left wheel
	currentWheelCount[0]++;
	delay(2);
}
void rightWheelHit(){
	//callback from interupt - right wheel
	currentWheelCount[1]++;
	delay(2);
}

void setMove(int dir, int mag){
  //sets the motor values 
  int mag_a, mag_b;
  //is bool right type? TODO: check this out
  bool a_dir, b_dir;

  if (controlTrim > 0){
	  //bias towards right
	  mag_a = mag;
	  mag_b = mag - controlTrim;
  }
  else if (controlTrim < 0){
	  //bias towards left
	  mag_a = mag + controlTrim;
	  mag_b = mag;
  }
  else{
	  mag_a = mag;
	  mag_b = mag;
  }

  switch(dir){
	  case FOREWARD:
		  a_dir = HIGH;
		  b_dir = HIGH;
		  break;
	  case BACKWARD:
		  //backward
		  a_dir = LOW;
		  b_dir = LOW;
		  break;
	  case LEFT:
		  //left
		  a_dir = LOW;
		  b_dir = HIGH;
		  break;
	  case RIGHT:
		  //right
		  a_dir = HIGH;
		  b_dir = LOW;
		  break;
	  case STOP:
		  mag_a = 0;
		  mag_b = 0;
		  break;
	case TRIM:
		  controlTrim = controlTrim + mag;
		  break;
	case RESET:
		  controlTrim = zeroTrim;
		  break;
	case CALIBRATE:
		  calibrate(zeroTrim, controlTrim);
		  break;
  }
   analogWrite(pwm_a, mag_a);
   analogWrite(pwm_b, mag_b);
   digitalWrite(dir_a, a_dir);
   digitalWrite(dir_b, b_dir); 


  printf("A mag: %d, B mag: %d, Left : %i%i : Right \n", mag_a, mag_b, a_dir, b_dir);
}


void parseMove(String command,int commandOut[]){
  //interprets commands recieved by the xbee and sends them to setMove()
  /* Directions:
   *
   * Stop/err= 0
   * Forward = 1
   * Backward= 2 
   * Left    = 3
   * Right   = 4
   * Trim    = 5
   * Reset   = 6
   *
   * Maximum magnitude is 255 */
	commandOut[0] = 5;

	//find the move code
	if      (command.substring(0,1) == "f") {commandOut[0] = FOREWARD;}
	else if (command.substring(0,1) == "b") {commandOut[0] = BACKWARD;}
	else if (command.substring(0,1) == "l") {commandOut[0] = LEFT;}
	else if (command.substring(0,1) == "r") {commandOut[0] = RIGHT;}
	else if (command.substring(0,1) == "s") {commandOut[0] = STOP;}
	else if (command.substring(0,1) == "t") {commandOut[0] = TRIM;}
	else if (command.substring(0,1) == "r") {commandOut[0] = RESET;}
	else if (command.substring(0,1) == "c") {commandOut[0] = CALIBRATE;}
	//shouldn't happen, but if all else fails set move as error
	else 			    {commandOut[0] = 5;}

	//find the magnitude, convert to an int
	int len = command.length();
	String mag;
	mag = command.substring(1);
	char mags[len+1];
	mag.toCharArray(mags,len+1);
	commandOut[1] = atoi(mags);
}


void loop(){
	switch(mode){
		case TEST:
			{
			//test mode
			break;
			}

		case REMOTE:
			{
			byte remoteDirection;
  			byte remoteTrim;
  
				 if ( xBeeSerial.available() > 0){
   				val = xBeeSerial.read();

    					if(val ==  0xAA){
    						remoteDirection = xBeeSerial.read();
						 //Serial.println(inByte,HEX);
						 remoteTrim = xBeeSerial.read();    
						 //Serial.println(inByte2, HEX);
					  }
					  	setMove(int remoteDirection, 0)
				{
				}
				 }
			
			break;
			}
	
		case PROGRAMMED:{
					//iterate through the moveBuffer, loop when you find the null char
			for (int i = 0; i <= (sizeof(moveBuffer)/7)-1; i++){
				int commands[2];
				if (moveBuffer[i] != "\0"){
					parseMove(moveBuffer[i],commands);
					setMove(commands[0],commands[1]);

					delay(1500);
				}
				else{
					break;
				}
			}
		}
	}
}
