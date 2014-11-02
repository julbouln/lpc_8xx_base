#include "Arduino.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

const uint32_t OscRateIn = 12000000;
const uint32_t ExtRateIn = 0;

//
volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
}
//

void pinMode(uint8_t pin, uint8_t mode) {
	Chip_GPIO_SetPinDIR(LPC_GPIO_PORT,0,pin,mode);
}

void digitalWrite(uint8_t pin, uint8_t value) {
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,0,pin,value);
}

int digitalRead(uint8_t pin) {
	return (uint32_t)Chip_GPIO_GetPinState(LPC_GPIO_PORT,0,pin);
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

void delayMicroseconds(unsigned long us) {
	uint32_t now = msTicks;
	while ((msTicks-now) < us/1000);
}


unsigned long millis() {
	return msTicks;
}
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

