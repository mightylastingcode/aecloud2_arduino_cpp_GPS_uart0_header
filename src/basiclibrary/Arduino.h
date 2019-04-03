/*********************************************************************
This is support basic library for running Arduino Sketch code on Renesas AE Cloud2 S5D9 board

Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)

This header file includes the pin assignment for all digital input/output GPIO pins on the board.
The LED pins are defined here and are already configured internally as GPIO output pins.

1. D0-D13 GPIO pins
2. D20,21,22 GPIO output pins for the onboard leds.

This file also contains the PMW functions for the following pins.

1. D5,D6

This file also contains the interrupt pin configuration and callback functuions for the following interrupt pins.

1. D8 (INT0), D9 (INT1)

This file also contains the ADC read functions for the following input pins.

1. ADC0, ADC3

Note: There are other pins which can be configured for ADC, PWM, or INT.  They would be required to configured with
the Renesas's Thread configuration Window.  This Arduino library takes advantage of the pre-configured pins to ease
the developers from understanding the Renesas's configuration method until the developers have understood it in depth.

The MIT License (MIT)


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*********************************************************************/


#ifndef DRIVERS_HELPER_H_
#define DRIVERS_HELPER_H_

#include "main_thread.h"

#define APP_ERROR_TRAP(x)    if (x) {\
                                   g_ioport.p_api->pinWrite(LED3R, IOPORT_LEVEL_HIGH);\
                                    __BKPT();}


typedef enum { INPUT=0, OUTPUT, INPUT_PULLUP } gpio_mode_t;
typedef enum { FALLING = 0, RISING, CHANGE, LOW_LEVEL  } interrupt_trigger_t;

typedef char byte;

#define HIGH 1
#define LOW  0



#define D0   IOPORT_PORT_06_PIN_14  //GPIO IO[0] UART TXD7
#define D1   IOPORT_PORT_06_PIN_13  //GPIO IO[1] UART RXD7
#define D2   IOPORT_PORT_06_PIN_12  //GPIO IO[2] GPIO
#define D3   IOPORT_PORT_06_PIN_11  //GPIO IO[3] GPIO
#define D4   IOPORT_PORT_06_PIN_09  //GPIO IO[4] GPIO
#define D5   IOPORT_PORT_06_PIN_02  //GPIO IO[5] PMW (ie 3 pins RBG led)
#define D6   IOPORT_PORT_01_PIN_02  //GPIO IO[6] PMW
#define D7   IOPORT_PORT_01_PIN_13  //GPIO IO[7] PMW

#define D8   IOPORT_PORT_01_PIN_11  //GPIO IO[8] INT0
#define D9   IOPORT_PORT_01_PIN_10  //GPIO IO[9] INT1
#define D10  IOPORT_PORT_01_PIN_07  //GPIO IO[10] SPI SS8
#define D11  IOPORT_PORT_01_PIN_05  //GPIO IO[11] SPI MOSI8
#define D12  IOPORT_PORT_01_PIN_04  //GPIO IO[12] SPI MISO8
#define D13  IOPORT_PORT_01_PIN_06  //GPIO IO[13] SPI SCK8

#define D14  IOPORT_PORT_04_PIN_12  //GPIO IO[14] GPIO  (PMOD Pin 8)
#define D15  IOPORT_PORT_03_PIN_07  //GPIO IO[15] GPIO  (PMOD Pin 9)
#define D16  IOPORT_PORT_03_PIN_06  //GPIO IO[16] GPIO  (PMOD Pin 10)



#define D20 IOPORT_PORT_07_PIN_13   // RED
#define D21 IOPORT_PORT_07_PIN_10   // Yellow
#define D22 IOPORT_PORT_07_PIN_08   // Green

// For internal use only.
#define LED3R IOPORT_PORT_07_PIN_11
#define LED2Y IOPORT_PORT_07_PIN_10
#define LED1G IOPORT_PORT_07_PIN_08

// For general user
void pinMode(uint8_t pin_number, gpio_mode_t mode);
void digitalWrite(uint8_t pin_number, uint8_t value);
uint8_t digitalRead(uint8_t pin_number);
uint8_t digitalPinToInterrupt(uint8_t pin_number);

void attachInterrupt(uint8_t interrupt,
                     void (* p_isr)(void),
                     interrupt_trigger_t mode);

void detachInterrupt(uint8_t interrupt);
void interrupts(void);
void noInterrupts(void);

extern void isr0(void);
extern void isr1(void);


void delay (int time_ms);
size_t millis(void);


void analogWrite(uint8_t pin, uint8_t value);
uint16_t analogRead(uint8_t pin);

// For internal use only

#define COUNT_PER_MILLISEC   (120E6/1000)  // number of counts per 1 msec based on CLKA (S5D9)
#define PWM_PIN_A         0   // 0 for A pin
#define PWM_PIN_B         1   // 1 for B pin
#define NEW_EVENT_ADC0    0x00000001

void system_setup(void);

void millisec_timer_callback(timer_callback_args_t * p_args);
void ext_intr0_callback(external_irq_callback_args_t * p_args);
void ext_intr1_callback(external_irq_callback_args_t * p_args);
void adc0_callback (adc_callback_args_t *p_args);
long map(long x, long in_min, long in_max, long out_min, long out_max);

#endif /* DRIVERS_HELPER_H_ */
