/*********************************************************************
This is support basic library for running Arduino Sketch code on Renesas AE Cloud2 S5D9 board


Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)

his header file includes the pin assignment for all digital input/output GPIO pins on the board.
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


#include "Arduino.h"

//#ifndef MILLIS_TIMER_H_
//#define MILLIS_TIMER_H_
volatile size_t milli_timer_counter = 0;   // 2^32 msec or 50 days.
//#endif

void delay (int time_ms){
    tx_thread_sleep (time_ms/10);  // divide by 10 because each sleep time slice is 10ms

}

/***************
 *
 * Check the header file to see whether the pin has already configured for another driver like PMW or SPI...
 *
 * @param pin_number
 * @param mode
 */
void pinMode(uint8_t pin_number, gpio_mode_t mode) {
    ioport_port_pin_t  pin_loc;
    bool               err_flag = false;
    ssp_err_t          status;

    switch (pin_number)
    {
        case 0:   pin_loc = D0;  break;
        case 1:   pin_loc = D1;  break;
        case 2:   pin_loc = D2;  break;
        case 3:   pin_loc = D3;  break;
        case 4:   pin_loc = D4;  break;
        case 5:   pin_loc = D5;  break;
        case 6:   pin_loc = D6;  break;
        case 7:   pin_loc = D7;  break;
        case 8:   pin_loc = D8;  break;
        case 9:   pin_loc = D9;  break;
        case 10:  pin_loc = D10;  break;
        case 11:  pin_loc = D11;  break;
        case 12:  pin_loc = D12;  break;
        case 13:  pin_loc = D13;  break;
        case 14:  pin_loc = D14;  break;
        case 15:  pin_loc = D15;  break;
        case 16:  pin_loc = D16;  break;
        default:  err_flag = true;
    }

    if (!err_flag) {
        if (mode == INPUT) {
            status = g_ioport.p_api->pinCfg(pin_loc,IOPORT_CFG_PORT_DIRECTION_INPUT);
            APP_ERROR_TRAP(status)
        } else if (mode == OUTPUT) {
            status = g_ioport.p_api->pinCfg(pin_loc,IOPORT_CFG_PORT_DIRECTION_OUTPUT);
            APP_ERROR_TRAP(status)
        } else if (mode == INPUT_PULLUP) {
            status = g_ioport.p_api->pinCfg(pin_loc,IOPORT_CFG_PORT_DIRECTION_INPUT);
            APP_ERROR_TRAP(status)
            status = g_ioport.p_api->pinCfg(pin_loc,IOPORT_CFG_PULLUP_ENABLE);
            APP_ERROR_TRAP(status)
        }
    }

}

void digitalWrite(uint8_t pin_number, uint8_t value) {
    ioport_port_pin_t  pin_loc;
    bool               err_flag = false;
    ssp_err_t          status;

    switch (pin_number)
     {
        case 0:   pin_loc = D0;  break;
        case 1:   pin_loc = D1;  break;
        case 2:   pin_loc = D2;  break;
        case 3:   pin_loc = D3;  break;
        case 4:   pin_loc = D4;  break;
        case 5:   pin_loc = D5;  break;
        case 6:   pin_loc = D6;  break;
        case 7:   pin_loc = D7;  break;
        case 8:   pin_loc = D8;  break;
        case 9:   pin_loc = D9;  break;
        case 10:  pin_loc = D10;  break;
        case 11:  pin_loc = D11;  break;
        case 12:  pin_loc = D12;  break;
        case 13:  pin_loc = D13;  break;
        case 14:  pin_loc = D14;  break;
        case 15:  pin_loc = D15;  break;
        case 16:  pin_loc = D16;  break;
        case 20:  pin_loc = D20;  break;  // GRN LED
        case 21:  pin_loc = D21;  break;  // YEL LED
        case 22:  pin_loc = D22;  break;  // RED LED
        default:  err_flag = true;
     }
    if (!err_flag) {
        status = g_ioport.p_api->pinWrite(pin_loc, (ioport_level_t) value);
        APP_ERROR_TRAP(status)
    }
}


uint8_t digitalRead(uint8_t pin_number) {
    ioport_port_pin_t  pin_loc;
    bool               err_flag = false;
    ssp_err_t          status;
    ioport_level_t     value;

    switch (pin_number)
     {
         case 0:   pin_loc = D0;  break;
         case 1:   pin_loc = D1;  break;
         case 2:   pin_loc = D2;  break;
         case 3:   pin_loc = D3;  break;
         case 4:   pin_loc = D4;  break;
         case 5:   pin_loc = D5;  break;
         case 6:   pin_loc = D6;  break;
         case 7:   pin_loc = D7;  break;
         case 8:   pin_loc = D8;  break;
         case 9:   pin_loc = D9;  break;
         case 10:  pin_loc = D10;  break;
         case 11:  pin_loc = D11;  break;
         case 12:  pin_loc = D12;  break;
         case 13:  pin_loc = D13;  break;
         case 14:  pin_loc = D14;  break;
         case 15:  pin_loc = D15;  break;
         case 16:  pin_loc = D16;  break;

         default:  err_flag = true;
     }
    if (!err_flag) {
        status = g_ioport.p_api->pinRead(pin_loc, &value);
        APP_ERROR_TRAP(status)
    }
    return (uint8_t) value;
}

// param: pin number
// return interrupt 0 or 1
uint8_t digitalPinToInterrupt(uint8_t pin_number){
    uint8_t intr = 0;
    switch (pin_number)
     {
         case 8:   intr = 0;  break;
         case 9:   intr = 1;  break;
         default:  intr = -1;  // error
     }
    return intr;
}


//  p_isr param: Not used in this subroutine.   Use the callback function below.
void attachInterrupt(uint8_t interrupt,
                     void (* p_isr)(void),
                     interrupt_trigger_t mode) {
    ssp_err_t err;


    if (interrupt == 0) {
        err = g_external_irq0.p_api->open(g_external_irq0.p_ctrl,g_external_irq0.p_cfg);
        APP_ERROR_TRAP(err)
        err = g_external_irq0.p_api->triggerSet(g_external_irq0.p_ctrl, (external_irq_trigger_t) mode);
        APP_ERROR_TRAP(err)
        err = g_external_irq0.p_api->enable(g_external_irq0.p_ctrl);
        APP_ERROR_TRAP(err)
    } else if (interrupt == 1) {
        err = g_external_irq1.p_api->open(g_external_irq1.p_ctrl,g_external_irq1.p_cfg);
        APP_ERROR_TRAP(err)
        err = g_external_irq1.p_api->triggerSet(g_external_irq1.p_ctrl, (external_irq_trigger_t) mode);
        APP_ERROR_TRAP(err)
        err = g_external_irq1.p_api->enable(g_external_irq1.p_ctrl);
        APP_ERROR_TRAP(err)
    }

}

// Turning off the interrupt for specific pin
void detachInterrupt(uint8_t interrupt){
    ssp_err_t err;

    if (interrupt == 0) {
        err = g_external_irq0.p_api->disable(g_external_irq0.p_ctrl);
        APP_ERROR_TRAP(err)
    } else if (interrupt == 1) {
        err = g_external_irq1.p_api->disable(g_external_irq1.p_ctrl);
        APP_ERROR_TRAP(err)
    }
}

// In the future, this may extend to other interrupts such as uart/i2c.
void interrupts(void) {
    ssp_err_t err;

    err = g_external_irq0.p_api->enable(g_external_irq0.p_ctrl);
    APP_ERROR_TRAP(err)
    err = g_external_irq1.p_api->enable(g_external_irq1.p_ctrl);
    APP_ERROR_TRAP(err)
}
// In the future, this may extend to other interrupts such as uart/i2c.
void noInterrupts(void){
    ssp_err_t err;

    err = g_external_irq0.p_api->disable(g_external_irq0.p_ctrl);
    APP_ERROR_TRAP(err)
    err = g_external_irq1.p_api->disable(g_external_irq1.p_ctrl);
    APP_ERROR_TRAP(err)
}

void ext_intr0_callback(external_irq_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    isr0();

}

void ext_intr1_callback(external_irq_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    isr1();

}

size_t millis(void) {
    return milli_timer_counter;
}

void millisec_timer_callback(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);

    milli_timer_counter++;
}


void analogWrite(uint8_t pin, uint8_t value) {
    ssp_err_t err;
    timer_size_t duty_cycle_time = 2 * (timer_size_t) value * COUNT_PER_MILLISEC / 256;

    if (pin == 5) {
        err = g_timer_pwm_d5.p_api->dutyCycleSet(g_timer_pwm_d5.p_ctrl, duty_cycle_time , TIMER_PWM_UNIT_RAW_COUNTS, PWM_PIN_B);
        APP_ERROR_TRAP(err)
    }
    if (pin == 6) {
        err = g_timer_pwm_d6.p_api->dutyCycleSet(g_timer_pwm_d6.p_ctrl, duty_cycle_time , TIMER_PWM_UNIT_RAW_COUNTS, PWM_PIN_B);
        APP_ERROR_TRAP(err)
    }

}


uint16_t analogRead(uint8_t pin) {

    ssp_err_t status;

    UINT ret;
    ULONG actual_flags;

    // Variable to hold ADC Data
    uint16_t adcCounts = 0;

    if (pin == 0 || pin ==3) {
        // Start ADC Scan
        g_adc0.p_api->scanStart (g_adc0.p_ctrl);

        ret = tx_event_flags_get(&g_adc_event_flags, NEW_EVENT_ADC0, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
        if ((actual_flags & (NEW_EVENT_ADC0 ) ) && (ret == TX_SUCCESS)) {
            // Read ADC
            if (pin == 0) {
                g_adc0.p_api->read (g_adc0.p_ctrl, ADC_REG_CHANNEL_20, &adcCounts);
            } else if (pin == 3) {
                g_adc0.p_api->read (g_adc0.p_ctrl, ADC_REG_CHANNEL_16, &adcCounts);
            } else {
                adcCounts = 0;
            }
        }
    }
    return adcCounts;

}

void adc0_callback (adc_callback_args_t *p_args) {
    if (p_args->event == ADC_EVENT_SCAN_COMPLETE) {
        tx_event_flags_set(&g_adc_event_flags, NEW_EVENT_ADC0, TX_OR);
    }

}

void system_setup(void) {
    ssp_err_t err;

    err = g_timer0.p_api->open (g_timer0.p_ctrl, g_timer0.p_cfg);
    APP_ERROR_TRAP(err)

    timer_size_t period_time = 2 * COUNT_PER_MILLISEC;  // 2.0 ms or 490 Hz same as Arduino UNO
    timer_size_t duty_cycle_time = 0;     // 50%

    err = g_timer_pwm_d5.p_api->open(g_timer_pwm_d5.p_ctrl, g_timer_pwm_d5.p_cfg);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d5.p_api->periodSet(g_timer_pwm_d5.p_ctrl,period_time,TIMER_UNIT_PERIOD_RAW_COUNTS);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d5.p_api->dutyCycleSet(g_timer_pwm_d5.p_ctrl, duty_cycle_time , TIMER_PWM_UNIT_RAW_COUNTS, PWM_PIN_B);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d5.p_api->start(g_timer_pwm_d5.p_ctrl);
    APP_ERROR_TRAP(err)

    err = g_timer_pwm_d6.p_api->open(g_timer_pwm_d6.p_ctrl, g_timer_pwm_d6.p_cfg);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d6.p_api->periodSet(g_timer_pwm_d6.p_ctrl,period_time,TIMER_UNIT_PERIOD_RAW_COUNTS);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d6.p_api->dutyCycleSet(g_timer_pwm_d6.p_ctrl, duty_cycle_time , TIMER_PWM_UNIT_RAW_COUNTS, PWM_PIN_B);
    APP_ERROR_TRAP(err)
    err = g_timer_pwm_d6.p_api->start(g_timer_pwm_d6.p_ctrl);
    APP_ERROR_TRAP(err)

    // Open the ADC
    g_adc0.p_api->open (g_adc0.p_ctrl, g_adc0.p_cfg);

    // Configure Scan
    g_adc0.p_api->scanCfg (g_adc0.p_ctrl, g_adc0.p_channel_cfg);

}


long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

