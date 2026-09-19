#pragma once
#include <vector>
#include <map>
#include <cstdint>

std::map<std::vector<uint8_t>, char> byteToWordDict();
std::map<char, std::vector<uint8_t>> wordToByteDict(std::map<std::vector<uint8_t>, char> decodeDict);