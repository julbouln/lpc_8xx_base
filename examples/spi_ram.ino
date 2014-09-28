#include <Arduino.h>
#include <SPI.h>

int cs=13;

#define RDSR        0x05
#define WRSR        0x01
#define READ        0x03
#define WRITE       0x02


void spi_sram_write_mode(uint8_t mode)
{
	digitalWrite(cs,LOW);
	SPI.transfer(WRSR);
	SPI.transfer(mode);
	digitalWrite(cs,HIGH);		
}

  
uint8_t spi_sram_read_byte(uint32_t address)
{
	uint8_t byte;
	
	digitalWrite(cs,LOW);
	
	SPI.transfer(READ);
	SPI.transfer((uint8_t)(address >> 8));
	SPI.transfer((uint8_t)address);
	byte = SPI.transfer(0x00);
	digitalWrite(cs,HIGH);
	
	return byte;
} 

void spi_sram_write_byte(uint32_t address, uint8_t byte)
{
	digitalWrite(cs,LOW);
	
	SPI.transfer(WRITE);
	SPI.transfer((uint8_t)(address >> 8));
	SPI.transfer((uint8_t)address);
	SPI.transfer(byte);
	digitalWrite(cs,HIGH);
	
} 

// write 255 first byte of sram
void test_write()
{
    int i;
 	for(i=0;i<256;i++) {
 		spi_sram_write_byte(i,i);
 	}
}

// read 255 first byte of sram and print to serial
void test_read()
{
	char bytes[256];
		int i;
		for(i=0;i<256;i++) {
			bytes[i]=spi_sram_read_byte(i);
			Serial.print("READ ");
			Serial.print(i,HEX);
			Serial.print("=");
			Serial.print(bytes[i],HEX);
			Serial.println();
		}
	}


void setup() {	
	pinMode(cs, OUTPUT);
  
   delay(1000);
  
   Serial.begin(9600);
   SPI.begin();
	
	
   delay(100);
   test_write();
}

void loop() {
	test_read();
}