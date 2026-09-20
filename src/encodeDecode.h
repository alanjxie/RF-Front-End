#pragma once
#include <vector>
#include <map>
#include "Arduino.h"
#include <cstdint>
std::vector<uint8_t> encoder(const String& word, const std::map<char, std::vector<uint8_t>>& w2bdict);
String decoder(const std::vector<uint8_t> bitArray, const std::map<std::vector<uint8_t>, char> b2wdict);