/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.

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
 */

#include <Arduino.h>

#include "Servo.h"
#include "HardwareSerial.h"
int sct_enabled=false;

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[1 ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

uint8_t ServoCount = 0;                                     // the total number of attached servos

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

static void initSCT()
{
    Chip_SCT_Init(LPC_SCT);
	
    LPC_SCT->EVENT[0].STATE = 1<<0; // state 0
    LPC_SCT->EVENT[0].CTRL = (1<<12) | (1<<14);

    LPC_SCT->LIMIT_L |= 1<<0; // event 0

    LPC_SCT->CONFIG |= (1<<0) | (1<<17); // unify, auto limit
    LPC_SCT->CTRL_L |= (SystemCoreClock/1000000-1)<<5; // set prescaler, SCT clock = 1 MHz

    LPC_SCT->CTRL_L &= ~(1<<2); // unhalt it by clearing bit 2 of CTRL reg	
	
	// 20ms period
	LPC_SCT->MATCHREL[0].U = REFRESH_INTERVAL;	
	
	sct_enabled=true;
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++; // assign a servo index to this instance
	servos[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH;
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  if(this->servoIndex < MAX_SERVOS ) {
	 byte channel = this->servoIndex;
	  
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;

    // initialize the timer if it has not already been initialized
	if(sct_enabled==false) {
		initSCT();
	}

    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);
	switch(channel) {
		case 0:
	    Chip_SWM_MovablePinAssign(SWM_CTOUT_0_O, pin);
		break;
		case 1:
	    Chip_SWM_MovablePinAssign(SWM_CTOUT_1_O, pin);		
		break;
		case 2:
	    Chip_SWM_MovablePinAssign(SWM_CTOUT_2_O, pin);		
		break;
		
	}
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);

	LPC_SCT->EVENT[channel+1].STATE = LPC_SCT->EVENT[0].STATE;
	LPC_SCT->EVENT[channel+1].CTRL = LPC_SCT->EVENT[0].CTRL | (channel+1)<<0; // match 1...5
 
	LPC_SCT->OUT[channel].SET = 1<<0; // event 0
	LPC_SCT->OUT[channel].CLR = 1<<channel+1; // event 1...5

    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  return this->servoIndex ;
}

void Servo::detach()
{
  servos[this->servoIndex].Pin.isActive = false;
  // TODO
}

void Servo::write(int value)
{
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0) value = 0;
    if(value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }

  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();

	LPC_SCT->MATCHREL[channel+1].U=value;

    servos[channel].ticks = value;
  }
}

int Servo::read() // return the value as degrees
{
  return  map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = servos[this->servoIndex].ticks;
  else
    pulsewidth  = 0;

  return pulsewidth;
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive ;
}


