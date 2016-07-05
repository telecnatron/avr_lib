// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
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
