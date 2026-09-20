#pragma once
enum class State {
    IDLE,
    TX_SYNC,
    RX_SYNC,
    RECEIVING,
    TRANSMITTING,
    COMPLETE,
    ERROR
};