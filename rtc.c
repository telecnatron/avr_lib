// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: rtc.h 114 2014-09-01 10:04:55Z steves $
// Real time clock interface
// -----------------------------------------------------------------------------
#include "rtc.h"
#include <stdio.h>
#include <avr/pgmspace.h>

char* rtc_datetime_to_string(rtc_datetime_t* dtp,char *s)
{
    sprintf_P(s,PSTR("20%02u%02u%02u%02u%02u%02u"),
	      dtp->date.year,
	      dtp->date.month,
	      dtp->date.dom,
	      dtp->time.hour,
	      dtp->time.minute,
	      dtp->time.second
	);
    return s;
}

char* rtc_date_to_string(rtc_date_t* dp, char *s)
{
    sprintf_P(s,PSTR("20%02u%02u%02u"),
	      dp->year,
	      dp->month,
	      dp->dom
	);
    return s;
}

char* rtc_time_to_string(rtc_time_t* tp, char *s)
{
    sprintf_P(s,PSTR("%02u%02u%02u"),
	      tp->hour,
	      tp->minute,
	      tp->second
	);
    return s;
}

signed char rtc_datecmp(rtc_date_t *d1, rtc_date_t *d2)
{
    if(d1->year == d2->year){
	if(d1->month==d2->month){
	    if(d1->dom==d2->dom){
		return 0;
	    }
	    if(d1->dom < d2->dom){
		return -1;
	    }
	    return 1;
	}else if (d1->month < d2->month){
	    return -1;
	}
	return 1;
    }else if (d1->year < d2->year){
	return -1;
    }else{
	return 1;
    }
    return 0;

}

signed char rtc_timecmp(rtc_time_t *d1, rtc_time_t *d2)
{
    if(d1->hour == d2->hour){
	if(d1->minute==d2->minute){
	    if(d1->second==d2->second){
		return 0;
	    }
	    if(d1->second < d2->second){
		return -1;
	    }
	    return 1;
	}else if (d1->minute < d2->minute){
	    return -1;
	}
	return 1;
    }else if (d1->hour < d2->hour){
	return -1;
    }else{
	return 1;
    }
    return 0;
}

signed char rtc_datetimecmp(rtc_datetime_t *d1, rtc_datetime_t *d2)
{
    char t=rtc_datecmp(&d1->date,&d2->date);
    if(t!=0)
	return t;
    return rtc_timecmp(&d1->time,&d2->time);
}
