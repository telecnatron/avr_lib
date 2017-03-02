// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
#include "wdt.h"


void wdt_init()
{
    MCUSR=0;
    wdt_disable();
}

void wdt_reset_mcu()
{
    // signal wdt to reset on timeout
    WDT_RESET();
}
