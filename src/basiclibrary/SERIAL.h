/*********************************************************************
This library file contains the Serial 0 communication API for Arduino Sketch to use.
Serial 0 port pins are pin D0(TX) and D1(RX) on the board.  It is using SCI_UART0 native driver.


Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)


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

#include "hal_data.h"  // type

#ifndef DRIVERS_SERIAL_H_
#define DRIVERS_SERIAL_H_

#define NEW_EVENT_TX0 0x00000001   // For tx events.



#define CR   0x0D  // Carriage Return
#define LF   0x0A  // Line Feed


#define BUFSIZE0  64  // read buffer size (spec from Serial libarary)

static char g_buf0[BUFSIZE0];           // max number of bytes per line??
static char g_event_ack0  = 1;
volatile static uint8_t buf_wrptr0 = 0;   // next byte to write
volatile static uint8_t buf_rdptr0 = 0;  // next byte to read
volatile static uint8_t datacount0 = 0;

#ifndef FORMAT_H_
#define FORMAT_H_
typedef enum { DEC=0, HEX, OCT, BIN } format_t;
#endif /* DRIVERS_SERIAL_H_ */

class SERIAL {
  public:
    SERIAL();
    void  begin(uint16_t baud_rate_param = 9600);
    uint8_t   available (void);
    char  read   (void);
    size_t write(char *buf, size_t len);
    size_t write (char * str);
    size_t write (char value);
    void  print  (int number);
    void  print  (float number);
    void  print  (char * str);
    void  println (int number);
    void  println(char * str);
    void  println(void);
    void  print  (int number, format_t format);
    void  println(int number, format_t format);
  private:
    uint16_t baud_rate = 9600;  // uart speed
    //uint16_t  = 0;         // number of characters read.

};


void user_event_uart0_callback(uart_callback_args_t *p_args);

#endif /* DRIVERS_SERIAL_H_ */
