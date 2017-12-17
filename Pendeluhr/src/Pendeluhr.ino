#include "DCF77.h"
#include "Time.h"

/*
 * Project Test_1
 * Description:
 * Author:
 * Date:
 */

// System settings -------------------------------------

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

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


 // Timers ----------------------------------------------

 void task10s(){
   time();

  // if (Particle.connected() == FALSE){ Particle.connect();}

// if(aaa == 1){WiFi.on(); Particle.connect(); aaa = 0;}
// else{WiFi.off(); aaa = 1;}

   if (Particle.connected()){connectingTimer = 0;}
   if (connectingTimer >= 4 && Particle.connected() == FALSE){WiFi.off(); connectingTimer = 0;}               // wenn nach x+n Sekunden keine Verbindung aufgebaut werdenn konnte, Verbindungsversuch beenden
   if (Particle.connected() == FALSE){connectingTimer++;}
   if (Particle.connected() == FALSE && connectingTimer == 3) {Particle.connect(); connectingTimer++;}     // alle x Sekunden versuchen Verbindung aufzubauen

 }

 Timer timer10s(10000, task10s);


 // -----------------------------------------------------

 void setup() {

   pinMode(led1, OUTPUT);

   pinMode(RX, INPUT_PULLUP);             // INPUT_PULLUP/PULLDOWN setzt den Microcontroller-eigenen PULLUP/DOWN-Widerstand --> kein externer Widerstand nötig
   attachInterrupt(RX, DCF77Interrupt, CHANGE);

  //  oldTime = millis();

   Serial.begin(9600);
   Time.zone(+1);

   timer10s.start();

   Particle.connect();
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
