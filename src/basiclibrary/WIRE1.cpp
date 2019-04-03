/*********************************************************************
This library file contains the WIRE 1 or I2C communication API for Arduino Sketch to use.

Wire 1 port is also mapped to I2C grove header port.  The pins are pin #1(SCL) and #2(SDA) on the header.
It is using RIIC_I2C1 native driver.


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


#include "WIRE1.h"
#include "main_thread.h"
#define  I2CTIMEOUT1  100   // time out for i2c read


// An option is added to skip writing register address for devices such as SHT31.
// Such device can output data and does not require the register address specified.
int8_t synergy_i2c1_read_option(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len, bool skip_reg_addr)
{
    uint8_t wBuf[2];
    ssp_err_t ssp_err = SSP_SUCCESS;

    //Lock the bus for exclusive access
    tx_mutex_get(&g_i2c1_mutex, TX_WAIT_FOREVER);

    ssp_err = g_i2c1.p_api->slaveAddressSet(g_i2c1.p_ctrl, dev_addr, I2C_ADDR_MODE_7BIT);
    if(ssp_err != SSP_SUCCESS)
    {
        tx_mutex_put(&g_i2c1_mutex);
        APP_ERR_TRAP(ssp_err);
    }

    if (!skip_reg_addr) {
        wBuf[0] = reg_addr;
        ssp_err = g_i2c1.p_api->write(g_i2c1.p_ctrl, wBuf,1,true);
        if(ssp_err != SSP_SUCCESS)
        {
            tx_mutex_put(&g_i2c1_mutex);
            APP_ERR_TRAP(ssp_err);
        }
    }

    ssp_err = g_i2c1.p_api->read(g_i2c1.p_ctrl, data, len, false );
    if(ssp_err != SSP_SUCCESS)
    {
        tx_mutex_put(&g_i2c1_mutex);
        APP_ERR_TRAP(ssp_err);
    }

    //unlock the bus
    tx_mutex_put(&g_i2c1_mutex);
    return 0;
}

// An option is added to skip writing register address for devices such as SHT31.
// Such device take commands only and does not require the register address specified.
int8_t synergy_i2c1_write_option(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len, bool skip_reg_addr)
{
    ssp_err_t ssp_err = SSP_SUCCESS;
    uint8_t wBuf[8];
    uint8_t i=0,j = 0;

    //Lock the bus for exclusive access
    tx_mutex_get(&g_i2c1_mutex, TX_WAIT_FOREVER);

    ssp_err = g_i2c1.p_api->slaveAddressSet(g_i2c1.p_ctrl, dev_addr, I2C_ADDR_MODE_7BIT);
    if(ssp_err != SSP_SUCCESS)
    {
        tx_mutex_put(&g_i2c1_mutex);
        APP_ERR_TRAP(ssp_err);
    }

    if (!skip_reg_addr) {
        wBuf[j++] = reg_addr;

        for(j = 1; j <= len; j++,i++)
            wBuf[j] = data[i];

        ssp_err = g_i2c1.p_api->write(g_i2c1.p_ctrl, wBuf, (uint32_t)(len+1), false);   // reg_addr + write data
    } else {
        ssp_err = g_i2c1.p_api->write(g_i2c1.p_ctrl, data, (uint32_t)(len), false);     // write data only.
    }

    if(ssp_err != SSP_SUCCESS)
    {
        tx_mutex_put(&g_i2c1_mutex);
        APP_ERR_TRAP(ssp_err);
    }

    //unlock the bus
    tx_mutex_put(&g_i2c1_mutex);

    return 0;
}



WIRE1::WIRE1() {
}

uint8_t WIRE1::begin() {

    if (SSP_SUCCESS == g_i2c1.p_api->open(g_i2c1.p_ctrl, g_i2c1.p_cfg))
        return 0;
    else
        return -1;
}

// Input : slave i2c address, quantity (# of bytes to read), stop (No restart option)
// Return : # of bytes to read
uint16_t WIRE1::requestFrom(uint16_t const address, uint16_t const quantity, bool stop){
    //char buffer[READBUFSIZE];  // later the buffer size needs to be flexible.

    slave_address = address;
    read_complete_flag = false;

    if (READBUFSIZE1 < quantity) {
        return 0;   // return zero because it is unable to read that many bytes due to the limited read buffer size.
    }

    //if (SSP_SUCCESS != g_i2c1.p_api->slaveAddressSet(g_i2c1.p_ctrl, address, I2C_ADDR_MODE_7BIT)) {
    //    return 0;   // return zero because the slave address cannot be set.
    //}
    slave_address = address;
    byte_read_length = quantity;

    if (SSP_SUCCESS == synergy_i2c1_read_option(slave_address, 0, read_data_buffer,quantity, true)){
       read_complete_flag = true;
       read_buffer_index = 0;
       return(quantity);   // return number of bytes read.
    } else {
       return 0;  // return zero for read failure
    }
}

uint16_t WIRE1::available(){
    if (read_complete_flag)
        return byte_read_length;
    else
        return 0;
}

uint8_t WIRE1::read() {
    if (read_buffer_index < byte_read_length) {
        return (read_data_buffer[read_buffer_index++]);
    } else {
        return '\0';   // end of the buffer
    }
}

uint8_t WIRE1::beginTransmission(uint8_t const address){
    //if (SSP_SUCCESS == g_sf_i2c_device0.p_api->slaveAddressSet(g_i2c1.p_ctrl, address, I2C_ADDR_MODE_7BIT)) {
    //    write_buffer_index = 0;
    //    return 0;
    //} else {
    //    return -1;
    //}
    slave_address = address;
    write_buffer_index = 0;
    return 0;
}

// Store each byte in the write buffer.
// limit to 1 data for now.
uint16_t WIRE1::write(uint8_t const data){
    if (write_buffer_index < WRITEBUFSIZE1) {
        write_data_buffer[write_buffer_index++] = data;
        return 1;
    } else {
        return 0;   // end of the buffer
    }
}

uint16_t WIRE1::endTransmission(void){
    if (SSP_SUCCESS == synergy_i2c1_write_option(slave_address, 0, write_data_buffer, write_buffer_index, true)) {
        return WIRE1_SUCCESS;
    } else {
        return WIRE1_ERROR;
    }
}
