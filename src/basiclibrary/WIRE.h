
/*********************************************************************
TThis library file contains the WIRE 0 or I2C communication API for Arduino Sketch to use.
Wire 0 port pins are pin ADC5(SCL) and ADC4(SDA) on the board.  It is using SCI_I2C0 native driver.

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


#ifndef DRIVERS_WIRE_H_
#define DRIVERS_WIRE_H_

#define APP_ERR_TRAP(a)             if(a) {__asm("BKPT #0\n");} /* trap the error location */


typedef bool boolean;

#define READBUFSIZE 20   // Maximum read buffer size
#define WRITEBUFSIZE 20   // Maximum write buffer size

/*
0:success
1:data too long to fit in transmit buffer
2:received NACK on transmit of address
3:received NACK on transmit of data
4:other error
*/

#define WIRE_SUCCESS 0
#define WIRE_ERROR   4

class WIRE {
  public:
    WIRE();
    uint8_t  begin();

    // read n bytes from the slave device
    uint16_t requestFrom(uint16_t const address, uint16_t const quantity=1, bool stop=true);
    uint16_t available(void);    // check for number of bytes read
    uint8_t  read();              // return one byte at a time from the read buffer
    uint8_t  beginTransmission(uint8_t const address);
    uint16_t write(uint8_t const data);
    uint16_t endTransmission(void);
  private:
    uint16_t slave_address = 0;     // i2c address
    uint16_t byte_read_length = 0;  // number of bytes to read
    bool read_complete_flag = false;// set to true by WIRE::requestFrom, set to false by WIRE::Available
    uint8_t  read_data_buffer[READBUFSIZE];// stored buffer of the read data
    uint16_t read_buffer_index = 0; // read buffer index
    uint8_t  write_data_buffer[WRITEBUFSIZE];// stored buffer of the write data
    uint16_t write_buffer_index = 0; // write buffer index

};

#endif /* DRIVERS_WIRE_H_ */
