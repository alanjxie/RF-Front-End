#include "dictionaries.h"
std::map<std::vector<uint8_t>, char> createDecodeDict() {
    std::map<std::vector<uint8_t>, char> decodeDict;
    decodeDict = {
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
    return decodeDict;
}


std::map<char, std::vector<uint8_t>> createEncodeDict (const std::map<std::vector<uint8_t>, char> decodeDict){
    std::map<char, std::vector<uint8_t>> encodeDict;
    for (const auto& pair : decodeDict) { //reverse dictionary setup
        encodeDict[pair.second] = pair.first;
    }
    return encodeDict;
}

