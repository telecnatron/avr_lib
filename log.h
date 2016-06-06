#ifndef LOG_H
#define LOG_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: log.h 405 2015-12-27 04:09:25Z steves $
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

//! Maximum number of characters allowed in the date string, including terminating \x0
//! If providing log_get_date_cb(), ensure that it does not write more than this number
//! of characters
#define LOG_DATE_STR_MAX 15

//! Pointer to the callback function used for formatting the log-message date string.
//! Points to a default, override to use your own function.
extern void (*log_get_date_cb)(char *dts);
//! Macro to config custome date callback function.
#define LOG_INIT_DATE_CB(cbf) log_get_date_cb=cbf

/** 
 * Log the passed message
 * @param level The log level being one of the CONTROL_LOG_LEVEL_xxx defines
 * @param msg The message to be logged
 */
void log_log(uint8_t level, char *msg);
// Convienience macros:
#define LOG_DEBUG(msg) log_log(LOG_LEVEL_DEBUG,msg)
#define LOG_INFO(msg) log_log(LOG_LEVEL_INFO,msg)
#define LOG_WARN(msg) log_log(LOG_LEVEL_WARN,msg)
#define LOG_ERROR(msg) log_log(LOG_LEVEL_ERROR,msg)
#define LOG(msg) LOG_INFO(msg)

//! Set the log level, level should be one of the LOG_LEVEL_XXX defines
void log_set_level(uint8_t level);


/** 
 * Log the passed message arguments printf style
 * @param level The log level being one of the CONTROL_LOG_LEVEL_xxx defines
 * @param format_p  The format string residing in PROGMEM
 */
void log_log_fmt_P(uint8_t level, const char* format_p, ...);
#define LOG_DEBUG_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_DEBUG,fmt,msg)
#define LOG_INFO_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_INFO,fmt,msg)
#define LOG_WARN_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_WARN,fmt,msg)
#define LOG_ERROR_FP(fmt,msg...) log_log_fmt_P(LOG_LEVEL_ERROR,fmt,msg)
#define LOG_FP(fmt,msg...) LOG_INFO_FP(fmt,msg);


/** 
 * Log messages that is contained in PROGMEM
 * 
 * @param level The log level being one of the CNOTROL_LOG_LEVEL_xxx defines
 * @param pmsg Pointer to location in PROGMEM of the message string
 */
void log_log_P(uint8_t level, const char *pmsg);
#define LOG_DEBUG_P(msg) log_log_P(LOG_DEBUG_INFO,msg)
#define LOG_INFO_P(msg) log_log_P(LOG_LEVEL_INFO,msg)
#define LOG_WARN_P(msg) log_log_P(LOG_LEVEL_WARN,msg)
#define LOG_ERROR_P(msg) log_log_P(LOG_LEVEL_ERROR,msg)
#define LOG_P(msg) LOG_INFO_P(msg)




#endif
