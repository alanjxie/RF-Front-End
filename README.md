Hello! This is an RF e2e project. There's two main parts of this project:
1.) The protocol - timing of sending bits and receiving them.
2.) The analog portion - designing a pcb that can be duplicated.

The objective of this project is to gain full e2e knowledge of the RF transmitting/receiving pipeline, from modulation to sampling to bit mapping.


Clocking is now scheduled in `loop()` with `millis()` (no extra libraries):
- Sample period: 5 ms.
- Symbol period: 1000 ms, or 200 sample periods per symbol.
- Receive decision: average I/Q from 400 ms up to, but not including, 600 ms
  into each symbol (normally 40 samples). The rest are ignored for data decoding.

DAC outputs are GPIO 25 (I) and 26 (Q). Receiver ADC inputs default to GPIO 34
(I) and 35 (Q); change them in `src/globals.h` to match the analog hardware.
For a wired baseband check, connect transmitter 25 to receiver 34, transmitter
26 to receiver 35, and share ground. For RF, connect the demodulated I/Q outputs
to those ADC inputs. The current decision logic expects levels near 0.943 V and
2.357 V, centered on 1.65 V, with Q already inverted as in the original mapping.
Adjust levels/thresholds if the analog receiver produces different voltages.

The sender holds sync symbols `11, 11, 10` for one second each. The receiver
looks for two seconds of high I/high Q, then uses the transition to low I/high Q
to mark the start of the final sync symbol. Data starts one second later. Each
board then advances its own symbol schedule without requiring data transitions.
The stop group is `11, 11, 11`. Start listening before transmission; this simple
sync detector requires the full sync sequence and assumes a stable, clean signal.
