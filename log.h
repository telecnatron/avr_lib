#ifndef LOG_H
#define LOG_H
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
 * @file   log.c
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Sun Dec 27 14:19:30 2015
 * 
 * @brief  Formatted log output using the printf()-type functions as provided
 * in <stdio.h>
 * 
 */
#include <stdint.h>
#include <avr/pgmspace.h>

// log levels
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

//! Maximum number of characters allowed in the date string, including terminating 0
//! If providing log_get_date_cb(), ensure that it does not write more than this number
//! of characters
#define LOG_DATE_STR_MAX 15

//! Pointer to the callback function used for formatting the log-message date string.
//! Points to a default, override to use your own function.
extern void (*log_get_date_cb)(char *dts, uint8_t dts_max_len);
//! Macro to config custome date callback function.
#define LOG_INIT_DATE_CB(cbf) log_get_date_cb=cbf

/** 
 * Log the passed message
 * @param level The log level being one of the LOG_LEVEL_xxx defines
 * @param msg The message to be logged
 */
void log_log(uint8_t level, char *msg);
// Convienience macros:
#define LOG_DEBUG(msg) log_log(LOG_LEVEL_DEBUG, msg)
#define LOG_INFO(msg)  log_log(LOG_LEVEL_INFO,  msg)
#define LOG_WARN(msg)  log_log(LOG_LEVEL_WARN,  msg)
#define LOG_ERROR(msg) log_log(LOG_LEVEL_ERROR, msg)
#define LOG(msg)       LOG_INFO(msg)

//! Set the log level, level should be one of the LOG_LEVEL_XXX defines
void log_set_level(uint8_t level);


/** 
 * Log the passed message arguments printf style, fmt being put into PSTR(fmt)
 * @param level The log level being one of the LOG_LEVEL_xxx defines
 * @param format_p  The format string which is put into PROGMEM
 */
void log_log_fmt_P(uint8_t level, const char* format_p, ...);
#define LOG_DEBUG_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_DEBUG, PSTR(fmt),msg)
#define LOG_INFO_FP(fmt,msg...)  log_log_fmt_P(LOG_LEVEL_INFO,  PSTR(fmt),msg)
#define LOG_WARN_FP(fmt,msg...)  log_log_fmt_P(LOG_LEVEL_WARN,  PSTR(fmt),msg)
#define LOG_ERROR_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_ERROR, PSTR(fmt),msg)
#define LOG_FP(fmt,msg...)       LOG_INFO_FP(PSTR(fmt),msg);


/** 
 * Log messages that is contained in PROGMEM
 * 
 * @param level The log level being one of the LOG_LEVEL_xxx defines
 * @param pmsg Pointer to location in PROGMEM of the message string
 */
void log_log_P(uint8_t level, const char *pmsg);
#define LOG_DEBUG_P(msg) log_log_P(LOG_DEBUG_DEBUG, PSTR(msg))
#define LOG_INFO_P(msg)  log_log_P(LOG_LEVEL_INFO,  PSTR(msg))
#define LOG_WARN_P(msg)  log_log_P(LOG_LEVEL_WARN,  PSTR(msg))
#define LOG_ERROR_P(msg) log_log_P(LOG_LEVEL_ERROR, PSTR(msg))
#define LOG_P(msg) LOG_INFO_P(msg)

// -----------------------------------------------------------------------------
#endif
