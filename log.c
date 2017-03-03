// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2013. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
#include "log.h"
#include "util.h"

#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//! log level string debug
const char log_level_debug[]  PROGMEM = "DEBUG";
//! log level string info
const char log_level_info[]   PROGMEM = "INFO";
//! log level string warn
const char log_level_warn[]   PROGMEM = "WARN";
//! log level string error
const char log_level_error[]  PROGMEM = "ERROR";

//! log level
uint8_t log_level=LOG_LEVEL_INFO;

//! (default callback) sets dts[LOG_DATE_STR_MAX] to string representaion of the (current) date to be logged
void log_get_date(char *dts, uint8_t dts_max_len);
//! pointer to the callback function used for formatting the log-message date string
void (*log_get_date_cb)(char *dts, uint8_t dts_max_len)  = log_get_date;

void log_set_level(uint8_t level)
{
    log_level=level;
}

/** 
 * Default function to write the date that will be displayed for the log message into the passed buffer
 * Note: the function pointed to by log_get_date_cb is the one that is called - use this to override
 * @param dts Character buffer into which data string is written.
 * @param dts_max_len Lenght of the buffer. No more than this many characters, including null terminator, may be copied to dts buffer.
 */
void log_get_date(char *dts, uint8_t dts_max_len)
{
    const char ds[]="NOW";
    strncpy(dts, ds, dts_max_len);
    // ensure null terminiated
    dts[dts_max_len-1]='\x0';
}

// write the first bit of the log message (ie stuff preceedign the message)
static void log_start(uint8_t level)
{
    char dts[LOG_DATE_STR_MAX];
    log_get_date_cb(dts, LOG_DATE_STR_MAX - 1);
    printf_P(PSTR("\tLOG:%s:"),dts );
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
