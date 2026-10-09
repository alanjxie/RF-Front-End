#include <Arduino.h>
#include <vector>
#include <map>
#include "dictionaries.h"
#include "State.h"
#include "globals.h"
#include "phaseMath.h"
#include "encodeDecode.h"

const std::map<std::vector<uint8_t>, char> b2wDict = byteToWordDict();
const std::map<char, std::vector<uint8_t>> w2bDict = wordToByteDict(b2wDict);
String serialLine;

void idleOutputs() {
  dacWrite(glob.I_PIN, 128);
  dacWrite(glob.Q_PIN, 128);
}

void handleCommand(const String& command) {
  if (glob.state != State::IDLE) {
    Serial.println("Busy. Wait until this message finishes.");
    return;
  }
  if (command == "READ") {
    rx = ReceiveState{};
    rx.sampleTick = millis();
    glob.state = State::RX_SYNC;
    Serial.println("Listening for sync...");
  } else if (command.startsWith("SEND ")) {
    String message = command.substring(5); // remove "SEND " to get the message
    for (unsigned int i = 0; i < message.length(); ++i) {
      if (w2bDict.find(message[i]) == w2bDict.end()) {
        Serial.println("Unsupported character. Use letters, 1-9, and spaces.");
        return;
      }
    }
    tx = TransmitState{};
    tx.symbols = {0b11, 0b11, 0b10}; // sync: held high/high for two seconds
    const auto payload = encoder(message, w2bDict);
    tx.symbols.insert(tx.symbols.end(), payload.begin(), payload.end());
    tx.symbols.insert(tx.symbols.end(), {0b11, 0b11, 0b11}); // stop
    tx.symbolStart = millis();
    glob.state = State::TRANSMITTING;
    byteToIQ(tx.symbols[0]);
    Serial.println("Sending...");
  } else {
    Serial.println("Type READ or SEND your message.");
  }
}

void transmitTick(uint32_t now) {
  if (now - tx.symbolStart < SYMBOL_PERIOD_MS) return;
  // If loop() misses an entire symbol, abort instead of emitting rushed symbols.
  if (now - tx.symbolStart >= 2 * SYMBOL_PERIOD_MS) {
    idleOutputs();
    glob.state = State::IDLE;
    Serial.println("Send stopped: missed a symbol deadline.");
    return;
  }
  tx.symbolStart += SYMBOL_PERIOD_MS;
  ++tx.symbolIndex;
  if (tx.symbolIndex == tx.symbols.size()) {
    idleOutputs();
    glob.state = State::IDLE;
    Serial.println("Sent. Type READ or SEND your message.");
  } else {
    byteToIQ(tx.symbols[tx.symbolIndex]);
  }
}

void finishReceivedSymbol() {
  if (rx.middleSamples == 0) {
    glob.state = State::IDLE;
    Serial.println("Receive stopped: missed the middle sampling window.");
    return;
  }
  // Average ONLY the samples collected near the center of the symbol.
  rx.readByteArray.push_back(iqToByte(rx.iSum / rx.middleSamples,
                                    rx.qSum / rx.middleSamples));
  rx.iSum = 0;
  rx.qSum = 0;
  rx.middleSamples = 0;
  if (rx.readByteArray.size() < 3) return;

  if (rx.readByteArray == std::vector<uint8_t>{0b11, 0b11, 0b11}) {
    Serial.print("Received: ");
    Serial.println(rx.endWord);
    glob.state = State::IDLE;
  } else if (b2wDict.find(rx.readByteArray) != b2wDict.end()) {
    rx.endWord += decoder(rx.readByteArray, b2wDict);
  } else {
    Serial.println("Receive stopped: invalid character group.");
    glob.state = State::IDLE;
  }
  rx.readByteArray.clear();
}

void receiveTick(uint32_t now) {
  if (now - rx.sampleTick < SAMPLE_PERIOD_MS) return;
  // Keep the sample schedule; don't invent samples if a tick was missed.
  rx.sampleTick += ((now - rx.sampleTick) / SAMPLE_PERIOD_MS) * SAMPLE_PERIOD_MS;

  if (glob.state == State::RECEIVING) {
    if (now - rx.symbolStart >= 2 * SYMBOL_PERIOD_MS) {
      glob.state = State::IDLE;
      Serial.println("Receive stopped: missed a symbol deadline.");
      return;
    }
    if (now - rx.symbolStart >= SYMBOL_PERIOD_MS) {
      if (rx.finishingSync) {
        rx.finishingSync = false; // final sync symbol has ended; data starts here
      } else {
        finishReceivedSymbol();
      }
      rx.symbolStart += SYMBOL_PERIOD_MS;
      if (glob.state != State::RECEIVING) return;
    }
  }

  glob.I = analogReadMilliVolts(glob.I_RX_PIN) / 1000.0;
  glob.Q = analogReadMilliVolts(glob.Q_RX_PIN) / 1000.0;

  if (glob.state == State::RX_SYNC) {
    // Reject the neutral idle level while looking for sync.
    const bool syncHigh = glob.I > 2.0 && glob.Q > 2.0;
    const bool syncLast = glob.I < 1.3 && glob.Q > 2.0;
    if (syncHigh) {
      if (!rx.trackingSync) {
        rx.trackingSync = true;
        rx.syncStart = now;
      }
    } else {
      // Detect the start of sync's third symbol. Data begins one second later.
      if (rx.trackingSync && syncLast &&
          now - rx.syncStart >= 2 * SYMBOL_PERIOD_MS - 2 * SAMPLE_PERIOD_MS) {
        rx.symbolStart = now;
        rx.finishingSync = true;
        glob.state = State::RECEIVING;
        Serial.println("Synced.");
      }
      rx.trackingSync = false;
    }
    return;
  }

  const uint32_t position = now - rx.symbolStart;
  // Ignore the last sync symbol; collect only the middle 200 ms of data symbols.
  if (!rx.finishingSync && position >= MIDDLE_START_MS && position < MIDDLE_END_MS) {
    rx.iSum += glob.I;
    rx.qSum += glob.Q;
    ++rx.middleSamples;
  }
}

void setup() {
  Serial.begin(115200);
  analogSetPinAttenuation(glob.I_RX_PIN, ADC_11db);
  analogSetPinAttenuation(glob.Q_RX_PIN, ADC_11db);
  idleOutputs();
  Serial.println("Type READ or SEND your message. Start READ before SEND.");
}

void loop() {
  // Read a line without readStringUntil() blocking either clock.
  while (Serial.available()) {
    const char c = Serial.read();
    if (c == '\n') {
      handleCommand(serialLine);
      serialLine = "";
    } else if (c != '\r') {
      serialLine += c;
    }
  }
  const uint32_t now = millis();
  if (glob.state == State::TRANSMITTING) transmitTick(now); 
  if (glob.state == State::RX_SYNC || glob.state == State::RECEIVING) receiveTick(now);
}
