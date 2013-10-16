#define pwm_a 10
#define pwm_b 11
#define dir_a 12
#define dir_b 13

#define FOREWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define STOP 0
//check/cite code from: http://www.instructables.com/id/Arduino-based-line-follower-using-Pololu-QTR-8RC-l/?ALLSTEPS

int incoming[4];
int intrim=0;
char sign='+';
//a mag is right motor, stops at 45
int a_mag;
//b mag is left motor, stops at 55
int b_mag;
int a_dir;
int b_dir;
void setup(){

	pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a, OUTPUT);
	pinMode(dir_b, OUTPUT);
	Serial.begin(9600);
	setMove(FOREWARD,128,intrim);
}

void setMove(int dir, int mag, int trim){
	//TODO trim

	if (trim !=0){
		if (trim<0){
			a_mag=mag-trim;
			b_mag=mag+trim;
			
			}
		if (trim>0){
			a_mag=mag+trim;
			b_mag=mag-trim;
			}
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
	 while ( xBeeSerial.available() > 0){        
	 val = xBeeSerial.read();

		if(val ==  0xAA){
			remoteDirection = xBeeSerial.read();
			 Serial.println(remoteDirection,HEX);
			 remoteTrim = xBeeSerial.read();    
			 //Serial.println(remoteTrim, HEX);
			 int remote = remoteDirection;
				}
		  }
		if(remoteDirection<8){
		  setMove(remoteDirection, 128,remoteTrim);	
		}
}



