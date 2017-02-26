#ifndef _TASK_H
#define _TASK_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   task.h
 * @author steves
 * @date   2017/02/25 01:10:34
 * 
 * @brief  Tasking (co-routine) system for AVR.
 * 
 */

#include "config.h"
#include <stdint.h>

// Configuration defines
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
 * 
 * 
 * @param task_num 
 * @param ticks 
 */
void task_num_set_tick_timer(uint8_t task_num, uint16_t ticks);

/** 
 * 
 * 
 * @param ticks 
 */
void task_set_tick_timer(uint16_t ticks);

/** 
 * 
 * 
 * @param task_num 
 * @param ticks 
 */
void task_num_set_seconds_timer(uint8_t task_num, uint16_t ticks);

/** 
 * 
 * 
 * @param ticks 
 */
void task_set_seconds_timer(uint16_t ticks);

/** 
 * 
 * 
 */
void task_run();

/** 
 * 
 * 
 */
void task_tick();

/** 
 * 
 * 
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
 * 
 * 
 * @param task_num 
 * @param data 
 */
void task_num_set_user_data(uint8_t task_num, void *data);

/** 
 * 
 * 
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

