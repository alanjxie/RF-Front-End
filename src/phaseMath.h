#pragma once
#include <cstdint>

void byteToIQ(uint8_t symbol);
uint8_t iqToByte(double I, double Q);
