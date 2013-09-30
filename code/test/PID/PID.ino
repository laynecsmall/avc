#include<PID_v1.h>

//PID variables
double Setpoint, Input, Output;

PID myPID(&Input, &Output, &Setpoint, 2,4,1, DIRECT);

void setup(){

	Setpoint=50;
	myPID.SetMode(AUTOMATIC);
	Serial.begin(9600);

}

void loop(){
	myPID.Compute();
	Serial.println(Output);


}
