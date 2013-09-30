#include "TimerOne.h"
//Timer example ganked from http://playground.arduino.cc/code/timer1
 
void setup()
{
  pinMode(10, OUTPUT);
  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  Serial.begin(9600);
}
 
void callback()
{
  Serial.println("ping!");
}
 
void loop()
{
  Serial.println("DELAY");
  delay(100000);
}

