#pragma once
#include <vector>
#include "State.h"
#include <cstdint>
#include "Arduino.h"

// Both boards use these periods: 200 sample ticks per symbol tick.
constexpr uint32_t SAMPLE_PERIOD_MS = 5;
constexpr uint32_t SYMBOL_PERIOD_MS = 1000;
constexpr uint32_t MIDDLE_START_MS = 400;
constexpr uint32_t MIDDLE_END_MS = 600;

struct GlobalState {
    double I = 0;
    double Q = 0;
    int I_PIN = 25;       // DAC outputs
    int Q_PIN = 26;
    int I_RX_PIN = 34;    // ADC inputs; change to match your wiring
    int Q_RX_PIN = 35;
    State state = State::IDLE;
};

struct TransmitState {
    std::vector<uint8_t> symbols;
    size_t symbolIndex = 0;
    uint32_t symbolStart = 0;
};

struct ReceiveState {
    std::vector<uint8_t> readByteArray;
    String endWord;
    uint32_t sampleTick = 0;
    uint32_t symbolStart = 0;
    uint32_t syncStart = 0;
    bool trackingSync = false;
    bool finishingSync = false;
    double iSum = 0;
    double qSum = 0;
    uint16_t middleSamples = 0;
};

extern GlobalState glob;
extern ReceiveState rx;
extern TransmitState tx;
