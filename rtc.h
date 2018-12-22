#ifndef RTC_H
#define RTC_H
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
// -----------------------------------------------------------------------------
// Real time clock interface
// XXX Note: the structure of the rtc_XXX_t types is relied upon by ds1302 rtc
// XXX Do not change, without also changing ds1302.c
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "config.h"

// define according to device being used
#ifndef RTC_DEFS
#define RTC_DEFS
#warning Using default definitions: (ds1302 device)
#define RTC_USE_DS1302
#endif

//! represents times of day
typedef struct
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} rtc_time_t;

//! represents dates - note year is alway two digits, leading 20 is assumed
typedef struct
{
    uint8_t dom;
    uint8_t month;
    uint8_t dow;
    uint8_t year;
} rtc_date_t;

//! structure for holding both a data and a time
typedef struct
{
    rtc_time_t time;
    rtc_date_t date;
} rtc_datetime_t;


/** 
 * Convert a datetime to a string of the format:
 * YYYYMMDDHHmmSS
 * @param dtp Pointer to the datetime structure
 * @param s Pointer to the string that will be written,
 * must be at least 15 bytes long
 * @return Pointer to the string
 */
char* rtc_datetime_to_string(rtc_datetime_t* dtp, char *s);
char* rtc_date_to_string(rtc_date_t* dp, char *s);
char* rtc_time_to_string(rtc_time_t* tp, char *s);

/** 
 * Return -1 if d1 is before d2, 0 if same, 1 if after
 * @param d1 First date
 * @param d2 Second date
 * @return As above
 */
signed char rtc_datecmp(rtc_date_t *d1, rtc_date_t *d2);

/** 
 * Return -1 if d1 is before d2, 0 if same, 1 if after
 * @param d1 First time
 * @param d2 Second time
 * @return As above
 */
signed char rtc_timecmp(rtc_time_t *d1, rtc_time_t *d2);

/** 
 * Return -1 if d1 is before d2, 0 if same, 1 if after
 * @param d1 First datetime
 * @param d2 Second datetime
 * @return As above
 */
signed char rtc_datetimecmp(rtc_datetime_t *d1, rtc_datetime_t *d2);


// -----------------------------------------------------------------------------
// For ds1302 chip
#ifdef RTC_USE_DS1302
#include "rtc/ds1302.h"

#define RTC_READ(dtp)  ds1302_get_datetime(dtp); 
#define RTC_WRITE(dtp) ds1302_set_datetime(dtp)  
#define RTC_INIT()     ds1302_init()
#endif
// -----------------------------------------------------------------------------

#endif
