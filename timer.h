#ifndef TIMER_H
#define TIMER_H
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
/**
 * @file   timer.h
 * @author Stephen Stebbing 
 * @date   Fri Jul  8 15:17:44 2016
 * 
 * @brief  Wrapper for timer header files for various processors. 
 * The processor used is determined by having one of the following defined:
 * ATTINY2313, ATMEGA8, ATMEGA88, ATMEGA168, ATMEGA328
 */

#include <stddef.h>

// -----------------------------------------------------------------------------
// ATTINY2313
// -----------------------------------------------------------------------------
#ifdef ATTINY2313
#include "timer/att2313_t0.h"
#endif

// -----------------------------------------------------------------------------
// ATMEGA8
// -----------------------------------------------------------------------------
#ifdef ATMEGA8
#include "timer/atm8_t0.h"
#include "timer/atm8_t1.h"
#include "timer/atm8_t2.h"
#endif

// -----------------------------------------------------------------------------
// ATMEG328
// -----------------------------------------------------------------------------
#if defined(ATMEGA328) || defined(ATMEGA168) || defined(ATMEGA88)
#include "timer/atm328_t0.h"
//#include "timer/atm328_t1.h"
#include "timer/atm328_t2.h"
#endif

#endif
