# si4713_adafruit2

> **⚠️ Project Status: CLOSED — No further development**

This project is no longer maintained and will not be continued.  
The codebase requires more extensive refactoring than is practical to pursue here.

---

## Looking for a working Si4713 library?

A fork of the **Adafruit Si4713 Library** with bug fixes and extended functionality
implemented according to the Silicon Labs datasheet is available here:

**[https://github.com/yo3hjv/Adafruit-Si4713-Library](https://github.com/yo3hjv/Adafruit-Si4713-Library)**

### What the fork includes

- **Bug fixes**
  - `begin()` order corrected: hardware reset is now performed *before* the I2C
    address scan, eliminating intermittent initialisation failures on ESP32
  - `getRev()` display fixed: firmware and component revision fields are now
    printed as ASCII characters (e.g. `2.0`) instead of raw hex values
  - `sendCommand()` CTS polling replaced with a timeout loop to prevent hangs
  - `#pragma once` include guard added; debug macro name mismatch corrected

- **New functions**
  - `getProperty(property)` — reads any chip property via GET_PROPERTY (0x13)
  - `powerDown()` — soft power-down without hardware reset
  - `powerUpDigital(sampleRate)` — power-up in I2S digital audio input mode
  - `setAudioCompander(mode, threshold, attack, release, gain, limiterRelease)` —
    configures the Si4713 audio dynamic range control and peak limiter
  - `setASQThresholds(levelLow, durLow, levelHigh, durHigh)` — configures Audio
    Signal Quality silence / overdrive detection thresholds
  - `scanNoise(startFreq, endFreq, step)` — scans a frequency range and returns
    the channel with the lowest noise floor
  - `setRDSpi(pi)` — sets the RDS Programme Identifier code independently of
    `beginRDS()`

---

*Author: Adrian YO3HJV*
