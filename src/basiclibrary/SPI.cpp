/*********************************************************************
TThis library file contains the SPI 0 communication API for Arduino Sketch to use.
SPI 0 port pins are pin D11(MOSI), D12(MISO) and D13(SCLK) on the board.  It is using SCI_SPI8 native driver.
The SS pin can be configured by the Arduino Sketch to any GPIO pin.

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

#include "SPI.h"
#include "main_thread.h"


SPI0::SPI0() {
}

uint8_t SPI0::begin(spi0_cs_pin_t cspin) {
    ssp_err_t err;
    device_cs_pin = cspin;

    if (device_cs_pin == SPI0_SSL0) {
        err = g_sf_spi_device0.p_api->open(g_sf_spi_device0.p_ctrl, g_sf_spi_device0.p_cfg);
    } else if (device_cs_pin == SPI0_SSL1) {
        err = g_sf_spi_device2.p_api->open(g_sf_spi_device2.p_ctrl, g_sf_spi_device2.p_cfg);
    } else {
        return -1;
    }

    if (SSP_SUCCESS == err)
        return 0;
    else
        return -1;
}
void  SPI0::read_transfer(char* const p_dest, int const length) {
    ssp_err_t err = SSP_SUCCESS;

    if (device_cs_pin == SPI0_SSL0) {
        err == g_sf_spi_device0.p_api->read(g_sf_spi_device0.p_ctrl, p_dest, length, SPI_BIT_WIDTH_8_BITS, TX_WAIT_FOREVER);
        APP_ERR_TRAP(err)
    } else if (device_cs_pin == SPI0_SSL1) {
        err == g_sf_spi_device2.p_api->read(g_sf_spi_device2.p_ctrl, p_dest, length, SPI_BIT_WIDTH_8_BITS, TX_WAIT_FOREVER);
        APP_ERR_TRAP(err)
    }

}
void  SPI0::write_transfer(char* const p_src, int const length) {

}
void  SPI0::readwrite_transfer(char* const p_dest, char* const p_src, int const length) {

}
