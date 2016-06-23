#ifndef TASK_H
#define TASK_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: task.h 148 2014-09-26 07:30:50Z steves $
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "config.h"

// maximum number of tasks - maybe overriden in  config.h etc
#ifndef TASK_NUM_TASKS
#define TASK_NUM_TASKS 0
#endif


//! id (index into TaskTable) of the task that is currently running,
//! only valid whilst in task_run(), otherwise is id of task that was last run
extern uint8_t task_running_id;

/** 
 * Make task unrunnable and schedule to be runnable again
 * once passed number of ticks have elapsed.
 * @param tid Task id of the task
 * @param ticks the number of ticks 
 */
void task_tick_alarm(uint8_t tid, uint16_t ticks);
//! set tick alarm for currently running task
#define TASK_TICK_ALARM(ticks) task_tick_alarm(task_running_id, ticks)

/** 
 * Make task unrunnable and schedule to be runnable again
 * once passed number of seconds have elapsed.
 * @param tid Task id of the task
 * @param ticks the number of seconds
 */
void task_seconds_alarm(uint8_t tid, uint16_t seconds);
//! set second alarm for currently running task
#define TASK_SECONDS_ALARM(seconds) task_seconds_alarm(task_running_id, seconds);

/** 
 * Make the task with passed tid (ie index into task table) ready to run
 * @param tid Task id number of the task to be made ready
 */
void task_ready(uint8_t tid);

/** 
 * Make the task with passed tid (ie index into task table) ready to run
 * @param tid Task id number of the task to be made ready
 */
void task_unready(uint8_t tid);
#define TASK_UNREADY() task_unready(task_running_id);

//! Return true if task is ready, false otherwise
uint8_t task_is_ready(uint8_t tid);

//! Return true if task is sleeping on either second timer or tick timer
uint8_t task_is_sleeping(uint8_t tid);

/** 
 * Loop thru the task table, calling the task function of any tasks that are flagged
 * as runnable. Starts with task 0
 */
void task_run();

/** 
 * Initialise a task by making entry in the task table, flags and timers are cleared
 * @param task_num The task id (tid). This is index into task_table
 * @param task Pointer to the function that gets called when the task is run.
 */
void task_init_task(uint8_t task_num, void (* task)());


/** 
 *  Initialise task and make it ready by means of calls to task_init_task()
 *  and task_ready()
 * @param task_num The task id (tid). This is index into task_table
 * @param task Pointer to the function that gets called when the task is run.
 */
void task_init_ready(uint8_t task_num, void (* task)());

/** 
 * Make task with passed id unrunnable for passed number of ticks,
 * task_tick() will then make it runnable when tick period has elapsed
 * @param tid The id of the task
 * @param ticks The number of ticks
 */
void task_wait_ticks(uint8_t tid, uint16_t ticks);

/** 
 * Make task with passed id unrunnable for passed number of seconds,
 * task_tick() will then make it runnable when second period has elapsed
 * @param tid The id of the task
 * @param seconds The number of seconds
 */
void task_wait_seconds(uint8_t tid, uint16_t seconds);


/** 
 * Should be called by the system each time a tick period has elapsed,
 * decrements the task's tick timers
 */
void task_tick();

/** 
 * Should be called by the system each time a second period has elapsed,
 * decrements the task's second timers
 */
void task_seconds_tick();

#endif
