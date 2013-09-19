const int spd_in = 0;
const int spd_out = 11;
const int ma1 = 7;
const int ma2 = 6;

void setup(){
Serial.begin(9600);
}
void loop(){
	int speed;
	speed = analogRead(spd_in);
	Serial.println(speed);
}
