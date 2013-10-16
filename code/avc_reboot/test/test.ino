#define pwm_a 10
#define pwm_b 11
#define dir_a 12
#define dir_b 13
#define zb_txPin 5
#define zb_rxPin 4


int incoming;
//a mag is right motor, stops at 45
int a_mag;
//b mag is left motor, stops at 55
int b_mag;
int a_dir;
int b_dir;

void setup(){
	a_mag=70;
	b_mag=70;

	pinMode(pwm_a, OUTPUT);  //Set control pins to be outputs
	pinMode(pwm_b, OUTPUT);
	pinMode(dir_a, OUTPUT);
	pinMode(dir_b, OUTPUT);
	Serial.begin(9600);
	digitalWrite(dir_a, HIGH);
	digitalWrite(dir_b, HIGH); 
	analogWrite(pwm_a, 255);
	analogWrite(pwm_b, 255);
}

void loop(){
	if (Serial.available() > 0){
		Serial.println("gotBytes:");
		incoming =(int) Serial.read();
		Serial.println(incoming);
		if (incoming==49){
			a_mag-=2;
		}
		if (incoming==50){
			b_mag-=2;
		}
	}

	delay(1000);
	analogWrite(pwm_a,a_mag);
	analogWrite(pwm_b,b_mag);
	Serial.print("Amag: ");
	Serial.print(a_mag);
	Serial.print(" : ");
	Serial.print(b_mag);
	Serial.println(" :Bmag\n");

}


