#include <wiringPi.h>
#include "wpmotor.h"
#include <stdlib.h>
#include <stdio.h>

// See the wpmotor.h file for more information
// about the pin definitions

int state_var = 0;

//#define DEBUG_MOTOR
//#define DEBUG_MOTOR_TURN
#define DELAY 5
#define TURN_DELAY 10

void setupPins()
{
	wiringPiSetup ();
	pinMode (IN_1, OUTPUT);
	pinMode (IN_2, OUTPUT);
	pinMode (IN_3, OUTPUT);
	pinMode (IN_4, OUTPUT);
	pinMode (ENABLE_1, OUTPUT);
	digitalWrite (ENABLE_1, HIGH);

	printf("Initializing the motor!\n\n");
	state_var = 1;
	digitalWrite(IN_1, HIGH);
	digitalWrite(IN_2, LOW);
	digitalWrite(IN_3, HIGH);
	digitalWrite(IN_4, LOW);

	motorStop(5000);
}

//Turns the motor to the left
void motorLeft(int t)
{
	state_var--;

	if(state_var < 1) {
#ifdef DEBUG_MOTOR
		printf("Illegal state of %d; resetting state_var to 4.\n", state_var);
#endif
		state_var = 4;
	}

#ifdef DEBUG_MOTOR_TURN
	printf("\nTurning motor left!\n");
#endif

	if(state_var < 1) {
#ifdef DEBUG_MOTOR
		printf("The state variable was less than 1; resetting to 4 for motorLeft function call\n");
#endif
		state_var = 4;
	}

#ifdef DEBUG_MOTOR
	printf("Entering left motor switch with state_var value of %d\n", state_var);
#endif

	switch(state_var) {
	case 4:
		//Step 4
		digitalWrite (IN_1, HIGH);
		digitalWrite (IN_2, LOW);
		digitalWrite (IN_3, LOW);
		digitalWrite (IN_4, HIGH);
		delay (TURN_DELAY);
		break;
		
	case 3:
		//Step 3
		digitalWrite (IN_1, LOW);
		digitalWrite (IN_2, HIGH);
		digitalWrite (IN_3, LOW);
		digitalWrite (IN_4, HIGH);
		delay (TURN_DELAY);
		break;
		
	case 2:
		//Step 2
		digitalWrite (IN_1, LOW);
		digitalWrite (IN_2, HIGH);
		digitalWrite (IN_3, HIGH);
		digitalWrite (IN_4, LOW);
		delay (TURN_DELAY);
		break;

	case 1:
		//Step 1
		digitalWrite (IN_1, HIGH);
		digitalWrite (IN_2, LOW);
		digitalWrite (IN_3, HIGH);
		digitalWrite (IN_4, LOW);
		delay (TURN_DELAY);
		break;

	default:
		printf("FATAL state_var ERROR with value of %d\n.  EXITING", state_var);
		exit(EXIT_FAILURE);
		break;
	}

#ifdef DEBUG_MOTOR
	printf("Current value of state_var after decrement: %d\n", state_var);
#endif

	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
	digitalWrite(IN_3, LOW);
	digitalWrite(IN_4, LOW);
	delay(t);
}

//Turns the motor to the right
void motorRight(int t)
{
	state_var++;

	if(state_var > 4) {
#ifdef DEBUG_MOTOR
	printf("Illegal value of state_var %d.  Resetting to 1 for motor right.\n", state_var);
#endif
		state_var = 1;
	}

#ifdef DEBUG_MOTOR_TURN
	printf("\nTurning motor right!\n");
#endif

	if(state_var < 1) {
#ifdef DEBUG_MOTOR
		printf("The state variable was less than 1; resetting to 1 for motor right function call\n");
#endif
		state_var = 1;
	}

#ifdef DEBUG_MOTOR
		 printf("Entering right motor switch with state_var value of: %d\n", state_var);
#endif

	switch(state_var) {

	case 1:
		//Step 1
		digitalWrite (IN_1, HIGH);
		digitalWrite (IN_2, LOW);
		digitalWrite (IN_3, HIGH);
		digitalWrite (IN_4, LOW);
		delay (TURN_DELAY);
		break;

	case 2:
		//Step 2
		digitalWrite (IN_1, LOW);
		digitalWrite (IN_2, HIGH);
		digitalWrite (IN_3, HIGH);
		digitalWrite (IN_4, LOW);
		delay (TURN_DELAY);
		break;

	case 3:
		//Step 3
		digitalWrite (IN_1, LOW);
		digitalWrite (IN_2, HIGH);
		digitalWrite (IN_3, LOW);
		digitalWrite (IN_4, HIGH);
		delay (TURN_DELAY);
		break;

	case 4:
		//Step 4
		digitalWrite (IN_1, HIGH);
		digitalWrite (IN_2, LOW);
		digitalWrite (IN_3, LOW);
		digitalWrite (IN_4, HIGH);
		delay (TURN_DELAY);
		break;

	default:
		printf("ILLEGAL state_var VALUE OF %d ON MOTOR TURN RIGHT; EXITING\n.", state_var);
		exit(EXIT_FAILURE);
		break;
	}

#ifdef DEBUG_MOTOR
	printf("Current value of state_var after increment: %d\n", state_var);
#endif

	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
	digitalWrite(IN_3, LOW);
	digitalWrite(IN_4, LOW);
	delay(t);
}

//Stops motor
void motorStop(int t)
{
#ifdef DEBUG_MOTOR
	printf("Stopping the motor!\n");
#endif
	//Write all zeros
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, LOW);
	delay (t);

	//state_var = 0;
}

int main(void)
{
	int i = 0;
	setupPins();
	while (1)
	{
		for(i = 0; i < 400; i++) {
			motorLeft(DELAY);
		}

		for(i = 0; i < 400; i++) {
			motorRight(DELAY);
		}
/*
		motorStop(DELAY);

		for(i = 0; i < 6; i++) {
			motorRight(DELAY);
		}

		for(i = 0; i < 6; i++) {
			motorLeft(DELAY);
		}

		motorStop(DELAY);

		for(i = 0; i < 6; i++) {
			motorRight(DELAY);
		}

		motorStop(DELAY);

		for(i = 0; i < 6; i++) {
			motorLeft(DELAY);
		}

		motorStop(DELAY);

		for(i = 0; i < 6; i++) {
			motorRight(DELAY);
			motorStop(DELAY);
		}

		for(i = 0; i < 6; i++) {
			motorLeft(DELAY);
			motorStop(DELAY);
		}

		for(i = 0; i < 6; i++) {
			motorLeft(DELAY);
			motorRight(DELAY);
		}
*/
	}
	return 0;
}
