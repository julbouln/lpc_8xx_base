#include "Arduino.h"
#include "HardwareSerial.h"

void setup() {	
    Serial.begin(9600);
}

void loop() {	
	Serial.println("Hello world");
		Serial.print(15);
		Serial.println();
}