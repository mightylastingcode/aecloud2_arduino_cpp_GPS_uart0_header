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

// Class Instance Instantiation
SERIAL1 Serial  = SERIAL1();   //UART 1  PC
SERIAL  GPS  = SERIAL();   //UART 0  GPS

// Constant
#define  G_LENGTH  128   // Max length of the protocol payload
#define  LF_CHAR    10
#define  NULL_CHAR   0

// Subroutine Prototype Declaration
float lat_convert(char *lat, char ns_dir);
float long_convert(char *long_s, char ew_dir);
void process_gps_data(char *str);

// Main Sketch (Setup & Loop)
void setup() {
    Serial.begin(9600);
    //while(!Serial);
    Serial.println("begin uart1 grove port...");
    GPS.begin(9600);
}

bool wait_flag = true;
bool end_flag = false;
bool start_flag = false;

void loop() {
    int incomingByte = 0;   // for incoming serial data
    // Test examples
    //char str[G_LENGTH] = "$GPRMC,164345.000,A,3722.9791,N,12151.5976,W,0.40,204.24,030419,,,A*7F";
    //char str[G_LENGTH] = "$GPRMC 164345.000 A 3722.9791 N 12151.5976 W 0.40 204.24 030419   A*7F";
    char str[G_LENGTH];
    int index;


    //process_gps_data(str);
    //while (true);
    //Example : $GPRMC,164345.000,A,3722.9791,N,12151.5976,W,0.40,204.24,030419,,,A*7F
    while (true) {
         if (GPS.available() > 0) {

             incomingByte = GPS.read();
             if (incomingByte == '$') {
                 start_flag = true;
                 end_flag   = false;
                 index      = 0;
             }
             if (incomingByte == LF_CHAR && start_flag == true) {
                 start_flag = false;
                 end_flag   = true;
                 str[index] = NULL_CHAR;
                 Serial.println(str);
                 process_gps_data(str);
             }

             if (start_flag) {
                 if (incomingByte == ',')
                     str[index++] = ' ';
                 else
                     str[index++] = incomingByte;
             }
         }

    }
}

//***********************************
// Subroutine Body Definition
//***********************************

// convert from ddmm.ss (degree format) to dd.xxxx (decimal format)
float lat_convert(char *lat, char ns_dir) {
    char buf[5];
    int  dd, mm, ss;

    buf[0] = lat[0];
    buf[1] = lat[1];
    buf[2] = '\0';
    dd = atoi(buf);

    buf[0] = lat[2];
    buf[1] = lat[3];
    buf[2] = '\0';
    mm = atoi(buf);

    buf[0] = lat[5];
    buf[1] = lat[6];
    buf[2] = '\0';
    ss = atoi(buf);

    float latitude;
    latitude = (float) dd + (float) mm / 60.0 + (float) ss / 3600.0;
    if (ns_dir == 'S')
        latitude = latitude * -1.0;
    return latitude;

}

// convert from ddmm.ss (degree format) to dd.xxxx (decimal format)
float long_convert(char *long_s, char ew_dir) {
    char buf[5];
    int  dd, mm, ss;

    buf[0] = long_s[0];
    buf[1] = long_s[1];
    buf[2] = long_s[2];
    buf[3] = '\0';
    dd = atoi(buf);

    buf[0] = long_s[3];
    buf[1] = long_s[4];
    buf[2] = '\0';
    mm = atoi(buf);

    buf[0] = long_s[6];
    buf[1] = long_s[7];
    buf[2] = '\0';
    ss = atoi(buf);

    float longitude;
    longitude = (float) dd + (float) mm / 60.0 + (float) ss / 3600.0;
    if (ew_dir == 'W')
        longitude = longitude * -1.0;
    return longitude;
}
void process_gps_data(char *str){
    int val;
    char id[10]; // message id
    char hhmmss_ss[10];
    char status;
    char lat_g[10];
    char ns_dir;
    char long_g[10];
    char ew_dir;
    char speed[10];
    char course[10];
    char date[10];

    char buf[2] = " ";

    //"$GPRMC 164345.000 A 3722.9791 N 12151.5976 W 0.40 204.24 030419   A*7F";
    if (str[1] == 'G' && str[2] == 'P' && str[3] == 'R' && str[4] == 'M' && str[5] == 'C' ) {
        Serial.println("===============================");
        Serial.println("Found GPRMC format");
        val = sscanf(str,"%s %s %c %s %c %s %c %s %s %s",id,hhmmss_ss, &status,lat_g, &ns_dir,
             long_g, &ew_dir, speed, course ,date);
        Serial.print("id =");
        Serial.println(id);
        Serial.print("HHMMSS.SS =");
        Serial.println(hhmmss_ss);
        Serial.print("Status =");
        buf[0] = status;
        Serial.println(buf);
        Serial.print("latitude =");
        Serial.println(lat_g);
        buf[0] = ns_dir;
        Serial.print("ns_dir =");
        Serial.println(buf);
        Serial.print("longitude =");
        Serial.println(long_g);
        buf[0] = ew_dir;
        Serial.print("ew_dir =");
        Serial.println(buf);
        Serial.print("speed =");
        Serial.println(speed);
        Serial.print("course =");
        Serial.println(course);
        Serial.print("date =");
        Serial.println(date);
        Serial.print("val =");
        Serial.println(val,DEC);

        float lat_dec;
        float long_dec;
        lat_dec = lat_convert(lat_g, ns_dir);
        long_dec = long_convert(long_g, ew_dir);

        char event[40];
        sprintf (event, "Latitude = %8.2f Longitude = %8.2f \n",lat_dec, long_dec);
        Serial.println(event);
        Serial.println("===============================");
        //while (true);
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

