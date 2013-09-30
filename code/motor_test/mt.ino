void setup(){
	
	int pwm_a = 3;
	int pwm_b = 11;
	int dir_a = 12;
	int dir_b = 13;

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
	}


