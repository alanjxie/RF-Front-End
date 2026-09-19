#include <Arduino.h>
#include <vector>
#include <map>
#include "dictionaries.h"
#include "state.h"
#include "globals.h"
#include "phaseMath.h"

//declarations
GlobalState glob;
ReceiveState rx;
TransmitState tx;
std::map<std::vector<uint8_t>, char> decodeDict = createDecodeDict(); //decode map, bits to words
std::map<char, std::vector<uint8_t>> encodeDict = createEncodeDict(decodeDict); //encode map, words to bits


void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Are you reading or writing? Type READ or SEND. \n");
  if (Serial.available()) {
    String userChoice = Serial.readStringUntil('\n');
    if (userChoice == "READ") {
      Serial.println("You selected read.");
      glob.state = State::RX_SYNC;

    } else if (userChoice == "WRITE") {
      Serial.println("You selected write.");
    } else {
      Serial.println("Invalid configuration. Try again.");
    }
      if (rx.synchSent == false) {
        phaseDecoder(glob.I, glob.Q, 0x11);
        phaseDecoder(glob.I, glob.Q, 0x11);
        phaseDecoder(glob.I, glob.Q, 0x10); //sent synch!
      }
      //receive data...
      /*
      Going to have to rewrite this loop when clocked...3 iterations is ugly brosdf
      
      */
      while (rx.stopReceived == false) {
        //implement this jawn later
    }
    rx.synchSent = true;
  }
}

//Assuming the negative sign is already implemented into Q

int I_PIN = 25;
int Q_PIN = 26;
double I;
double Q;



std::vector<uint8_t> wordEncoder(String word){ //transforms words into bits
  std::vector<uint8_t> fullBitArray;
  for (int i = 0; i < word.length(); i++) {
    std::vector<uint8_t> charBitArray = encodeDict.at(word[i]);
    fullBitArray.insert(fullBitArray.end(), charBitArray.begin(), charBitArray.end());
  };

  return fullBitArray;
}

String wordDecoder(std::vector<uint8_t> bitArray) {//transforms bits into word
  String endWord;
  std::vector<uint8_t> buffer;
  for (int i = 0; i < bitArray.size(); i += 3) {
    buffer = {bitArray[i], bitArray[i+1], bitArray[i+2]};
    endWord += decodeDict.at(buffer);
  };

  return endWord;
}