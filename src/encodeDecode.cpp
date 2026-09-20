#include "encodeDecode.h"
#include <vector>
#include <map>
#include <cstdint>
#include "Arduino.h"


std::vector<uint8_t> encoder(const String& word, const std::map<char, std::vector<uint8_t>>& dict){ //transforms words into bits
  std::vector<uint8_t> fullBitArray;
  for (int i = 0; i < word.length(); i++) {
    const std::vector<uint8_t> charBitArray = dict.at(word[i]);
    fullBitArray.insert(fullBitArray.end(), charBitArray.begin(), charBitArray.end());
  };

  return fullBitArray;
}

String decoder(const std::vector<uint8_t>& bitArray, const std::map<std::vector<uint8_t>, char>& dict) {//transforms bits into word
  String endWord;
  std::vector<uint8_t> buffer;
  for (int i = 0; i < bitArray.size(); i += 3) {
    buffer = {bitArray[i], bitArray[i+1], bitArray[i+2]};
    endWord += dict.at(buffer);
  };

  return endWord;
}