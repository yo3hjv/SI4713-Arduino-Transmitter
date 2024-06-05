#include <Wire.h>

// I2C address for SI4713
const uint8_t SI4713_ADDR = 0x63;

// Function to transmit I2C data
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
}

// Nothing here in the loop() 
void loop() {}




// Individual functions
void setClock() {
          uint8_t data[] = {0x00, 0x02, 0x01, 0x7E, 0xF4};
          transmitI2C(0x12, data, sizeof(data));
}

void powerup() {
          uint8_t data[] = {0xC2, 0x50};
          transmitI2C(0x01, data, sizeof(data));
}

void limiterRelease() {
          uint8_t data[] = {0x00, 0x22, 0x05, 0x00, 0x0D};
          transmitI2C(0x12, data, sizeof(data));
}

void compSM() {
              uint8_t data1[] = {0x00, 0x22, 0x01, 0xFF, 0xD8};
              uint8_t data2[] = {0x00, 0x22, 0x02, 0x00, 0x02};
              uint8_t data3[] = {0x00, 0x22, 0x03, 0x00, 0x02};
              uint8_t data4[] = {0x00, 0x22, 0x04, 0x00, 0x0F};
            
              transmitI2C(0x12, data1, sizeof(data1));
              transmitI2C(0x12, data2, sizeof(data2));
              transmitI2C(0x12, data3, sizeof(data3));
              transmitI2C(0x12, data4, sizeof(data4));
}

void tune(uint16_t frequency_kHz) {
            // Frequency must be divided by 10 to convert kHz to 10 kHz steps
            uint16_t frequency_10kHz = frequency_kHz / 10;
          
            // Convert frequency to two bytes
            uint8_t high_byte = (frequency_10kHz >> 8) & 0xFF;
            uint8_t low_byte = frequency_10kHz & 0xFF;
          
            // Data array to send
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
