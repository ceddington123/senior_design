#ifndef wpmotor_h_
#define wpmotor_h_

// Definitions for each of the WiringPi pins to the L293D chip
// http://pi.gadgetoid.com/pinout/wiringpi
// for the descriptions of each of the GPIO pins

#define IN_1 1 //Coil 1a
#define IN_2 4 //Coil 1b
#define IN_3 5 //Coil 2a
#define IN_4 6 //Coil 2b

#define ENABLE_1 0
#define MOTOR_DELAY 500

void setupPins();
void motorLeft(int t);
void motorRight(int t);
void motorStop(int t);

#endif
