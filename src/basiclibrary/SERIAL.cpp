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

#include "main_thread.h"  // TX_WAIT_FOREVER
#include "SERIAL.h"
#include "hal_data.h"  // type
#include <stdio.h>     // sprintf
#include "string.h"

#define LED3R IOPORT_PORT_01_PIN_13
#define LED2Y IOPORT_PORT_01_PIN_03
#define LED1G IOPORT_PORT_01_PIN_02

const char *byte_to_binary(int x);

void uart_write (char *buf, int len) {
    UINT ret;
    ULONG actual_flags;

    g_event_ack0 = 1;
    g_uart0.p_api->write(g_uart0.p_ctrl,(const uint8_t*) buf,len);
    ret = tx_event_flags_get(&g_uart_event_flags, NEW_EVENT_TX0, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if ((actual_flags & NEW_EVENT_TX0) && (ret == TX_SUCCESS)) {
        g_event_ack0 = 0;
    }
}


SERIAL::SERIAL() {
}

// baud rate: (default 9600), 1200, 2400, 4800, 19200, 38400, and 57600.
//
//  Note: S5D9 external UART: 57600 is max baud rate achievable. 115200 rate will not work.
void  SERIAL::begin(uint16_t baud_rate_param) {
    if (SSP_SUCCESS != g_uart0.p_api->open(g_uart0.p_ctrl, g_uart0.p_cfg)) {
        g_ioport.p_api->pinWrite(LED3R, IOPORT_LEVEL_HIGH);
        while(1);
    }

    baud_rate = baud_rate_param;
    if (SSP_SUCCESS != g_uart0.p_api->baudSet(g_uart0.p_ctrl,baud_rate_param)) {
        g_ioport.p_api->pinWrite(LED3R, IOPORT_LEVEL_HIGH);
        while(1);
    }
}


uint8_t SERIAL::available (void) {
    return (datacount0);
}

// return the data byte or -1 if no data available
char SERIAL::read (void) {
    char bytedata;
    if (!datacount0) {
        return -1;
    } else {
        datacount0--;  // update this immediately before next call back.
        bytedata = g_buf0[buf_rdptr0];
        if (buf_rdptr0 < BUFSIZE0)
            buf_rdptr0++;
        else
            buf_rdptr0 = 0;
        return bytedata;
    }
}

void  SERIAL::print(char * str) {
    uart_write (str, strlen(str));
}

void  SERIAL::println(char * str) {

    uart_write (str, strlen(str));
    uart_write ("\r\n", strlen("\r\n"));
}

void  SERIAL::println(void) {
    uart_write ("\r\n", strlen("\r\n"));
}

void  SERIAL::print (float number){
    char g_buffer[64];

    sprintf(g_buffer, "%5.2f", number);
    uart_write (g_buffer, strlen(g_buffer));
}

void  SERIAL::print (int number){
    char g_buffer[64];

    sprintf(g_buffer, "%d", number);
    uart_write (g_buffer, strlen(g_buffer));
}

void  SERIAL::print (int number, format_t format){
    char g_buffer[64];

    if (format == DEC)
        sprintf(g_buffer, "%d", number);
    else if (format == HEX)
        sprintf(g_buffer, "%X", number);
    else if (format == OCT)
        sprintf(g_buffer, "%o", number);
    else if (format == BIN)
        sprintf(g_buffer, "%s", byte_to_binary(number));
    else
        sprintf(g_buffer, "%d", number);
    uart_write (g_buffer, strlen(g_buffer));
}

void  SERIAL::println (int number){
    char g_buffer[64];

    sprintf(g_buffer, "%d", number);
    uart_write (g_buffer, strlen(g_buffer));
    uart_write ("\r\n", strlen("\r\n"));
}


void  SERIAL::println (int number, format_t format){
     SERIAL::print(number,format);
     uart_write ("\r\n", strlen("\r\n"));
}

// write a string fo characters.
size_t  SERIAL::write(char * str) {
    uart_write (str, strlen(str));
    return strlen(str);
}

// Write an array of byte data.  The size is defined by the len value.
size_t  SERIAL::write(char *buf, size_t len) {
    uart_write (buf, len);
    return len;
}

// write a single byte data.
size_t  SERIAL::write(char value) {
    char str[2];

    str[0] = value;
    str[1] = '\0';
    uart_write (str, strlen(str));
    return 1;
}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

/*
 * A simple uart byte reciver and transmitter
 */
void user_event_uart0_callback(uart_callback_args_t *p_args) {

    if (p_args->event == UART_EVENT_RX_CHAR) {
        if (datacount0 <= BUFSIZE0) {   // any unused buffer slot?
            g_buf0[buf_wrptr0] = (char)p_args->data;
            if (buf_wrptr0 < (BUFSIZE0-1))
                buf_wrptr0++;
            else
                buf_wrptr0 = 0;

            datacount0++;
        }
    }
    if ((p_args->event == UART_EVENT_TX_COMPLETE) && g_event_ack0) {
            g_event_ack0 = 0;
            tx_event_flags_set(&g_uart_event_flags, NEW_EVENT_TX0, TX_OR);
    }
}

