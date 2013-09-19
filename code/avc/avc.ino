#include <stdarg.h>
#include <stdio.h>



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





//motor trim value
int trim = 0;
String moveBuffer[] = {"f255", "b255","l128","r128"};

int pwm_a = 3;
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
}

void calibrateTrim(){
  //binary search throuhg trim values till wheel pulses match
  //TODO
}

void setMove(char dir, int mag){
  //sets the motor values 
  int mag_a, mag_b;
  //is bool right type? TODO: check this out
  bool a_dir, b_dir;

  if (trim > 0){
	  //bias towards right
	  mag_a = mag;
	  mag_b = mag - trim;
  }
  else if (trim < 0){
	  //bias towards left
	  mag_a = mag + trim;
	  mag_b = mag;
  }
  else{
	  mag_a = mag;
	  mag_b = mag;
  }

  switch(dir){
	  case 1:
		  //forward
		  a_dir = HIGH;
		  b_dir = HIGH;
		  break;
	  case 2:
		  //backward
		  a_dir = LOW;
		  b_dir = LOW;
		  break;
	  case 3:
		  //left
		  a_dir = LOW;
		  b_dir = HIGH;
		  break;
	  case 4:
		  //right
		  a_dir = HIGH;
		  b_dir = LOW;
		  break;
	  case 5:
		  //stop
		  //TODO
		  1+1;
  }
  /* analogWrite(pwm_a, mag_a);
   * analogWrite(pwm_b, mag_b);
   * digitalWrite(dir_a, a_dir);
   * digitalWrite(dir_b, b_dir); */

  printf("A mag: %d, B mag: %d, Left : %i%i : Right \n", mag_a, mag_b, a_dir, b_dir);
}


void parseMove(String command,int commandOut[]){
  //interprets commands recieved by the xbee and sends them to setMove()
  /* Directions:
   * Forward = 1
   * Backward= 2 
   * Left    = 3
   * Right   = 4
   * Stop/err= 5
   *
   * Maximum magnitude is 255 */
	commandOut[0] = 5;

	//find the move code
	if      (command.substring(0,1) == "f") {commandOut[0] = 1;}
	else if (command.substring(0,1) == "b") {commandOut[0] = 2;}
	else if (command.substring(0,1) == "l") {commandOut[0] = 3;}
	else if (command.substring(0,1) == "r") {commandOut[0] = 4;}
	//shouldn't happen, but if all else fails set move as error
	else 			    {commandOut[0] = 5;}

	//find the magnitude, convert to an int
	int len = command.length();
	String mag;
	mag = command.substring(1);
	Serial.println(mag);
	char mags[len+1];
	mag.toCharArray(mags,len+1);
	commandOut[1] = atoi(mags);
}


void loop(){
	for (int i = 0; i <= (sizeof(moveBuffer)/7)-1; i++){
		int commands[2];
		parseMove(moveBuffer[i],commands);
		setMove(commands[0],commands[1]);

		delay(1500);
	}
}
