#ifndef _TASK_H
#define _TASK_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
/**
 * @file   task.h
 * @author steves
 * @date   2017/02/25 01:10:34
 * 
 * @brief  Tasking (co-routine) system for AVR.
 * 
 * 
 */

#include "config.h"
#include <stdint.h>

// Configuration defines:
// XXX How to get doxygen to document this? this just don't seem to work.
//! \def TASK_NUM_TASKS 
//! \brief The number of tasks in the task table. This must be defined prior to including this file.
#ifndef TASK_NUM_TASKS 
#error "TASK_NUM_TASKS is not defined."
#endif

/** 
 * Initialise a task.
 * 
 * @param task_num The number of the task. Must be unique amongst all tasks and within range: 0 <= task_num < TASK_NUM_TASKS
 * @param task Pointer to the task's callback function that will be called whenever the task is run
 * @param data Pointer that will be passed as parameter to task's calllback function when it is called. May be NULL.
 * @param run Flag to indicate whether the task should be made ready (ie runnable). If non-zero, task is made ready, or unready otherwise
 */
void task_init(uint8_t task_num, void(*task_callback)(void *data), void *data, uint8_t run);

/** 
 * Make task number task_num unready until the passed number of ticks has occured, ie put task to sleep
 * for this many ticks.
 * 
 * @param task_num The number of task for which the tick timer is to be set.
 * @param ticks The number of ticks that the task will sleep for.
 */
void task_num_set_tick_timer(uint8_t task_num, uint16_t ticks);

/** 
 * Put current task to sleep (ie make unrunnable) until the passed number of ticks has occured.
 * This function would normally only be called from within a task's callback function.
 * @see task_num_set_tick_timer
 * @param ticks The numer of ticks that the task will sleep for.
 */
void task_set_tick_timer(uint16_t ticks);

/** 
 * Put task numbered task_num to sleep for passed number of seconds, ie task will be made unrunnable,
 * and then runnable again after this number of seconds.
 * @param task_num The number of the task to be put to sleep.
 * @param seconds The number of the task for which the second timer is to be set
 */
void task_num_set_seconds_timer(uint8_t task_num, uint16_t seconds);

/** 
 * Put current task to sleep for passed number of seconds
 * @see task_num_set_seconds_timer
 * @param seconds The number of the task for which the second timer is to be set
 */
void task_set_seconds_timer(uint16_t seconds);

/** 
 * Function that should be called periodically to make the tasks run. This function, in turn, calls the callback function of all
 * tasks that are in a ready (runnable) state.
 */
void task_run();

/** 
 * Function that should be called periodically to indicate that a tick period has expired. 
 * Checks tick timers of all sleeping tasks and make task runnable if timer has expired.
 */
void task_tick();

/** 
 * Function that should be called every 'second'. Makes tasks whose seconds-timer have expired runnable.
 * Note that 'second' is defined as a time period that is close to one second.
 */
void task_seconds_tick();

/** 
 * Set the callback function that is to be called for task number task_num
 * 
 * @param task_num The number of the task for which callback function is to be set
 * @param callback Pointer to the callback function.
 */
void task_num_set_callback(uint8_t task_num, void (* callback)(void *data));

/** 
 * Set the callback function that is to be called for the current task.
 * 
 * @param callback Pointer to the callback function.
 */
void task_set_callback(void (* callback)(void *data));

/** 
 * Sets the pointer that is passed to a task's callback function when it is called.
 * 
 * @param task_num The number of the task.
 * @param data Pointer to the task's user data.
 */
void task_num_set_user_data(uint8_t task_num, void *data);

/** 
 * Sets the pointer to the task's user data for the current task.
 * @see task_num_set_user_data
 * @param data Pointer to the user data
 */
void task_set_user_data(void *data);


/** 
 * Make task ready to run, or unready to run.
 * 
 * @param task_num The number of the task to be made ready (or unready)
 * @param ready if zero then task is made unready, or ready otherwise
 */
void task_num_ready(uint8_t task_num, uint8_t ready);

/** 
 * Make current task ready to run, or unready to run
 * 
 * @param ready if zero then task is made unready, or ready otherwise
 */
void task_ready(uint8_t ready);

#endif /* _TASK_H */

