#include "Arduino.h"
#include "HardwareSerial.h"

void setup() {	
    Serial.begin(9600);
}

void loop() {	
	Serial.println("Hello world");
	Serial.println(15);
	Serial.println(140,HEX);
	Serial.println();
	delay(1000);
}