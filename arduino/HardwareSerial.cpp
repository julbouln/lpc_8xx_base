/*
  HardwareSerial.cpp - Hardware serial library for Wiring
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
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "Arduino.h"

#include "HardwareSerial.h"

// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(int uart_n)
{
	_uart_n=uart_n;
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baud, int tx, int rx)
{
	begin(baud,SERIAL_8N1,tx,rx);
}

void HardwareSerial::begin(unsigned long baud)
{
	// default pins(P0.4, P0.0)
	begin(baud,SERIAL_8N1,4,0);
}

void HardwareSerial::begin(unsigned long baud, byte config)
{
	// default pins(P0.4, P0.0)
	begin(baud,config,4,0);

}

void HardwareSerial::begin(unsigned long baud, byte config, int tx, int rx)
{
	Chip_Clock_SetUARTClockDiv(1);	/* divided by 1 */


	if(_uart_n) {
	/* Connect the U0_TXD_O and U0_RXD_I signals  */
		Chip_SWM_MovablePinAssign(SWM_U1_TXD_O, tx);
		Chip_SWM_MovablePinAssign(SWM_U1_RXD_I, rx);
		_uart=(LPC_USART_T *)(LPC_USART1_BASE);
	}
	else
	{
//		Chip_SWM_DisableFixedPin(SWM_FIXED_ACMP_I1);
		
		Chip_SWM_MovablePinAssign(SWM_U0_TXD_O, tx);
		Chip_SWM_MovablePinAssign(SWM_U0_RXD_I, rx);
		_uart=(LPC_USART_T *)(LPC_USART0_BASE);		
	}
	
	Chip_UART_Init(_uart);
//	Chip_UART_ConfigData(_uart, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
	
	Chip_UART_ConfigData(_uart, config);
	
	Chip_UART_SetBaud(_uart, baud);
	Chip_UART_Enable(_uart);
	Chip_UART_TXEnable(_uart);
	
//		Chip_UART_IntEnable(_uart, UART_INTEN_RXRDY);
//		Chip_UART_IntDisable(_uart, UART_INTEN_TXRDY);
	
}

void HardwareSerial::end()
{
  Chip_UART_DeInit(_uart);
}

int HardwareSerial::available(void)
{
	return 1;
}

int HardwareSerial::peek(void)
{
	return 0;
}

int HardwareSerial::read(void)
{
	return Chip_UART_ReadByte(_uart);
}

void HardwareSerial::flush()
{
}

size_t HardwareSerial::write(uint8_t c)
{
	while((Chip_UART_GetStatus(_uart) & UART_STAT_TXRDY) == 0);
	Chip_UART_SendByte(_uart,c);
   
  return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
  return Chip_UART_SendBlocking(_uart,buffer,size);
}


/* from Arduino */

size_t HardwareSerial::print(const char str[])
{
  return write(str);
}

size_t HardwareSerial::print(char c)
{
  return write((uint8_t)c);
}

size_t HardwareSerial::print(unsigned char b, int base)
{
  return print((unsigned long) b, base);
}

size_t HardwareSerial::print(int n, int base)
{
  return print((long) n, base);
}

size_t HardwareSerial::print(unsigned int n, int base)
{
  return print((unsigned long) n, base);
}

size_t HardwareSerial::print(long n, int base)
{
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      int t = print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

size_t HardwareSerial::print(unsigned long n, int base)
{
  if (base == 0) return write(n);
  else return printNumber(n, base);
}

size_t HardwareSerial::print(double n, int digits)
{
  return printFloat(n, digits);
}

size_t HardwareSerial::println(void)
{
  size_t n = print('\r');
  n += print('\n');
  return n;
}

size_t HardwareSerial::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t HardwareSerial::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t HardwareSerial::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t HardwareSerial::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t HardwareSerial::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t HardwareSerial::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t HardwareSerial::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t HardwareSerial::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t HardwareSerial::printNumber(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

size_t HardwareSerial::printFloat(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  if (isnan(number)) return print("nan");
  if (isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print("."); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    n += print(toPrint);
    remainder -= toPrint; 
  } 
  
  return n;
}

HardwareSerial::operator bool() {
	return true;
}

HardwareSerial Serial(0);
#ifndef MCUlpc810
HardwareSerial Serial1(1);
#endif

