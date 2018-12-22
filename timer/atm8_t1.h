#ifndef ATM8_T1_H
#define ATM8_T1_H
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

// Waveform generation settings:

// normal mode
#define T1_WGM_NORMAL()  t1_ctl.tccr1a = ( t1_ctl.tccr1a &~ (_BV(WGM_11) | _BV(WGM10) ) ; t1_ctl.tccr1a = (t1_ctl.tccr1b &~ ( _BV(WGM13 | _BV(WGM12)) )) 

// pwm phase correct, 8 bit
#define T1_WGM_PWMPC_8() t1_ctl.tccr1a = (t1_ctl.tccr1a &~ _BV()) | _BV(); t1_ctl.tccr1a = (t1_ctl.tccr1b &~ _BV()) | _BV()
// pwm phase correct, 9 bit
#define T1_WGM_PWMPC_9()
// pwm phase correct, 10 bit
#define T1_WGM_PWMPC_10()

// CTC, OCR1A to indicate TOP
#define T1_WGM_CTC_OCR1A()

// Fast PWM, 8 bit
#define T1_WGM_PWMF_8()
// Fast PWM, 9 bit
#define T1_WGM_PWMF_9()
// Fast PWM, 10 bit
#define T1_WGM_PWMF_10()

// PWM, Phase and frequency correct, ICR1 to indicate top
#define T1_WGM_PWMPFC_ICR1()
// PWM, Phase and frequency correct, OCR1A to indicate top
#define T1_WGM_PWMPFC_OCR1A()
// PWM phase correct, OCR1A to indicate top
#define T1_WGM_PWMPC_ICR1()
// PWM phase correct, ICR1 to indicate top
#define T1_WGM_PWMPC_OCR1A()

// CTC IRC1 to indicate top
#define T1_WGM_CTC_ICR1()

// Fast PWM, ICR1 to indicat top
#define T1_WGM_PWMF_ICR1()
// Fast PWM, OCR1A to indicat top
#define T1_WGM_PWMF_OCR1A()


// Waveform generation settings
// XXX this is defined above???!!!
//#define T1_WGM_NORMAL() t1_ctl.tccr2 &=~ (_BV(WGM21) | _BV(WGM20))
// Clear timer on compare match.
#define T1_WGM_CTC()    t1_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(WGM20)) | _BV(WGM21)
// Phase correct PWM.
#define T1_WGM_PWMPC() t1_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(WGM21)) | _BV(WGM20)
// Fast PWM.
#define T1_WGM_PWMF()  t1_ctl.tccr2 |=  (_BV(WGM20) | _BV(WGM21))


// control structure definition
typedef struct {
    // value that TCCR2 reg will be set to when timer is started.
    uint8_t tccr1a;
    uint8_t tccr1b;
}t1_ctl_t;

// global control structure - ensure that this is defined when the timer is used
extern  t1_ctl_t t1_ctl;


#endif
