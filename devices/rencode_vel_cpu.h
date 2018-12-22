#ifndef RENCODE_VEL_CPU_H
#define RENCODE_VEL_CPU_H
// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// -----------------------------------------------------------------------------   
#include "../timer.h"

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// atmega328 using timer 2
#ifdef ATMEGA328
// ------------
#define REV_TIMER_START() T2_START()
#define REV_TIMER_STOP()  T2_STOP()
#define REV_TIMER_RESET()  T2_WRITE(0)

// timer compare match ISR name
#define REV_CM_ISR TIMER2_COMPA_vect

#if F_CPU == 16000000
#pragma message("Using F_CPU 16MHz, ATMEGA328, timer2")    
static inline void rev_init_timer()
{
    // normal mode, output pin disabled
    T2_OC2A_NONPWM_NORMAL();
    // set CTC mode - clear timer on compare match
    T2_WGM_CTC();
    // divide by 64, timer is clocked at 250kHz, 4us
    T2_CS_PRE_64();
    // interrupt on terminal count of 250 for interrupt period of 1ms
    T2_OCR2A(250);
    // enable compare match interrupt
    T2_CMA_E();
}
// ------------
#else    
#error "Not implemented for this F_CPU frequency"
#endif
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------
#else
#error "Not implemented for this processor"
#endif



#endif
