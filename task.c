// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: $
// -----------------------------------------------------------------------------
#include <string.h>

#include "task.h"
#include "util/io.h"

// structure defining a task
typedef struct {    
    uint8_t flags;
    void (*task)();
    uint16_t tick_timer;
    uint16_t second_timer;
} task_t;

// bits comprising task_t.flags
#define TASK_FLAG_RUNNABLE 1

// task table
static task_t TaskTable[TASK_NUM_TASKS];

// id (index into TaskTable) of the task that is currently running, or that was last run
uint8_t task_running_id;

// helpers
#define TASK_MKP_READY(tp)   BIT_HI(tp->flags, TASK_FLAG_RUNNABLE);
#define TASK_MKP_UNREADY(tp) BIT_LO(tp->flags, TASK_FLAG_RUNNABLE);
#define TASK_IS_RUNNABLE_P(tp) BIT_IS_SET(tp->flags, TASK_FLAG_RUNNABLE )

void task_run()
{
    for(task_running_id=0; task_running_id!=TASK_NUM_TASKS; ++task_running_id){
	task_t *tp=&(TaskTable[task_running_id]);
	if(TASK_IS_RUNNABLE_P(tp)){
	    // call the task's function
	    tp->task();
	}
    }
    task_running_id--;
}

inline uint8_t task_is_ready(uint8_t tid)
{
    return TaskTable[tid].flags & _BV(TASK_FLAG_RUNNABLE);
}

inline uint8_t task_is_sleeping(uint8_t tid)
{
    return TaskTable[tid].tick_timer || TaskTable[tid].second_timer;
}

inline void task_ready(uint8_t tid)
{
    BIT_HI(TaskTable[tid].flags, TASK_FLAG_RUNNABLE);
}

inline void task_unready(uint8_t tid)
{
    BIT_LO(TaskTable[tid].flags, TASK_FLAG_RUNNABLE);
}

void task_tick_alarm(uint8_t tid, uint16_t ticks)
{
    task_t *tp=&(TaskTable[tid]);    
    TASK_MKP_UNREADY(tp);
    tp->tick_timer=ticks;
}

void task_seconds_alarm(uint8_t tid, uint16_t seconds)
{
    task_t *tp=&(TaskTable[tid]);    
    TASK_MKP_UNREADY(tp);
    tp->second_timer=seconds;
}

void task_init_task(uint8_t task_num, void (* task)())
{
    // zero table entry
    memset((void *)&(TaskTable[task_num]), 0, sizeof(task_t));
    TaskTable[task_num].task = task;
}

void task_tick()
{
    // loop thru task table decrementing tick counters 
    for(uint8_t i=0; i!=TASK_NUM_TASKS; ++i){
	task_t *tp=&(TaskTable[i]);
	if(tp->tick_timer){
	    if(!--tp->tick_timer){
		// timer has reached zero, make runnable
		TASK_MKP_READY(tp);
	    }
	}
    }
}

void task_seconds_tick()
{
    // loop thru task table decrementing tick counters 
    for(uint8_t i=0; i!=TASK_NUM_TASKS; ++i){
	task_t *tp=&(TaskTable[i]);
	if(tp->second_timer){
	    if(!--tp->second_timer){
		// timer has reached zero, make runnable
		TASK_MKP_READY(tp);
	    }
	}
    }
}
    

