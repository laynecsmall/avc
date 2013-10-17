#include <QTRSensors.h>
#include <PID_v1.h>

#define pwm_a 10
#define pwm_b 11
#define dir_a 12
#define dir_b 13

#define FOREWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define STOP 0

int incoming[4];
//a mag is right motor, stops at 45
int a_mag;
//b mag is left motor, stops at 55
int b_mag;
int a_dir;
int b_dir;

//robot tunings
int front_speed = 200; //how fast should go forwrd, before correction
int front_delay = 200; //how long between calculating next move
int back_speed = 255;
int back_delay = 200;
int bias = 68; //constant to correct for wheel difarential

//robot pid tuning values
float Kp = 0.013;
float Ki = 0.05;
float Kd = 0.0023;


//best values found so far:
//
//int front_speed = 200; //how fast should go forwrd, before correction
//int front_delay = 200; //how long between calculating next move
//int back_speed = 255;
//int back_delay = 200;
//int bias = 68; //constant to correct for wheel difarential

////robot pid tuning values
//float Kp = 0.013;
//float Ki = 0.05;
//float Kd = 0.0023;


//PID variables
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//QTR setup
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 2 through 9, 10-13 are motor pins
QTRSensorsRC qtrrc((unsigned char[]) {2, 3, 4, 5, 6, 7, 8, 9},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void setup(){
	//line sensor setup
	qtr_calibrate();

	//motor setup
	pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a, OUTPUT);
	pinMode(dir_b, OUTPUT);
	Serial.begin(9600);

	//pid setup
	Setpoint=3500;
	myPID.SetOutputLimits(-255.0,255.0);
	myPID.SetMode(AUTOMATIC);


}
void qtr_calibrate(){
delay(500);
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
  setMove(FOREWARD,128,0,-70);
	
}
void setMove(int dir, int mag, int trim, int bias){
	//TODO trim
	if (bias != 0){
		trim = trim + bias;
	}

	if (trim !=0){
			a_mag=mag+trim;
			b_mag=mag-trim;
		}
	else{
		a_mag=mag;
		b_mag=mag;
		}

	switch(dir){
		case FOREWARD:{
		     a_dir=HIGH;
		     b_dir=HIGH;
		     break;
		}
		case BACKWARD:{
		     a_dir=LOW;
		     b_dir=LOW;
		     break;
		}
		case LEFT:{
		     a_dir=LOW;
		     b_dir=HIGH;
		     break;
		}
		case RIGHT:{
		     a_dir=LOW;
		     b_dir=HIGH;
		     break;
		}
		case STOP:{
		     a_dir=HIGH;
		     b_dir=HIGH;
		     a_mag=0;
		     b_mag=0;
		     break;
		}
	}

	if (a_mag>255){a_mag=255;}
	if (b_mag>255){b_mag=255;}
	if (a_mag<0){a_mag=0;}
	if (b_mag<0){b_mag=0;}

	digitalWrite(dir_a, a_dir);
	digitalWrite(dir_b, b_dir); 
	analogWrite(pwm_a, a_mag);
	analogWrite(pwm_b, b_mag);

	Serial.print("Amag: ");
	Serial.print(a_mag);
	Serial.print(" : ");
	Serial.print(b_mag);
	Serial.println(" :Bmag\n");
	Serial.print("Adir: ");
	Serial.print(a_dir);
	Serial.print(" : ");
	Serial.print(b_dir);
	Serial.println(" :Bdir\n");
	Serial.println("==============");
	
}


void loop(){

	Input = qtrrc.readLine(sensorValues);
	if (Input == 0 || Input > 7000){
		setMove(BACKWARD,back_speed,0,bias);
		delay(back_delay);
	}
	else{
		myPID.Compute();
		Serial.println(Input);
		setMove(FOREWARD,front_speed,Output,bias);
		Serial.print("==Output:");
		Serial.println(Output);
		delay(front_delay);
	}
}



