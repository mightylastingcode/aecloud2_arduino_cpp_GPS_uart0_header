/*********************************************************************
This is the main thread for placing Arduino Sketch code to run on Renesas AE Cloud2 S5D9 board

Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)

An Arduino sketch is placed inside the section that is defined by two comment lines.  The section has the
example of setup() and loop() functions.   You can replace them with your example code.  It is very simple.
Most common functions like Serial and Wire functions are supported except of the SPI functions which need to
be modified to run.


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



#include "main_thread.h"
#include <Arduino.h>
#include <Wire.h>
#include <Wire1.h>
#include "SERIAL.h"
#include "SERIAL1.h"
#include "SPI.h"
#include "SPI1.h"
#include <stdio.h>


//====================== Your Arduino Example Sketch Begin ===========//
SERIAL1 Serial  = SERIAL1();   //UART 1

void setup() {
    Serial.begin(9600);
    //while(!Serial);
    Serial.println("begin uart1 grove port...");
}

int counter=0;
void loop() {
    int incomingByte = 0;   // for incoming serial data
    char str[2] = " ";

    bool wait_flag = true;
    // reply only when you receive data:
     if (Serial.available() > 0) {
         // read the incoming byte:
         //if (wait_flag) {
         //    delay(10);
         //    wait_flag = false;
         //}

         incomingByte = Serial.read();
         str[0] = incomingByte;

         // say what you got:
         counter++;
         Serial.print(counter,DEC);
         Serial.print("I received: ");
         Serial.print(str);
         Serial.print("   ASCII value: ");
         Serial.println(incomingByte, DEC);
     }
}

//====================== Your Arduino Example Sketch End ===========//



//============================= Renesas Main Thread entry function ====================================//
void main_thread_entry(void)
{
   system_setup();
   setup();

    while (1) {
        loop();
        delay(1);  // required for the thread if delay()does not exist in the loop() function.
    }
}

