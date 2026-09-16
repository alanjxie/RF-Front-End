#pragma once
#include <vector>
#include <map>
#include <cstdint>

std::map<std::vector<uint8_t>, char> createDecodeDict();
std::map<char, std::vector<uint8_t>> createEncodeDict(std::map<std::vector<uint8_t>, char> decodeDict);