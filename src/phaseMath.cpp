#include "phaseMath.h"
#include "globals.h";
#include <Arduino.h>;

void phaseDecoder(double I, double Q, uint8_t phase) { //takes symbol, computes I and Q, and writes to DAC
  switch (phase) {
    case 0x00: //45
      I = 2.357;
      Q = 0.943;
      dacWrite(25, I);
      dacWrite(26, Q);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x01: //135
      I = 0.943;
      Q = 0.943;
      dacWrite(25, I);
      dacWrite(26, Q);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x10: //225
      I = 0.943;
      Q = 2.357;
      dacWrite(25, I);
      dacWrite(26, Q);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x11: //315
      I = 2.357;
      Q = 2.357;
      dacWrite(25, I);
      dacWrite(26, Q);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;
  }
}

u_int8_t phaseEncoder(double I, double Q, int phaseAngle){ //takes I and Q, computes phase, returns symbol
  if ((I - Q) == 1.414) {
    phaseAngle = 45;
  } else if ((I - Q) == 0) {
    phaseAngle = 135;
  } else if ((I - Q) == -1.414) {
    phaseAngle = 225;
  } else {
    phaseAngle = 315;
  }
  switch (phaseAngle) {
    case 45:
      return 0x00;
      break;

    case 135:
      return 0x01;
      break;

    case 225:
      return 0x10;
      break;

    case 315:
      return 0x11;
      break;
  }
  return 0;
}