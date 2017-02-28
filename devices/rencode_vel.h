#ifndef RENCODE_VEL_H
#define RENCODE_VEL_H
// ------------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://www.telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// ------------------------------------------------------------------------------
#include "../timer.h"

//! 
void rev_init(void (* rev_handler_fn)(uint8_t dir, uint8_t vel));



#endif
