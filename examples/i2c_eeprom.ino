#include <Arduino.h>
#include <Wire.h>

// I2C 24C02 eeprom

#define EEPROM_ADDR 0x50

void write_byte(uint8_t addr, uint8_t data)
{
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write(addr);
    Wire.write(data);
    Wire.endTransmission();	
}

uint8_t read_byte(uint8_t addr)
{
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write(addr);
    Wire.endTransmission();

    Wire.requestFrom((byte)EEPROM_ADDR,(byte)1);
    return Wire.read();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Test EEPROM");
  Wire.begin();

  delay(1000);    
  write_byte(0x00,0xAA);
}

void loop() {
  uint8_t data=0;

  delay(1000);
  data=read_byte(0x00);
  Serial.println(data);

}