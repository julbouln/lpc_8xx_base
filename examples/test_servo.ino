#include <Arduino.h>
#include "Servo.h"

Servo myservo;

void setup() 
{ 
  myservo.attach(7);
  myservo.writeMicroseconds(1500);  // set servo to mid-point
} 

void loop() {
    myservo.writeMicroseconds(1500);  // set servo to mid-point
	delay(1000);
    myservo.writeMicroseconds(1000);  
	delay(1000);
    myservo.writeMicroseconds(1500);  // set servo to mid-point
	delay(1000);
    myservo.writeMicroseconds(2000);  
	delay(1000);
    myservo.writeMicroseconds(1500);  // set servo to mid-point
	delay(5000);
    myservo.write(0);
	delay(1000);
    myservo.write(90); // set servo to mid-point
	delay(1000);
    myservo.write(180);
	delay(1000);
    

}