/*
        SI4713_V6.ino
        All rights reserved to YO3HJV, Adrian Florescu 
        2024

        This is the final iteration for the SI4713 FM Transmitter controller.
        This version can set some of the most important parameters through global declarations
        For more informations about what the variables are doing, I strongly reccomend reading the datasheet.
    Set parameters and read parameters that were set
*/

#include <Wire.h>

// I2C address for SI4713
const uint8_t SI4713_ADDR = 0x63;

// Default values for dynamic range control settings
const int8_t TX_ACOMP_THRESHOLD = -40; // in dB
const uint8_t TX_ACOMP_ATTACK_TIME = 5; // in ms
const uint16_t TX_ACOMP_RELEASE_TIME = 100; // in ms
const uint8_t TX_ACOMP_GAIN = 15; // in dB
const uint16_t TX_LIMITER_RELEASE_TIME = 5; // in ms.  MPX limiter

// Default values for deviation settings
const uint16_t TX_AUDIO_DEVIATION = 6825; // default value, in 10 Hz increments. 6825 is default meaning a deviation of +/-68.25 KHz.
                // It can be increased to 7500 +/-75 KHz for a BW of 150 KHz.
                // In this case, the TX_PILOT_DEVIATION should be set for 750
const uint16_t TX_PILOT_DEVIATION = 675; // default value, in 10 Hz increments. The pilot tone deviation  must be 10% from the audio deviation

void setup() {
                pinMode(12, OUTPUT);
                digitalWrite(12, LOW);
                delay(100);
                digitalWrite(12, HIGH);
                delay(5);
              
                Wire.begin(); // Initialize I2C
                Serial.begin(9600); // Initialize Serial communication
  
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
              
                readProperties(); // Read and print properties after setting them
}

void loop() {}

// Helper function to transmit I2C data
void transmitI2C(uint8_t reg, const uint8_t *data, size_t len) {
                Wire.beginTransmission(SI4713_ADDR);
                Wire.write(reg);
                Wire.write(data, len);
                Wire.endTransmission();
                delay(5);  // Allow register to set
}

// Helper function to read I2C data
void readI2C(uint8_t reg, uint8_t *data, size_t len) {
                Wire.beginTransmission(SI4713_ADDR);
                Wire.write(reg);
                Wire.endTransmission();
                
                Wire.requestFrom(SI4713_ADDR, len);
                for (size_t i = 0; i < len; i++) {
                  data[i] = Wire.read();
                }
}

void setClock() {
               uint8_t data[] = {0x00, 0x02, 0x01, 0x80, 0x00}; // 0x8000 for 32768 Hz. Default.
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

// Function to read and print properties
void readProperties() {
                Serial.println("Reading SI4713 Properties:");
              
                // Read and print TX_Frequency
                uint8_t tx_frequency[3] = {0};
                readI2C(0x13, tx_frequency, sizeof(tx_frequency));
                uint16_t frequency_10kHz = (tx_frequency[1] << 8) | tx_frequency[2];
                uint16_t frequency_kHz = frequency_10kHz * 10;
                Serial.print("TX_Frequency: ");
                Serial.print(frequency_kHz / 1000.0, 1);
                Serial.println(" MHz");
              
                // Read and print TX_ACOMP_THRESHOLD
                uint8_t acomp_threshold[3] = {0};
                readI2C(0x13, acomp_threshold, sizeof(acomp_threshold));
                int16_t threshold = (acomp_threshold[1] << 8) | acomp_threshold[2];
                Serial.print("TX_ACOMP_THRESHOLD: ");
                Serial.print(threshold / 256.0);
                Serial.println(" dB");
              
                // Read and print TX_ACOMP_ATTACK_TIME
                uint8_t acomp_attack_time[3] = {0};
                readI2C(0x13, acomp_attack_time, sizeof(acomp_attack_time));
                uint16_t attack_time = acomp_attack_time[2];
                Serial.print("TX_ACOMP_ATTACK_TIME: ");
                Serial.print(attack_time * 0.5);
                Serial.println(" ms");
              
                // Read and print TX_ACOMP_RELEASE_TIME
                uint8_t acomp_release_time[3] = {0};
                readI2C(0x13, acomp_release_time, sizeof(acomp_release_time));
                uint16_t release_time = (acomp_release_time[1] << 8) | acomp_release_time[2];
                Serial.print("TX_ACOMP_RELEASE_TIME: ");
                Serial.print(release_time * 250);
                Serial.println(" ms");
              
                // Read and print TX_ACOMP_GAIN
                uint8_t acomp_gain[3] = {0};
                readI2C(0x13, acomp_gain, sizeof(acomp_gain));
                uint16_t gain = acomp_gain[2];
                Serial.print("TX_ACOMP_GAIN: ");
                Serial.print(gain);
                Serial.println(" dB");
              
                // Read and print TX_LIMITER_RELEASE_TIME
                uint8_t limiter_release_time[3] = {0};
                readI2C(0x13, limiter_release_time, sizeof(limiter_release_time));
                uint16_t release_time_limiter = (limiter_release_time[1] << 8) | limiter_release_time[2];
                Serial.print("TX_LIMITER_RELEASE_TIME: ");
                Serial.print(release_time_limiter / 20.4);
                Serial.println(" ms");
              
                // Read and print TX_AUDIO_DEVIATION
                uint8_t audio_deviation[3] = {0};
                readI2C(0x13, audio_deviation, sizeof(audio_deviation));
                uint16_t audio_dev = (audio_deviation[1] << 8) | audio_deviation[2];
                Serial.print("TX_AUDIO_DEVIATION: ");
                Serial.print(audio_dev * 10);
                Serial.println(" Hz");
              
                // Read and print TX_PILOT_DEVIATION
                uint8_t pilot_deviation[3] = {0};
                readI2C(0x13, pilot_deviation, sizeof(pilot_deviation));
                uint16_t pilot_dev = (pilot_deviation[1] << 8) | pilot_deviation[2];
                Serial.print("TX_PILOT_DEVIATION: ");
                Serial.print(pilot_dev * 10);
                Serial.println(" Hz");
              
                // Read and print TX Level
                uint8_t level_data[3] = {0};
                readI2C(0x13, level_data, sizeof(level_data));
                uint16_t level = (level_data[1] << 8) | level_data[2];
                Serial.print("TX Level: ");
                Serial.print(level);
                Serial.println(" dB");
              
                // Read and print TX Preemphasis
                uint8_t preemphasis_data[3] = {0};
                readI2C(0x13, preemphasis_data, sizeof(preemphasis_data));
                uint16_t preemphasis = (preemphasis_data[1] << 8) | preemphasis_data[2];
                Serial.print("TX Preemphasis: ");
                Serial.print(preemphasis);
                Serial.println(" μs");
              
                // Read and print TX Multiplex
                uint8_t multiplex_data[3] = {0};
                readI2C(0x13, multiplex_data, sizeof(multiplex_data));
                uint16_t multiplex = (multiplex_data[1] << 8) | multiplex_data[2];
                Serial.print("TX Multiplex: ");
                Serial.print(multiplex);
                Serial.println(" kHz");
              
                // Read and print TX Power
                uint8_t power_data[3] = {0};
                readI2C(0x13, power_data, sizeof(power_data));
                uint16_t power = (power_data[1] << 8) | power_data[2];
                Serial.print("TX Power: ");
                Serial.print(power);
                Serial.println(" dB");
}
