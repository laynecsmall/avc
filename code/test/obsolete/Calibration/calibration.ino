void calibrate(int zt, int ct){
	//runs the binary serach, attempts to find the best trim value to equalise pulses

	//TODO: this may cause problems in production as arguments are passed by value, not address
	int zeroTrim = zt;
	int controlTrim = ct;

	int testTrim = 0;
	int min = -255;
	int max = 255;
	int moe = 2; //margin of error
	int motors[] = {0,0};
	//TODO this needs to be modified in production code so it reads real pulses
	getPulses(motors,controlTrim,testTrim);

	while (!((motors[0] >= motors[1] - moe) && (motors[0] <= motors[1] + moe))){
		int compare = compareMotors(motors);
		if (compare == 1){
			//motor 0 is too powerful, negitive trim
			max = testTrim;	
			testTrim = (int)(max+min)/2;
		}
		else if (compare == -1){
			//motor 1 is too powerful, positive trim
			min = testTrim;
			testTrim = (int)(max+min)/2;
		}
		else{
			//motor pulses are the same, we're good.
			break;
		}
		controlTrim = testTrim;
		printf("MotorA: %d - %d : MotorB, Trim: %d \n", motors[0],motors[1], controlTrim);
		getPulses(motors,controlTrim, testTrim);
	}
	zeroTrim = testTrim;
	printf("Calibration done");
	return;
	}

int compareMotors(int motors[]){
	//motor comparitor, if m0 > m1 return 1, vice versa -1, ==  return 0
	if (motors[0] > motors[1]){
		return 1;
	}
	else if (motors[0] < motors[1]){
		return -1;
	}
	else{
		return 0;
	}
}

void getPulses(int motors[], int controlTrim, int testTrim){
	//TODO will need to be changed in production to read from interrupts
	//runs the motors at a given rate and records pulses, sets motors[0] and motors[1] as those pulses and returns.
	motors[0] = 188;
	motors[1] = 148;

	if (controlTrim > 0){
		motors[1] = motors[1] - testTrim;
	}
	else if (controlTrim < 0){
		motors[0] = motors[0] + testTrim;
	}
	else{
		//motors are all good, leave as is
	}
	return;
}

