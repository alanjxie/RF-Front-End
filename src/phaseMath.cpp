#include "phaseMath.h"
#include "globals.h"
#include <Arduino.h>

void byteToIQ(uint8_t symbol) {
  // DAC takes counts (0..255), not volts. These approximate 0.943/2.357 V.
  const uint8_t low = 73;
  const uint8_t high = 182;
  switch (symbol) {
    case 0b00: dacWrite(glob.I_PIN, high); dacWrite(glob.Q_PIN, low); break;
    case 0b01: dacWrite(glob.I_PIN, low);  dacWrite(glob.Q_PIN, low); break;
    case 0b10: dacWrite(glob.I_PIN, low);  dacWrite(glob.Q_PIN, high); break;
    case 0b11: dacWrite(glob.I_PIN, high); dacWrite(glob.Q_PIN, high); break;
  }
  // Hold this output until the next symbol tick.
}

uint8_t iqToByte(double I, double Q) {
  // Compare each channel with its 1.65 V midpoint (Q is already inverted).
  if (I >= 1.65) return Q >= 1.65 ? 0b11 : 0b00;
  return Q >= 1.65 ? 0b10 : 0b01;
}
