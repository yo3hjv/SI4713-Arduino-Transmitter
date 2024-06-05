#include <Wire.h>

// I2C address for SI4713
const uint8_t SI4713_ADDR = 0x63;

// Default values for dynamic range control settings
const int8_t TX_ACOMP_THRESHOLD = -40; // in dB
const uint8_t TX_ACOMP_ATTACK_TIME = 5; // in ms
const uint16_t TX_ACOMP_RELEASE_TIME = 100; // in ms
const uint8_t TX_ACOMP_GAIN = 15; // in dB
const uint16_t TX_LIMITER_RELEASE_TIME = 5; // in ms

// Default values for deviation settings
const uint16_t TX_AUDIO_DEVIATION = 6825; // in 10 Hz increments
const uint16_t TX_PILOT_DEVIATION = 675; // in 10 Hz increments

  // Helper function to transmit I2C data
void transmitI2C(uint8_t reg, const uint8_t *data, size_t len) {
            Wire.beginTransmission(SI4713_ADDR);
            Wire.write(reg);
            Wire.write(data, len);
            Wire.endTransmission();
            delay(5);  // Allow register to set
}

void setup() {
            pinMode(12, OUTPUT);
            digitalWrite(12, LOW);
            delay(100);
            digitalWrite(12, HIGH);
            delay(5);
          
            Wire.begin(); // Initialize I2C
            
            powerup();
            setClock();
            
            // Example frequency: 101.70 MHz (101700 kHz)
            tune(101700);
            
            multiplex();
            preemphasis();
            compEnable();
            compSM();
            limiterRelease();
            deviation();
            setPower();
            level();
            audioDeviation();
            pilotDeviation();
}

void loop() {}

void setClock() {
            uint8_t data[] = {0x00, 0x02, 0x01, 0x7E, 0xF4};
            transmitI2C(0x12, data, sizeof(data));
}

void powerup() {
            uint8_t data[] = {0xC2, 0x50};
            transmitI2C(0x01, data, sizeof(data));
}

void limiterRelease() {
            // Convert limiter release time to 16-bit value
            uint16_t limiter_release_time = TX_LIMITER_RELEASE_TIME * 20.4; // each step = 0.1 ms
            uint8_t data[] = {0x00, 0x22, 0x05, (limiter_release_time >> 8) & 0xFF, limiter_release_time & 0xFF};
            transmitI2C(0x12, data, sizeof(data));
}

void compSM() {
            // Convert threshold to 16-bit value (e.g., -40 dB -> 0xFFD8)
            int16_t threshold = TX_ACOMP_THRESHOLD * 256; // 1 dB step size
          
            // Send TX_ACOMP_THRESHOLD
            uint8_t data1[] = {0x00, 0x22, 0x01, (threshold >> 8) & 0xFF, threshold & 0xFF};
            transmitI2C(0x12, data1, sizeof(data1));
          
            // Send TX_ACOMP_ATTACK_TIME (e.g., 5 ms -> 0x0005)
            uint8_t data2[] = {0x00, 0x22, 0x02, 0x00, TX_ACOMP_ATTACK_TIME / 0.5}; // each step = 0.5 ms
            transmitI2C(0x12, data2, sizeof(data2));
          
            // Convert release time to 16-bit value (e.g., 100 ms -> 0x0064)
            uint16_t release_time = TX_ACOMP_RELEASE_TIME / 250; // each step = 250 ms
          
            // Send TX_ACOMP_RELEASE_TIME
            uint8_t data3[] = {0x00, 0x22, 0x03, (release_time >> 8) & 0xFF, release_time & 0xFF};
            transmitI2C(0x12, data3, sizeof(data3));
          
            // Send TX_ACOMP_GAIN (e.g., 15 dB -> 0x000F)
            uint8_t data4[] = {0x00, 0x22, 0x04, 0x00, TX_ACOMP_GAIN};
            transmitI2C(0x12, data4, sizeof(data4));
}

void tune(uint16_t frequency_kHz) {
            uint16_t frequency_10kHz = frequency_kHz / 10;
            uint8_t high_byte = (frequency_10kHz >> 8) & 0xFF;
            uint8_t low_byte = frequency_10kHz & 0xFF;
            uint8_t data[] = {0x00, high_byte, low_byte};
            transmitI2C(0x30, data, sizeof(data));
}

void deviation() {
            uint8_t data[] = {0x00, 0x21, 0x01, 0x1A, 0x90};
            transmitI2C(0x12, data, sizeof(data));
}

void compEnable() {
            uint8_t data[] = {0x00, 0x22, 0x00, 0x00, 0x03};
            transmitI2C(0x12, data, sizeof(data));
}

void level() {
            uint8_t data[] = {0x00, 0x21, 0x04, 0x21, 0x96};
            transmitI2C(0x12, data, sizeof(data));
}

void preemphasis() {
            uint8_t data[] = {0x00, 0x21, 0x06, 0x00, 0x01};
            transmitI2C(0x12, data, sizeof(data));
}

void multiplex() {
            uint8_t data[] = {0x00, 0x21, 0x00, 0x00, 0x03};
            transmitI2C(0x12, data, sizeof(data));
}

void setPower() {
            uint8_t data[] = {0x00, 0x00, 0x73, 0x00};
            transmitI2C(0x31, data, sizeof(data));
}

void audioDeviation() {
            // Convert audio deviation to 16-bit value
            uint16_t audio_deviation = TX_AUDIO_DEVIATION;
            uint8_t data[] = {0x00, 0x21, 0x01, (audio_deviation >> 8) & 0xFF, audio_deviation & 0xFF};
            transmitI2C(0x12, data, sizeof(data));
}

void pilotDeviation() {
            // Convert pilot deviation to 16-bit value
            uint16_t pilot_deviation = TX_PILOT_DEVIATION;
            uint8_t data[] = {0x00, 0x21, 0x02, (pilot_deviation >> 8) & 0xFF, pilot_deviation & 0xFF};
            transmitI2C(0x12, data, sizeof(data));
}
