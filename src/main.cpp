#include <Arduino.h>
#include <vector>
#include <map>
#include "dictionaries.h"

std::map<std::vector<uint8_t>, char> decodeDict = createDecodeDict();
std::map<char, std::vector<uint8_t>> encodeDict = createEncodeDict(decodeDict);
bool synchSent = false;
bool stopReceived = false;
std::vector<u_int8_t> readByteArray;
String endWord = "";
int phaseEncoderVar;
u_int8_t byte_buffer1;
u_int8_t byte_buffer2;
u_int8_t byte_buffer3;
float raw1;
float raw2;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Are you reading or writing? Type READ or WRITE. \n");
  if (Serial.available()) {
    String state = Serial.readStringUntil('\n');
    if (state == "READ") {
      Serial.println("You selected read.");
      if (synchSent == false) {
        phaseDecoder(0x11);
        phaseDecoder(0x11);
        phaseDecoder(0x10); //sent synch!
      }
      //receive data...
      /*
      Going to have to rewrite this loop when clocked...3 iterations is ugly brosdf
      
      */
      while (stopReceived == false) {
        raw1 = analogRead(36); //read first raw voltage
        raw2 = analogRead(39); //read second raw voltage
        float volt1 = raw1 * 3.3 / 4095.0; //convert to actual voltage
        float volt2 = raw2 * 3.3 / 4095.0; // ^
        byte_buffer1 = phaseEncoder(volt1, volt2); //first byte
        float raw1 = analogRead(36);
        float raw2 = analogRead(39);
        byte_buffer2 = phaseEncoder(volt1, volt2); //second byte
        float raw1 = analogRead(36);
        float raw2 = analogRead(39);
        byte_buffer3 = phaseEncoder(volt1, volt2); //third byte     
        delay(500);

        readByteArray = {byte_buffer1, byte_buffer2, byte_buffer3};
        endWord += wordDecoder(readByteArray);
      }
      Serial.println(endWord);
      endWord = "";
      
    } else if (state == "WRITE") {
      Serial.println("You selected write.");
      //if ()
    }
  }
  synchSent = true;
}

//Assuming the negative sign is already implemented into Q

u_int8_t phaseEncoder(double I, double Q){
  if ((I - Q) == 1.414) {
    phaseEncoderVar = 45;
  } else if ((I - Q) == 0) {
    phaseEncoderVar = 135;
  } else if ((I - Q) == -1.414) {
    phaseEncoderVar = 225;
  } else {
    phaseEncoderVar = 315;
  }
  switch (phaseEncoderVar) {
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

int I_PIN = 25;
int Q_PIN = 26;
double I;
double Q;
void phaseDecoder(u_int8_t phase) { //virtual 0 at 1.65
  switch (phase) {
    case 0x00: //45
      I = 2.357;
      Q = 0.943;
      dacWrite(25, I_PIN);
      dacWrite(26, Q_PIN);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x01: //135
      I = 0.943;
      Q = 0.943;
      dacWrite(25, I_PIN);
      dacWrite(26, Q_PIN);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x10: //225
      I = 0.943;
      Q = 2.357;
      dacWrite(25, I_PIN);
      dacWrite(26, Q_PIN);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;

    case 0x11: //315
      I = 2.357;
      Q = 2.357;
      dacWrite(25, I_PIN);
      dacWrite(26, Q_PIN);
      delay(500);
      dacWrite(25, 1.65);
      dacWrite(26, 1.65);
      delay(500);
      break;
  }
}


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