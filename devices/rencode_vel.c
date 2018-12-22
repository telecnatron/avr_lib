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
#include <avr/interrupt.h>
#include <avr/io.h>

// debug
#include <stdio.h>

#include "rencode_vel.h"
#include "rencode_vel_cpu.h"
#include "rencode.h"
#include "../util/io.h"


// Globals
static void (* rev_handler_user)(uint8_t dir, uint8_t vel);

//! as incremented by the timer OC ISR
static uint16_t rev_ticks;

//! This is called when a re pulse has been received
//! dir is 0 or 1
static void rev_handler(uint8_t dir)
{

    // tick interval between this pulse and previous
    uint16_t ticks;

    // check whether timer was running
    if(rev_ticks){
	// timer was running
	REV_TIMER_RESET();
	ticks = rev_ticks;
    }else{
	// timer was not running.
	REV_TIMER_START();
	ticks = 0;
    }
    rev_ticks=0;

    if(rev_handler_user)
	rev_handler_user(dir, ticks);
}


//! 
void rev_init(void (* rev_handler_fn)(uint8_t dir, uint8_t vel))
{

    re_init(rev_handler);
    rev_handler_user=rev_handler_fn;

    // init timer
    rev_init_timer();
}


// Tick hander ISR
// XXX dependent on 62.5Hz tick
ISR(REV_CM_ISR){
    rev_ticks++;
    if(rev_ticks>=1000){
	// 1000 ticks -> 1 second
	// time out: no re pulse was recieved
	// reset timer and rev_ticks
	REV_TIMER_STOP();
	REV_TIMER_RESET();
	rev_ticks=0;
	printf("rst-");
    }
}


