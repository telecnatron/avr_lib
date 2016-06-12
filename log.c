// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: log.c 405 2015-12-27 04:09:25Z steves $
// -----------------------------------------------------------------------------
#include "log.h"
#include "util.h"

#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// log level strings
const char log_level_debug[]  PROGMEM = "DEBUG";
const char log_level_info[]   PROGMEM = "INFO";
const char log_level_warn[]   PROGMEM = "WARN";
const char log_level_error[]  PROGMEM = "ERROR";

// log level
uint8_t log_level=LOG_LEVEL_INFO;

// (default callback) sets dts[LOG_DATE_STR_MAX] to string representaion of the (current) date to be logged
void log_get_date(char *dts);
//!pointer to the callback function used for formatting the log-message date string
void (*log_get_date_cb)(char *dts)  = log_get_date;

void log_set_level(uint8_t level)
{
    log_level=level;
}

// Default function to write the date that will be displayed for the log message into the passed buffer
// Note: the function pointed to by log_get_date_cb is the one that is called - use this to override
void log_get_date(char *dts)
{
    const char ds[]="NOW";
    strcpy(dts, ds);
}

// write the first bit of the log message (ie stuff preceedign the message)
static void log_start(uint8_t level)
{
    char dts[LOG_DATE_STR_MAX];
    log_get_date_cb(dts);
    printf_P(PSTR("|LOG: %s-"),dts );
    switch(level){
	case 0: putss_P((char *)log_level_debug);break;
	case 1: putss_P((char *)log_level_info);break;
	case 2: putss_P((char *)log_level_warn);break;
	case 3: putss_P((char *)log_level_error);break;
    }
    putss_P((char *)PSTR(": "));
    
}

void log_log(uint8_t level, char *msg){
    if(level >= log_level){
	log_start(level);
	printf_P(PSTR("%s\n"),msg);
    }
}


void log_log_P(uint8_t level, const char *pmsg)
{
    if(level >= log_level){
	log_start(level);
	puts_P(pmsg);
    }
}

void log_log_fmt_P(uint8_t level, const char* format_p, ...)
{
    if(level >= log_level){
	va_list args;
	
	log_start(level);
	va_start(args, format_p);
	vfprintf_P(stdout, format_p, args);
	va_end(args);
	printf("\n");
    }
}
