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

size_t HardwareSerial::write(const char *str) {
	return Chip_UART_SendBlocking(_uart,(const uint8_t *)str, strlen(str));
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
  return Chip_UART_SendBlocking(_uart,buffer,size);
}

HardwareSerial::operator bool() {
	return true;
}

HardwareSerial Serial(0);
#ifndef MCUlpc810
HardwareSerial Serial1(1);
#endif

// for printf
extern "C" {
	void uart_putchar(char c)
	{
		Serial.print(c);
	}
}