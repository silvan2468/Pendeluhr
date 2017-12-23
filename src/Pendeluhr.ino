#include "DCF77.h"
#include "Time.h"
#include "ledmatrix-max7219-max7221.h"
#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"

/*
 * Project Pendeluhr
 * Description:
 * Author: Silvan Huber
 * Date: 23.12.2017
 */

// System settings -------------------------------------

SYSTEM_THREAD(ENABLED);
// SYSTEM_MODE(SEMI_AUTOMATIC);

LSM6DS3 myIMU(SPI_MODE, A2); // SPI
// LSM6DS3 myIMU( I2C_MODE, 0x6B ); //Default constructor is I2C, addr 0x6B

//------------------------------------------------------


 void DCF77Interrupt(void);

 int led1 = D7; // Instead of writing D7 over and over again, we'll write led2
 // This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

 volatile int state = LOW;
 volatile int oldState = LOW;

 unsigned char connectingTimer = 0;

 unsigned long counterMillis = 0;
 unsigned long oldTime2 = 0;

 unsigned char aaa = 1;

 LEDMatrix *led;


 // Timers ----------------------------------------------

 void task200ms(){
   //Get all parameters
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X = ");
    Serial.println(myIMU.readFloatAccelX(), 4);
    Serial.print(" Y = ");
    Serial.println(myIMU.readFloatAccelY(), 4);
    Serial.print(" Z = ");
    Serial.println(myIMU.readFloatAccelZ(), 4);

    Serial.print("\nGyroscope:\n");
    Serial.print(" X = ");
    Serial.println(myIMU.readFloatGyroX(), 4);
    Serial.print(" Y = ");
    Serial.println(myIMU.readFloatGyroY(), 4);
    Serial.print(" Z = ");
    Serial.println(myIMU.readFloatGyroZ(), 4);

    Serial.print("\nThermometer:\n");
    Serial.print(" Degrees C = ");
    Serial.println(myIMU.readTempC(), 4);
 }

 void task10s(){
   time();

  // if (Particle.connected() == FALSE){ Particle.connect();}

// if(aaa == 1){WiFi.on(); Particle.connect(); aaa = 0;}
// else{WiFi.off(); aaa = 1;}

  //  if (Particle.connected()){connectingTimer = 0;}
  //  if (connectingTimer >= 4 && Particle.connected() == FALSE){WiFi.off(); connectingTimer = 0;}               // wenn nach x+n Sekunden keine Verbindung aufgebaut werdenn konnte, Verbindungsversuch beenden
  //  if (Particle.connected() == FALSE){connectingTimer++;}
  //  if (Particle.connected() == FALSE && connectingTimer == 3) {Particle.connect(); connectingTimer++;}     // alle x Sekunden versuchen Verbindung aufzubauen

 }

 Timer timer200ms(200, task200ms);
 Timer timer10s(10000, task10s);


 // ------------------------------------------------------------------------------



 void setup() {

// Pins --------------------------------------------------------------------------
   pinMode(led1, OUTPUT);
   attachInterrupt(RX, DCF77Interrupt, CHANGE);

   pinMode(RX, INPUT_PULLUP);      // INPUT_PULLUP/PULLDOWN setzt den Microcontroller-eigenen PULLUP/DOWN-Widerstand --> kein externer Widerstand nötig

// Serial ------------------------------------------------------------------------
   Serial.begin(9600);

// Time(er) ----------------------------------------------------------------------
   Time.zone(+1);
   timer200ms.start();    // start timer 200ms
   timer10s.start();    // start timer 10s

// MAX7219 -----------------------------------------------------------------------
   // setup pins and library
   // 1 display per row, 1 display per column
   // optional pin settings - default: CLK = D4, CS = D0, D_OUT = D2
   // (pin settings is independent on HW SPI)
   led  = new LEDMatrix(1, 1, D4, D0, D2);

   // > add every matrix in the order in which they have been connected <
   // the first matrix in a row, the first matrix in a column
   // vertical orientation (-90°) and no mirroring - last three args optional
   led->addMatrix(0, 0, 270, false, false);
   led->setIntensity(1, 0);                // Intensität 1-15 , Display 0
   led->bitmap->setPixel(0, 0, true);
   led->bitmap->setPixel(1, 1, true);
   led->bitmap->setPixel(2, 2, true);
   led->flush();

// LSM6DS33 ----------------------------------------------------------------------
   //Call .begin() to configure the IMUs
   if( myIMU.begin() != 0 ){Serial.println("Problem starting the sensor with CS @ Pin A2.");}
   else{Serial.println("Sensor with CS @ Pin A2 started.");}

// WiFi --------------------------------------------------------------------------
   //  Particle.connect();
//--------------------------------------------------------------------------------

 }


 void loop() {

    if(state != oldState){            // wird 2x pro Sekunde durchlaufen!! bei 100 oder 200ms und bei 800 oder 900ms
      digitalWrite(led1, state);
      DCF77read();
      oldState = state;
    }
 }

 void DCF77Interrupt() {
  state = !state;
}
