#include <wiringPi.h>
#include "wpmotor.h"

// See the wpmotor.h file for more information
// about the pin definitions

void setupPins()
{
	wiringPiSetup ();
	pinMode (IN_1, OUTPUT);
	pinMode (IN_2, OUTPUT);
	pinMode (IN_3, OUTPUT);
	pinMode (IN_4, OUTPUT);
	pinMode (ENABLE_1, OUTPUT);
	digitalWrite (ENABLE_1, HIGH);
	motorStop(500);
};

//Turns the motor to the left
void motorLeft(int t)
{
	//Step 1
	digitalWrite (IN_1, HIGH);
	digitalWrite (IN_2, HIGH);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, LOW);
	delay (t);
	//Step 2
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, HIGH);
	digitalWrite (IN_3, HIGH);
	digitalWrite (IN_4, LOW);
	delay (t);
	//Step 3
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, HIGH);
	digitalWrite (IN_4, HIGH);
	delay (t);
	//Step 4
	digitalWrite (IN_1, HIGH);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, HIGH);
	delay (t);
};

//Turns the motor to the right
void motorRight(int t)
{
	//Step 4
	digitalWrite (IN_1, HIGH);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, HIGH);
	delay (t);
	//Step 3
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, HIGH);
	digitalWrite (IN_4, HIGH);
	delay (t);
	//Step 2
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, HIGH);
	digitalWrite (IN_3, HIGH);
	digitalWrite (IN_4, LOW);
	delay (t);
	//Step 1
	digitalWrite (IN_1, HIGH);
	digitalWrite (IN_2, HIGH);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, LOW);
	delay (t);
};

//Stops motor
void motorStop(int t)
{
	//Write all zeros
	digitalWrite (IN_1, LOW);
	digitalWrite (IN_2, LOW);
	digitalWrite (IN_3, LOW);
	digitalWrite (IN_4, LOW);
	delay (t);
};

int main(void)
{
	setupPins();
	while (1)
	{
		motorLeft(500);
		//motorRight(500);
	}
	return 0;
}
