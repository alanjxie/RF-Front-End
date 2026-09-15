#include <Arduino.h>
#include <vector>
#include <map>

std::map<std::vector<uint8_t>, char> decodeDict = {

    // lowercase
    {{0b00, 0b00, 0b00}, 'a'},
    {{0b00, 0b00, 0b01}, 'b'},
    {{0b00, 0b00, 0b10}, 'c'},
    {{0b00, 0b00, 0b11}, 'd'},

    {{0b00, 0b01, 0b00}, 'e'},
    {{0b00, 0b01, 0b01}, 'f'},
    {{0b00, 0b01, 0b10}, 'g'},
    {{0b00, 0b01, 0b11}, 'h'},

    {{0b00, 0b10, 0b00}, 'i'},
    {{0b00, 0b10, 0b01}, 'j'},
    {{0b00, 0b10, 0b10}, 'k'},
    {{0b00, 0b10, 0b11}, 'l'},

    {{0b00, 0b11, 0b00}, 'm'},
    {{0b00, 0b11, 0b01}, 'n'},
    {{0b00, 0b11, 0b10}, 'o'},
    {{0b00, 0b11, 0b11}, 'p'},

    {{0b01, 0b00, 0b00}, 'q'},
    {{0b01, 0b00, 0b01}, 'r'},
    {{0b01, 0b00, 0b10}, 's'},
    {{0b01, 0b00, 0b11}, 't'},

    {{0b01, 0b01, 0b00}, 'u'},
    {{0b01, 0b01, 0b01}, 'v'},
    {{0b01, 0b01, 0b10}, 'w'},
    {{0b01, 0b01, 0b11}, 'x'},

    {{0b01, 0b10, 0b00}, 'y'},
    {{0b01, 0b10, 0b01}, 'z'},


    // uppercase
    {{0b01, 0b10, 0b10}, 'A'},
    {{0b01, 0b10, 0b11}, 'B'},

    {{0b01, 0b11, 0b00}, 'C'},
    {{0b01, 0b11, 0b01}, 'D'},
    {{0b01, 0b11, 0b10}, 'E'},
    {{0b01, 0b11, 0b11}, 'F'},

    {{0b10, 0b00, 0b00}, 'G'},
    {{0b10, 0b00, 0b01}, 'H'},
    {{0b10, 0b00, 0b10}, 'I'},
    {{0b10, 0b00, 0b11}, 'J'},

    {{0b10, 0b01, 0b00}, 'K'},
    {{0b10, 0b01, 0b01}, 'L'},
    {{0b10, 0b01, 0b10}, 'M'},
    {{0b10, 0b01, 0b11}, 'N'},

    {{0b10, 0b10, 0b00}, 'O'},
    {{0b10, 0b10, 0b01}, 'P'},
    {{0b10, 0b10, 0b10}, 'Q'},
    {{0b10, 0b10, 0b11}, 'R'},

    {{0b10, 0b11, 0b00}, 'S'},
    {{0b10, 0b11, 0b01}, 'T'},
    {{0b10, 0b11, 0b10}, 'U'},
    {{0b10, 0b11, 0b11}, 'V'},

    {{0b11, 0b00, 0b00}, 'W'},
    {{0b11, 0b00, 0b01}, 'X'},
    {{0b11, 0b00, 0b10}, 'Y'},
    {{0b11, 0b00, 0b11}, 'Z'},


    // numbers 1-9
    {{0b11, 0b01, 0b00}, '1'},
    {{0b11, 0b01, 0b01}, '2'},
    {{0b11, 0b01, 0b10}, '3'},
    {{0b11, 0b01, 0b11}, '4'},

    {{0b11, 0b10, 0b00}, '5'},
    {{0b11, 0b10, 0b01}, '6'},
    {{0b11, 0b10, 0b10}, '7'},
    {{0b11, 0b10, 0b11}, '8'},

    {{0b11, 0b11, 0b00}, '9'},


    // space
    {{0b11, 0b11, 0b01}, ' '}

    // {0b11, 0b11, 0b10} synch command
    // {0b11, 0b11, 0b11} stop command
};


std::map<char, std::vector<uint8_t>> encodeDict;
bool synchSent = false;
bool stopReceived = false;
std::vector<u_int8_t> readArray;
int phaseEncoderVar;
u_int8_t byte_buffer1;
u_int8_t byte_buffer2;
u_int8_t byte_buffer3;


void setup() {
  Serial.begin(115200);
    for (const auto& pair : decodeDict) { //reverse dictionary setup
        encodeDict[pair.second] = pair.first;
    }
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
      while (stopReceived == false) {
        float raw1 = analogRead(36);
        float raw2 = analogRead(39);
        float volt1 = raw1 * 3.3 / 4095.0;
        float volt2 = raw2 * 3.3 / 4095.0;
        byte_buffer1 = phaseEncoder(volt1, volt2);
        float raw1 = analogRead(36);
        float raw2 = analogRead(39);
        byte_buffer2 = phaseEncoder(volt1, volt2);
        float raw1 = analogRead(36);
        float raw2 = analogRead(39);
        byte_buffer3 = phaseEncoder(volt1, volt2);        
          delay(500);
        synchSent = true;
      }
    }

  }

}

//Assuming the negative sign is already implemented into Q

u_int8_t phaseEncoder(double I, double Q){
  if ((I - Q) == 1.414) {
    phaseEncoderVar = 45;
  } else if ((I - Q) == 0) {
    phaseEncoderVar = 135;
  } else if ((I - Q) == -1.414) {
    phaseEncoderVar == 225;
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
    std::vector<uint8_t> charBitArray = encodeDict[word[i]];
    fullBitArray.insert(fullBitArray.end(), charBitArray.begin(), charBitArray.end());
  };

  return fullBitArray;
}

String wordDecoder(std::vector<uint8_t> bitArray) {//transforms bits into word
  String endWord;
  std::vector<uint8_t> buffer;
  for (int i = 0; i < bitArray.size(); i += 3) {
    buffer = {bitArray[i], bitArray[i+1], bitArray[i+2]};
    endWord += decodeDict[buffer];
  };

  return endWord;
}