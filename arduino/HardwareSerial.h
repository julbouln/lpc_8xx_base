/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#include "Stream.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

// Define config for Serial.begin(baud, config);
#define SERIAL_7N1 UART_CFG_DATALEN_7 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1
#define SERIAL_8N1 UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1
#define SERIAL_7N2 UART_CFG_DATALEN_7 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_2
#define SERIAL_8N2 UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_2
#define SERIAL_7E1 UART_CFG_DATALEN_7 | UART_CFG_PARITY_EVEN | UART_CFG_STOPLEN_1
#define SERIAL_8E1 UART_CFG_DATALEN_8 | UART_CFG_PARITY_EVEN | UART_CFG_STOPLEN_1
#define SERIAL_7E2 UART_CFG_DATALEN_7 | UART_CFG_PARITY_EVEN | UART_CFG_STOPLEN_2
#define SERIAL_8E2 UART_CFG_DATALEN_8 | UART_CFG_PARITY_EVEN | UART_CFG_STOPLEN_2
#define SERIAL_7O1 UART_CFG_DATALEN_7 | UART_CFG_PARITY_ODD | UART_CFG_STOPLEN_1
#define SERIAL_8O1 UART_CFG_DATALEN_8 | UART_CFG_PARITY_ODD | UART_CFG_STOPLEN_1
#define SERIAL_7O2 UART_CFG_DATALEN_7 | UART_CFG_PARITY_ODD | UART_CFG_STOPLEN_2
#define SERIAL_8O2 UART_CFG_DATALEN_8 | UART_CFG_PARITY_ODD | UART_CFG_STOPLEN_2

#include "uart_8xx.h"

struct ring_buffer;

class HardwareSerial
	// : public Stream
{
private:
	int _uart_n;
	LPC_USART_T *_uart;

    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);
	
  public:
    HardwareSerial(int);
    void begin(unsigned long);
    void begin(unsigned long, uint8_t);
    void begin(unsigned long, int, int);
    void begin(unsigned long, uint8_t, int, int);

    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
	size_t write(uint8_t);
	
    size_t write(const char *str) {
				return Chip_UART_SendBlocking(_uart,(const uint8_t *)str, strlen(str));
 //     if (str == NULL) return 0;
//      return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }
	
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
	
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);
    operator bool();
};

  extern HardwareSerial Serial;
#ifndef MCUlpc810
  extern HardwareSerial Serial1;
#endif
extern void serialEventRun(void) __attribute__((weak));

#endif
