#ifndef Arduino_h
#define Arduino_h

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
	
#include <stdio.h>
#include "chip.h"

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3 
	
typedef uint8_t boolean;
typedef uint8_t byte;

void init(void);

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
//void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);

void delay(unsigned long);
void delayMicroseconds(unsigned long);

unsigned long millis(void);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

extern void setup(void);
extern void loop(void);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
#endif