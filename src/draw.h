#pragma once 

#define WISHY 3  // Offset of the Y coordinats of the plate-wisher

// When in calibration mode, adjust the following factors until the servos move
// exactly 90 degrees
#define SERVOFAKTORLEFT 600
#define SERVOFAKTORRIGHT 600

// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are
// at all times parallel either to the X or Y axis
#define SERVOLEFTNULL 1950
#define SERVORIGHTNULL 835

#define SERVOPINLIFT 2
#define SERVOPINLEFT 3
#define SERVOPINRIGHT 4

#define ZOFF 115
// lift positions of lifting servo
#define LIFT0 1110 + ZOFF  // on drawing surface
#define LIFT1 995 + ZOFF   // between numbers
#define LIFT2 735 + ZOFF   // going towards sweeper

// speed of liftimg arm, higher is slower
#define LIFTSPEED 2000

// length of arms
#define L1 35
#define L2 55.1
#define L3 13.2
#define L4 45

// origin points of left and right servo
#define O1X 24
#define O1Y -25
#define O2X 49
#define O2Y -25


void initServo();
double return_angle(double a, double b, double c);
void drawTo(double pX, double pY);
void lift(char lift);
void number(float bx, float by, int num, float scale);