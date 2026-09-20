#pragma once
#include <vector>
#include "State.h"
#include <cstdint>
#include "Arduino.h"
struct GlobalState {
    double I;
    double Q;
    int I_PIN = 25;
    int Q_PIN = 26;
    State state;
};

struct TransmitState {
    bool synchReceived = false; //synch flag
    bool stopSent = false;
};

struct ReceiveState {
    bool stopReceived = false; //stop flag
    bool synchSent = false; //synch flag
    std::vector<uint8_t> readByteArray; //buffer for symbols
    String endWord = ""; //buffer/end result for words
    uint8_t byte_buffer1;
    uint8_t byte_buffer2;
    uint8_t byte_buffer3;
    float raw1;
    float raw2;
};


extern GlobalState glob;
extern ReceiveState rx;
extern TransmitState tx;
