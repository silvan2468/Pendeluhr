#ifndef DCF77_h
#define DCF77_h

void DCF77read();
unsigned short DCF77getTime();
unsigned long DCF77getDate();
volatile bool DCF77isValid();

#endif
