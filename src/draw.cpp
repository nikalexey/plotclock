#include "draw.h"
#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;


void initServo() {
  servo1.attach(SERVOPINLIFT);
  servo2.attach(SERVOPINLEFT);
  servo3.attach(SERVOPINRIGHT);
}

double return_angle(double a, double b, double c) {
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

int servoLift = 1500;


volatile double lastX = 75;
volatile double lastY = 47.5;


void set_XY(double Tx, double Ty) {
  delay(1);
  double dx = Tx - O1X;
  double dy = Ty - O1Y;

  double c = sqrt(dx * dx + dy * dy);
  double a1 = atan2(dy, dx);
  double a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(
      floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  a2 = return_angle(L2, L1, c);
  double Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI);
  double Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, L4, c);

  servo3.writeMicroseconds(
      floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));
}

void drawTo(double pX, double pY) {
  double dx = pX - lastX;
  double dy = pY - lastY;
  double c = floor(7 * sqrt(dx * dx + dy * dy));

  if (c < 1) {
    c = 1;
  }

  for (int i = 0; i <= c; i++) {
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));
  }
  lastX = pX;
  lastY = pY;
}

void lift(char lift) {
  switch (lift) {
    case 0:  // 850
      if (servoLift >= LIFT0) {
        while (servoLift >= LIFT0) {
          servoLift--;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      } else {
        while (servoLift <= LIFT0) {
          servoLift++;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      }
      break;
    case 1:  // 150
      if (servoLift >= LIFT1) {
        while (servoLift >= LIFT1) {
          servoLift--;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      } else {
        while (servoLift <= LIFT1) {
          servoLift++;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      }
      break;
    case 2:
      if (servoLift >= LIFT2) {
        while (servoLift >= LIFT2) {
          servoLift--;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      } else {
        while (servoLift <= LIFT2) {
          servoLift++;
          servo1.writeMicroseconds(servoLift);
          delayMicroseconds(LIFTSPEED);
        }
      }
      break;
  }
}

void bogenUZS(float bx, float by, float radius, int start, int ende,
              float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
           radius * sin(start + count) + by);
    count += inkr;
  } while ((start + count) > ende);
}

void bogenGZS(float bx, float by, float radius, int start, int ende,
              float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
           radius * sin(start + count) + by);
    count += inkr;
  } while ((start + count) <= ende);
}

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals
// a 20 mm high font. The structure follows this principle: move to first
// startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {
  switch (num) {
    case 0:
      drawTo(bx + 12 * scale, by + 6 * scale);
      lift(0);
      bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
      lift(1);
      break;
    case 1:
      drawTo(bx + 3 * scale, by + 15 * scale);
      lift(0);
      drawTo(bx + 10 * scale, by + 20 * scale);
      drawTo(bx + 10 * scale, by + 0 * scale);
      lift(1);
      break;
    case 2:
      drawTo(bx + 2 * scale, by + 12 * scale);
      lift(0);
      bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
      drawTo(bx + 1 * scale, by + 0 * scale);
      drawTo(bx + 12 * scale, by + 0 * scale);
      lift(1);
      break;
    case 3:
      drawTo(bx + 2 * scale, by + 17 * scale);
      lift(0);
      bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
      bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
      lift(1);
      break;
    case 4:
      drawTo(bx + 10 * scale, by + 0 * scale);
      lift(0);
      drawTo(bx + 10 * scale, by + 20 * scale);
      drawTo(bx + 2 * scale, by + 6 * scale);
      drawTo(bx + 12 * scale, by + 6 * scale);
      lift(1);
      break;
    case 5:
      drawTo(bx + 2 * scale, by + 5 * scale);
      lift(0);
      bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
      drawTo(bx + 5 * scale, by + 20 * scale);
      drawTo(bx + 12 * scale, by + 20 * scale);
      lift(1);
      break;
    case 6:
      drawTo(bx + 2 * scale, by + 10 * scale);
      lift(0);
      bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
      drawTo(bx + 11 * scale, by + 20 * scale);
      lift(1);
      break;
    case 7:
      drawTo(bx + 2 * scale, by + 20 * scale);
      lift(0);
      drawTo(bx + 12 * scale, by + 20 * scale);
      drawTo(bx + 2 * scale, by + 0);
      lift(1);
      break;
    case 8:
      drawTo(bx + 5 * scale, by + 10 * scale);
      lift(0);
      bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
      bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
      lift(1);
      break;
    case 9:
      drawTo(bx + 9 * scale, by + 11 * scale);
      lift(0);
      bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
      drawTo(bx + 5 * scale, by + 0);
      lift(1);
      break;

    case 111:
      lift(0);
      drawTo(70, 45);
      drawTo(65 - WISHY, 43);

      drawTo(65 - WISHY, 46);
      drawTo(5, 49);
      drawTo(5, 46);
      drawTo(63 - WISHY, 46);
      drawTo(63 - WISHY, 42);

      drawTo(5, 42);
      drawTo(5, 38);
      drawTo(63 - WISHY, 38);
      drawTo(63 - WISHY, 34);

      drawTo(5, 34);
      drawTo(5, 29);
      drawTo(6, 29);
      drawTo(65 - WISHY, 26);

      drawTo(5, 26);
      drawTo(60 - WISHY, 40);

      drawTo(73.2, 44.0);
      lift(2);

      break;

    case 11:
      drawTo(bx + 5 * scale, by + 15 * scale);
      lift(0);
      bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(1);
      drawTo(bx + 5 * scale, by + 5 * scale);
      lift(0);
      bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
      delay(10);
      lift(1);
      break;
  }
}
