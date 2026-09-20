#include <Arduino.h>
#include <vector>
#include <map>
#include "dictionaries.h"
#include "State.h"
#include "globals.h"
#include "phaseMath.h"
#include "encodeDecode.h"

//declarations
const std::map<std::vector<uint8_t>, char> b2wDict = byteToWordDict(); //decode map, bits to words
const std::map<char, std::vector<uint8_t>> w2bDict = wordToByteDict(b2wDict); //encode map, words to bits
State state = State::IDLE;


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
        byteToIQ(0x11);
        byteToIQ(0x11);
        byteToIQ(0x10); //sent synch!
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
