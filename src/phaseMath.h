#pragma once
#include <cstdint>

void byteToIQ(uint8_t phase);
uint8_t iqToByte(double I, double Q, int phase);


