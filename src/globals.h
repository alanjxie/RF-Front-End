#include <vector>
#include "State.h"
#include <cstdint>

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
    std::vector<u_int8_t> readByteArray; //buffer for symbols
    String endWord = ""; //buffer/end result for words
    u_int8_t byte_buffer1;
    u_int8_t byte_buffer2;
    u_int8_t byte_buffer3;
    float raw1;
    float raw2;
};

