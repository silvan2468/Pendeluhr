#include "application.h"

volatile int dcfVal = 0;
int minCounter = 0;
unsigned char minute = 0;
unsigned char parityMinute = 0;
unsigned char hour = 0;
unsigned char parityHour = 0;
unsigned char day = 0;
unsigned char month = 0;
unsigned char year = 0;
unsigned long newTime = 0;
unsigned long oldTime = 0;
unsigned long dt = 0;            // counter für DCF-Absenkung (100 oder 200 ms)

unsigned short DCF77time = 0;
unsigned long DCF77date = 0;

volatile bool DCF77valid = FALSE;
volatile bool parity = TRUE;


void DCF77read(){

  newTime = millis();
  dt = newTime-oldTime;
  oldTime = newTime;

  if(dt >= 50 && dt < 150){minCounter++;}
  if(dt >= 150 && dt < 250){minCounter++;}
  if(minCounter > 20 && dt >= 50 && dt < 150){/*Serial.print(0);*/ dcfVal = 0;}
  if(minCounter > 20 && dt >= 150 && dt < 250){/*Serial.print(1);*/ dcfVal = 1;}

  if(dt >= 50 && dt < 250 && parity){

    if(minCounter == 21 && dcfVal == 1){minute = minute+1; parityMinute++;}
    if(minCounter == 22 && dcfVal == 1){minute = minute+2; parityMinute++;}
    if(minCounter == 23 && dcfVal == 1){minute = minute+4; parityMinute++;}
    if(minCounter == 24 && dcfVal == 1){minute = minute+8; parityMinute++;}
    if(minCounter == 25 && dcfVal == 1){minute = minute+10; parityMinute++;}
    if(minCounter == 26 && dcfVal == 1){minute = minute+20; parityMinute++;}
    if(minCounter == 27 && dcfVal == 1){minute = minute+40; parityMinute++;}


    if(     minCounter == 28 && dcfVal == 1 && (parityMinute % 2) == 0){parity = FALSE; /*Serial.print("Parity FALSE");*/}
    else if(minCounter == 28 && dcfVal == 0 && (parityMinute % 2) == 1){parity = FALSE; /*Serial.print("Parity FALSE");*/}


    if(minCounter == 29 && dcfVal == 1){hour = hour+1; parityHour++;}
    if(minCounter == 30 && dcfVal == 1){hour = hour+2; parityHour++;}
    if(minCounter == 31 && dcfVal == 1){hour = hour+4; parityHour++;}
    if(minCounter == 32 && dcfVal == 1){hour = hour+8; parityHour++;}
    if(minCounter == 33 && dcfVal == 1){hour = hour+10; parityHour++;}
    if(minCounter == 34 && dcfVal == 1){hour = hour+20; parityHour++;}

    if(     minCounter == 35 && dcfVal == 1 && (parityHour % 2) == 0){parity = FALSE; /*Serial.print("Parity FALSE");*/}
    else if(minCounter == 35 && dcfVal == 0 && (parityHour % 2) == 1){parity = FALSE; /*Serial.print("Parity FALSE");*/}

    if(minCounter == 36 && dcfVal == 1){day = day+1;}
    if(minCounter == 37 && dcfVal == 1){day = day+2;}
    if(minCounter == 38 && dcfVal == 1){day = day+4;}
    if(minCounter == 39 && dcfVal == 1){day = day+8;}
    if(minCounter == 40 && dcfVal == 1){day = day+10;}
    if(minCounter == 41 && dcfVal == 1){day = day+20;}

    if(minCounter == 45 && dcfVal == 1){month = month+1;}
    if(minCounter == 46 && dcfVal == 1){month = month+2;}
    if(minCounter == 47 && dcfVal == 1){month = month+4;}
    if(minCounter == 48 && dcfVal == 1){month = month+8;}
    if(minCounter == 49 && dcfVal == 1){month = month+10;}

    if(minCounter == 50 && dcfVal == 1){year = year+1;}
    if(minCounter == 51 && dcfVal == 1){year = year+2;}
    if(minCounter == 52 && dcfVal == 1){year = year+4;}
    if(minCounter == 53 && dcfVal == 1){year = year+8;}
    if(minCounter == 54 && dcfVal == 1){year = year+10;}
    if(minCounter == 55 && dcfVal == 1){year = year+20;}
    if(minCounter == 56 && dcfVal == 1){year = year+40;}
    if(minCounter == 57 && dcfVal == 1){year = year+80;}
  }


  if(dt >= 1500){
//    Serial.println(minCounter);
    if(minCounter == 58 && parity == TRUE){
      Serial.println("DCF77 ok");
      DCF77valid = TRUE;
      DCF77time = hour*100 + minute;                     // z.B. 1523  = 15:23 Uhr
      DCF77date = day*10000 + month*100 + year;          // z.B. 210317  =  12.03.17
      // Serial.print(hour); Serial.print(":"); Serial.println(minute);
      // Serial.print(day); Serial.print("."); Serial.print(month); Serial.print("."); Serial.println(year);
    }
    else{DCF77valid = FALSE;}

    minCounter = -1; minute = 0; hour = 0; day = 0; month = 0; year = 0; parityMinute = 0; parityHour = 0; parity = TRUE;
  }
}


unsigned short DCF77getTime(){
  return DCF77time;
}

unsigned long DCF77getDate(){
  return DCF77date;
}

volatile bool DCF77isValid(){
  return DCF77valid;
}
