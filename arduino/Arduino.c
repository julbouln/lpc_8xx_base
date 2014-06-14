#include "Arduino.h"

const uint32_t OscRateIn = 12000000;
const uint32_t ExtRateIn = 0;

//
volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
}
//

void pinMode(uint8_t pin, uint8_t mode) {
	switch (mode)
	{
		case INPUT:
			Chip_GPIO_SetPinDIR(LPC_GPIO_PORT,0,pin,false);
		case OUTPUT:
			Chip_GPIO_SetPinDIR(LPC_GPIO_PORT,0,pin,true);
	}
}

void digitalWrite(uint8_t pin, uint8_t value) {
	switch (value)
	{
	case HIGH:
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,pin,true);
	break;
	case LOW:
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,pin,false);
	break;
	}
}

int digitalRead(uint8_t pin) {
    uint32_t regVal = LOW;	

	if(Chip_GPIO_GetPinState(LPC_GPIO_PORT,0,pin))
	{
		regVal=HIGH;
	}
    return ( regVal );		
}
int analogRead(uint8_t pin) {
	// no analog
	return 0;
}

void analogWrite(uint8_t pin, int data) {
	// no analog
}

void delay(unsigned long ms) {
	uint32_t now = msTicks;
	while ((msTicks-now) < ms);
}

unsigned long millis() {
	return msTicks;
}
