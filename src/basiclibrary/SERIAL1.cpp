/*********************************************************************
This library file contains the Serial 1 communication API for Arduino Sketch to use.

Serial 1 port pins are pin ADC1(TX) and ADC2(RX) on the board.  It is using SCI_UART1 native driver.

Serial 1 port is also mapped to UART grove header port.  The pins are pin #1(TX) and #2(RX) on the header.
It is using SCI_UART1 native driver.



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
#include "SERIAL1.h"
#include "hal_data.h"  // type
#include <stdio.h>     // sprintf
#include "string.h"

#define LED3R IOPORT_PORT_01_PIN_13
#define LED2Y IOPORT_PORT_01_PIN_03
#define LED1G IOPORT_PORT_01_PIN_02

const char *byte_to_binary1(int x);

void uart1_write (char *buf, int len) {
    UINT ret;
    ULONG actual_flags;

    g_event_ack1 = 1;
    g_uart1.p_api->write(g_uart1.p_ctrl,(const uint8_t*) buf,len);
    ret = tx_event_flags_get(&g_uart_event_flags, NEW_EVENT_TX1, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if ((actual_flags & NEW_EVENT_TX1) && (ret == TX_SUCCESS)) {
        g_event_ack1 = 0;
    }
}


SERIAL1::SERIAL1() {
}

// baud rate: (default 9600), 1200, 2400, 4800, 19200, 38400, and 57600.
//
//  Note: S5D9 external UART: 57600 is max baud rate achievable. 115200 rate will not work.
void  SERIAL1::begin(uint16_t baud_rate_param) {
    if (SSP_SUCCESS != g_uart1.p_api->open(g_uart1.p_ctrl, g_uart1.p_cfg)) {
        g_ioport.p_api->pinWrite(LED3R, IOPORT_LEVEL_HIGH);
        while(1);
    }

    baud_rate = baud_rate_param;
    if (SSP_SUCCESS != g_uart1.p_api->baudSet(g_uart1.p_ctrl,baud_rate_param)) {
        g_ioport.p_api->pinWrite(LED3R, IOPORT_LEVEL_HIGH);
        while(1);
    }
}


uint8_t SERIAL1::available (void) {
    return (datacount1);
}

// return the data byte or -1 if no data available
char SERIAL1::read (void) {
    char bytedata;
    if (!datacount1) {
        return -1;
    } else {
        datacount1--;  // update this immediately before next call back.
        bytedata = g_buf1[buf_rdptr1];
        if (buf_rdptr1 < BUFSIZE1)
            buf_rdptr1++;
        else
            buf_rdptr1 = 0;
        return bytedata;
    }
}

void  SERIAL1::print(char * str) {
    uart1_write (str, strlen(str));
}

void  SERIAL1::println(char * str) {

    uart1_write (str, strlen(str));
    uart1_write ("\r\n", strlen("\r\n"));
}

void  SERIAL1::println(void) {
    uart1_write ("\r\n", strlen("\r\n"));
}

void  SERIAL1::print (float number){
    char g_buffer[64];

    sprintf(g_buffer, "%5.2f", number);
    uart1_write (g_buffer, strlen(g_buffer));
}

void  SERIAL1::print (int number){
    char g_buffer[64];

    sprintf(g_buffer, "%d", number);
    uart1_write (g_buffer, strlen(g_buffer));
}

void  SERIAL1::print (int number, format_t format){
    char g_buffer[64];

    if (format == DEC)
        sprintf(g_buffer, "%d", number);
    else if (format == HEX)
        sprintf(g_buffer, "%X", number);
    else if (format == OCT)
        sprintf(g_buffer, "%o", number);
    else if (format == BIN)
        sprintf(g_buffer, "%s", byte_to_binary1(number));
    else
        sprintf(g_buffer, "%d", number);
    uart1_write (g_buffer, strlen(g_buffer));
}


void  SERIAL1::println (int number){
     char g_buffer[64];

     sprintf(g_buffer, "%d", number);
     uart1_write (g_buffer, strlen(g_buffer));
     uart1_write ("\r\n", strlen("\r\n"));
}

void  SERIAL1::println (int number, format_t format){
     SERIAL1::print(number,format);
     uart1_write ("\r\n", strlen("\r\n"));
}

// write a string fo characters.
size_t  SERIAL1::write(char * str) {
    uart1_write (str, strlen(str));
    return strlen(str);
}

// Write an array of byte data.  The size is defined by the len value.
size_t  SERIAL1::write(char *buf, size_t len) {
    uart1_write (buf, len);
    return len;
}

// write a single byte data.
size_t  SERIAL1::write(char value) {
    char str[2];

    str[0] = value;
    str[1] = '\0';
    uart1_write (str, strlen(str));
    return 1;
}

const char *byte_to_binary1(int x)
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
void user_event_uart1_callback(uart_callback_args_t *p_args) {

    if (p_args->event == UART_EVENT_RX_CHAR) {
        if (datacount1 <= BUFSIZE1) {   // any unused buffer slot?
            g_buf1[buf_wrptr1] = (char)p_args->data;
            if (buf_wrptr1 < (BUFSIZE1-1))
                buf_wrptr1++;
            else
                buf_wrptr1 = 0;

            datacount1++;
        }
    }
    if ((p_args->event == UART_EVENT_TX_COMPLETE) && g_event_ack1) {
            g_event_ack1 = 0;
            tx_event_flags_set(&g_uart_event_flags, NEW_EVENT_TX1, TX_OR);
    }
}


