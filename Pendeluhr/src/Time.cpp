#include "application.h"
#include "DCF77.h"

unsigned short timeDCF77 = 0;
unsigned short timeCloud = 0;
unsigned short timeX = 0;
bool timeIsValid = FALSE;


void time(){
  if(Time.isValid() && Particle.connected()){
    timeIsValid == TRUE;
    Serial.print("cloud Time: ");
    Serial.print(Time.hour()); Serial.print(":"); Serial.print(Time.minute()); Serial.print(":"); Serial.println(Time.second());
    Serial.print(Time.day()); Serial.print("."); Serial.print(Time.month()); Serial.print("."); Serial.println(Time.year());
  }
  else if((Time.isValid() == FALSE || Particle.connected() == FALSE) && DCF77isValid()){
    timeX = timeDCF77;
    timeIsValid == TRUE;
    Serial.print("DCF77 Time: ");
    Serial.println(DCF77getTime());
    Serial.println(DCF77getDate());
  }
  else{timeIsValid == FALSE; Serial.println("no valid Time");}

}
