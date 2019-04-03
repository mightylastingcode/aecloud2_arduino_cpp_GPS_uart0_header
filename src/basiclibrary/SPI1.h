/*********************************************************************
TThis library file contains the SPI 1 communication API for Arduino Sketch to use.
SPI 1 port pins are pin #1(SS), pin #2(MOSI), #3(MISO) and #4(SCLK) of the PMOD header of the board.
It is using R_SPI1 native driver.


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

#ifndef BASICLIBRARY_SPI1_H_
#define BASICLIBRARY_SPI1_H_

#include "hal_data.h"  // type

#define APP_ERR_TRAP(a)             if(a) {__asm("BKPT #0\n");} /* trap the error location */

typedef enum { SPI1_SSL0=0, SPI1_SSL1 } spi1_cs_pin_t;

class SPI1 {  // SPI1 bus
  public:
    SPI1();
    uint8_t  begin(spi1_cs_pin_t cspin);

    // read n bytes from the slave device
    void  read_transfer(char* const p_dest, int const length);
    void  write_transfer(char* const p_src, int const length);
    void  readwrite_transfer(char* const p_dest, char* const p_src, int const length);
  private:
    spi1_cs_pin_t device_cs_pin = SPI1_SSL0;     // device select pin
};


#endif /* BASICLIBRARY_SPI1_H_ */
