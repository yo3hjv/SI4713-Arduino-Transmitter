

   /* Astea sunt comenzi trimise catre SI4713 direct prin I2C si regleaza tot ce e de reglat astfel incat transmitatorul sa 
   furnizeze un semnal compatibil cu receptoarele europene, folosind si etajele de limitare si compresie incorporate in 
   circuit.
   Valorile din registri au fost alese pentru semnal dintr-un mp3 player.
   Copyright 2015, YO3HJV, Adrian
   */

   /*
   The setiings are sent to SI4713 directly via I2C commands and not via the Arduino library. The commands are documented in the SI4713 datasheet.
   The audio settings for the limiter and AGS are documented in "AN332.pdf".
   Copyright 2015, YO3HJV, Adrian
   */

   // The transmitter's frequency was previously programmed via the AdaFruit example .ino
   
   
        #include <Wire.h>


    
        

        void setup() {
                
          // RESET, wait then POWERUP and wait then SET
          
               pinMode(12, OUTPUT);
               digitalWrite(13, LOW);
               delay(100);
               digitalWrite(12, HIGH);
               delay(5);
               powerup();
               
               setClock();          
               tune();
               multiplex();
               preemphasys();
             
               compEnable();
               compsm();
               limiterrelease();
               deviation();             
               setpower();
               level();   
               
               
                 
               
                     }



         void loop()  {                       
                 
                      }
         
         void setClock()   {
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x02);                                  // 
                              Wire.write(0x01);                                  // 
                              Wire.write(0x7E);                                  // 
                              Wire.write(0xF4);                                  // 
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                            }
           
           
  
           
         void powerup()    {
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x01);                                  // sends register address
                              Wire.write(0xC2);                                  // sends register data
                              Wire.write(0x50);                                  // 
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                            }
         
         
         void limiterrelease() {
              //  TX_LIMITER_RELEASE_TIME 39.38 ms)
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // 
                              Wire.write(0x05);                                  // TX_LIMITER_RELEASE_TIME
                              Wire.write(0x00);                                  // Sets the limiter release time to 13 (39.38 ms)
                              Wire.write(0x0D);                                  //
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                               }
                  
                               
           
         void  compsm()  {    // Smooth Compression
                 //  TX_ACOMP_THRESHOLD    -40db
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // 
                              Wire.write(0x01);                                  // TX_ACOMP_THRESHOLD
                              Wire.write(0xFF);                                  // Threshold = –40 dBFS = 0xffd8
                              Wire.write(0xD8);                                  //
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                             
                 // TX_ACOMP_ATTACK_TIME = 5 ms
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // 
                              Wire.write(0x02);                                  // TX_ACOMP_ATTACK_TIME
                              Wire.write(0x00);                                  // 
                              Wire.write(0x02);                                  // Attack time = 1.5 ms = 2 STEP: 0.5 msec
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                              
                 // TX_ACOMP_RELEASE_TIME = 100 ms
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // 
                              Wire.write(0x03);                                  // TX_ACOMP_RELEASE_TIME
                              Wire.write(0x00);                                  // 
                              Wire.write(0x02);                                  // Release time = 1000 ms = 4 STEP: 250ms
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                              
                 // TX_ACOMP_GAIN = 15 dB
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x12);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // 
                              Wire.write(0x04);                                  // TX_ACOMP_GAIN
                              Wire.write(0x00);                                  // 
                              Wire.write(0x0F);                                  // 
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);
                       }

              void tune()  {
                              Wire.beginTransmission(0x63);                      // transmit to device 0x63
                              Wire.write(0x30);                                  // sends register address
                              Wire.write(0x00);                                  // sends register data
                              Wire.write(0x22);                                  // Set frequency in HEX
                              Wire.write(0x2E);                                  // 96.50 >>> 9650 = 25B2 in HEX
                              Wire.endTransmission();                            // stop transmitting
                              delay(5);                                          // allow register to set
                            }
                 
              void deviation() {                                            // max dev 68.25 kHz >>> 6825 to Hex: 1AA9
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x12);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x21);                                // sends register data
                              Wire.write(0x01);                                // sends register data
                              Wire.write(0x1A);                                // Max deviation 68.00
                              Wire.write(0x90);                                // 1A90
                              Wire.endTransmission();                          // stop transmitting
                              delay(5);                                        // allow register to set                                 
                               }
                               
              void compEnable() {                                            // 
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x12);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x22);                                // sends register data
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x00);                                //                               
                              Wire.write(0x03);                                //  
                              Wire.endTransmission();                          // stop transmitting
                              delay(5);                                        // allow register to set                                 
                                 }

       
                 
              void level()  {
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x12);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x21);                                // sends register data
                              Wire.write(0x04);                                // sends register data
                              Wire.write(0x21);                                // sends register data
                              Wire.write(0x96);                                // Max input 150mV pk
                              Wire.endTransmission();                          // stop transmitting
                            }
                 
              void preemphasys() {
                        
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x12);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x21);                                // sends register data
                              Wire.write(0x06);                                // sends register data
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x01);                                //  50 uSec for Europe
                              Wire.endTransmission();                          // stop transmitting
                              delay(5);  
                                 }
                                 
              void multiplex()  {
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x12);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x21);                                // sends register data
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x03);                                // 
                              Wire.endTransmission();                          // stop transmitting
                                }
                                 
              void setpower()    {
                              Wire.beginTransmission(0x63);                    // transmit to device 0x63
                              Wire.write(0x31);                                // sends register address
                              Wire.write(0x00);                                // sends register data
                              Wire.write(0x00);                                // Power
                              Wire.write(0x73);                                // to 115 dBuV
                              Wire.write(0x00);                                // Antenna capacitor to AUTO
                              Wire.endTransmission();                          // stop transmitting
                              delay(5);  
                                 }


