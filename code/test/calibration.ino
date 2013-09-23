void calibrate(){
	//runs the binary serach, attempts to find the best trim value to equalise pulses
	testTrim = 0;
	min = -255;
	max = 255;
	moe = 5; //margin of error
	//TODO this needs to be modified in production code so it reads real pulses
	int motors[] = {128,168};
	//int motors[] = getPulses(motors);

	while ((motors[0] <= motors[0] + moe OR motors[0] >= motors[0] - moe) AND (motors[0] <= motors[0] + moe OR motors[0] >= motors[0] - moe)){
		compare = CompareMotors(motors);
		if (compare == 1){
			//motor 1 is too powerful, negitive trim
			max = testTrim;	
			testTrim = (int)(max-min)/2
		}
		else if (compare == -1){
			//motor 2 is too powerful, positive trim
			min = testTrim;
			testTrim = (int)(max-min)/2;
		}
		else{
			//motor pulses are the same, we're good.
			break;
		}
		controlTrim = testTrim;
		printf("MotorA: %d - %d : MotorB, Trim: %d \n", motors[0],motors[1], controlTrim);
		getPulses(motors);
	}
	zeroTrim = testTrim;
	}

}

int compareMotors(int motors[]){
	//motor comparitor, if m0 > m1 return 1, vice versa -1, ==  return 0
	if (motor[0] > motor[1]){
		return 1;
	}
	else if (motor[0] < motor[1]){
		return -1;
	}
	else{
		return 0;
	}
}

void getPulses(int motors[]){
	//TODO will need to be changed in production to read from interrupts
	//runs the motors at a given rate and records pulses, sets motors[0] and motors[1] as those pulses and returns.

	if (controlTrim > 0){
		motor[1] == motor[1] - testTrim;
	}
	else if (controlTrim < 0){
		motor[0] == motor[0] - testTrim;
	}
	else{
		//motors are all good, leave as is
	}
	return;
}

