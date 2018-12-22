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
#include "task.h"
#include "lib/util/io.h"


#ifdef TASK_LOGGING
#include "lib/log.h"
#define TASK_LOG_DEBUG(fmt, msg...) LOG_DEBUG_FP(fmt, msg)
#else
#define TASK_LOG_DEBUG(fmt, msg...)
#endif

// structure defining a task
typedef struct {    
    uint8_t flags;
    void (*task)(void *data);
    // tick number at which task will be made runnable
    uint16_t tick_alarm;
    // second number at which task will be made runnable
    uint32_t seconds_alarm;
    // user data gets passed to task function when it is called.
    void *user_data;
} task_t;


typedef struct {
    // task table
    task_t task_tab[TASK_NUM_TASKS];
    // task num (index into task_tab) of currently running task
    uint8_t task_num;
    // current tick
    uint16_t tick_count;
    // current second
    uint32_t seconds_count;
    // tick at which next task alarm will expire
    uint16_t tick_wake;
    // number of tasks that are waiting on a tick alarm
    uint8_t task_alarm_count;
} task_ctrl_t;


// defines for task_t.flags bits:
// if this bit is set then task is ready to be run
#define TASK_FLAGS_READY      0x1
// if this bit is set then task is waiting for a tick alarm
#define TASK_FLAGS_TICK_ALARM 0x2
// if this bit is set then task is waiting for seconds alarm
#define TASK_FLAGS_SECONDS_ALARM 0x4

// global control structure
static task_ctrl_t task_ctrl;

// convenience macros
#define TASK_READY(task_p)    BIT_HI(task_p->flags, TASK_FLAGS_READY)
#define TASK_UNREADY(task_p)  BIT_LO(task_p->flags, TASK_FLAGS_READY)
#define TASK_IS_READY(task_p) BIT_IS_SET(task_p->flags, TASK_FLAGS_READY )

#define TASK_IS_ALARM_TICK(task_p)     BIT_IS_SET(task_p->flags, TASK_FLAGS_TICK_ALARM )
#define TASK_SET_TICK_ALARM(task_p)    BIT_HI(task_p->flags, TASK_FLAGS_TICK_ALARM)
#define TASK_UNSET_TICK_ALARM(task_p)  BIT_LO(task_p->flags, TASK_FLAGS_TICK_ALARM)

#define TASK_IS_ALARM_SECONDS(task_p)  BIT_IS_SET(task_p->flags, TASK_FLAGS_SECONDS_ALARM )
#define TASK_SET_SECONDS_ALARM(task_p)    BIT_HI(task_p->flags, TASK_FLAGS_SECONDS_ALARM)
#define TASK_UNSET_SECONDS_ALARM(task_p)  BIT_LO(task_p->flags, TASK_FLAGS_SECONDS_ALARM)

// calculate tick of alarm to expire soonest, set task_ctrl.tick_wake to that value.
void task_set_tick_wake()
{
    if(task_ctrl.task_alarm_count){
	// calculate when the next task alarm will expire
	task_t *task = task_ctrl.task_tab;
	
	uint16_t ticks_away = UINT16_MAX;
	for (uint8_t i=0; i < TASK_NUM_TASKS;  i++, task++){
	    if (TASK_IS_ALARM_TICK(task) ){
		uint16_t task_ticks_away = task->tick_alarm - task_ctrl.tick_count;
		if (task_ticks_away < ticks_away){
		    ticks_away = task_ticks_away;
		}
	    }
	}
	task_ctrl.tick_wake = task_ctrl.tick_count + ticks_away;
    }
}



void task_num_set_callback(uint8_t task_num, void (* callback)(void *data))
{
    task_ctrl.task_tab[task_num].task = callback;
}

void task_set_callback(void (* callback)(void *data))
{
    task_ctrl.task_tab[task_ctrl.task_num].task = callback;
}

void task_num_set_user_data(uint8_t task_num, void *data)
{
    task_ctrl.task_tab[task_num].user_data = data;
}

void task_set_user_data(void *data)
{
    task_num_set_user_data(task_ctrl.task_num, data);
}


void task_init(uint8_t task_num, void(*task_callback)(void *data), void *data, uint8_t run)
{
    task_t *task = &(task_ctrl.task_tab[task_num]);
    task->flags = 0;
    task->task= task_callback;
    task->user_data = data;
    //  make task ready (runnable) according to 'run' parameter
    if(run){
	TASK_READY(task);
    }
}


void task_num_ready(uint8_t task_num, uint8_t ready)
{
    task_t *task = &(task_ctrl.task_tab[task_num]);
    TASK_UNSET_SECONDS_ALARM(task);
    task_num_cancel_tick_timer(task_num);
    if (ready){
	TASK_LOG_DEBUG("%s:%u: ready %u",__FILE__,__LINE__,task_ctrl.tick_count, task_num);
	TASK_READY(task);
    }else{
	TASK_UNREADY(task);
	TASK_LOG_DEBUG("%s:%u: unready %u",__FILE__,__LINE__,task_ctrl.tick_count, task_num);
    }
    // figure out next alarm to expire
    // XXX this is done in task_num_cancel_tick_timer();
//  task_set_tick_wake();
}

void task_ready(uint8_t ready)
{
    task_num_ready(task_ctrl.task_num, ready);
}

void task_num_cancel_tick_timer(uint8_t task_num)
{
    task_t *task = &(task_ctrl.task_tab[task_num]);
    if( TASK_IS_ALARM_TICK(task)){
	// yup, alarm was set
	TASK_UNSET_TICK_ALARM(task);
	task_ctrl.task_alarm_count--;
	TASK_LOG_DEBUG("%s:%u: cancelled tick timer: %u",__FILE__,__LINE__, task_num);
	// figure out next alarm to expire
	task_set_tick_wake();

    }
}

void task_num_set_tick_timer(uint8_t task_num, uint16_t ticks)
{
    task_t *task = &(task_ctrl.task_tab[task_num]);
    TASK_UNREADY(task);
    // set flag to indicate task is waiting on timer
    TASK_SET_TICK_ALARM(task);
    // set the tick_count at which timer expires
    task->tick_alarm = task_ctrl.tick_count + ticks;
    // increment count of task that are waiting on an alarm
    task_ctrl.task_alarm_count++;

    // figure out next alarm to expire
    task_set_tick_wake();
    TASK_LOG_DEBUG("%s:%u:%u %u wake at %u ticks, next wake: %u ticks",__FILE__,__LINE__,task_ctrl.tick_count, task_num, task->tick_alarm, task_ctrl.tick_wake);

}

inline void task_set_tick_timer(uint16_t ticks)
{
    task_num_set_tick_timer(task_ctrl.task_num, ticks);
}

void task_num_set_seconds_timer(uint8_t task_num, uint16_t seconds)
{
    task_t *task = &(task_ctrl.task_tab[task_num]);
    TASK_SET_SECONDS_ALARM(task);
    TASK_UNREADY(task);
    task->seconds_alarm  = task_ctrl.seconds_count + seconds;
}

void task_set_seconds_timer(uint16_t seconds)
{
    task_num_set_seconds_timer(task_ctrl.task_num, seconds);
}

void task_run()
{
    // loop thru all tasks,
    task_t *task = task_ctrl.task_tab;
    for (uint8_t i=0; i< TASK_NUM_TASKS; i++, task++) {
	// check if task is ready to be run
	if (TASK_IS_READY(task)){
	    // yup it's ready, call it
	    task_ctrl.task_num = i;
	    TASK_LOG_DEBUG("%s:%u:%u running %u",__FILE__,__LINE__,task_ctrl.tick_count, i );
	    task->task(task->user_data);
	}
    }
}

void task_tick()
{
    task_ctrl.tick_count ++;
    if(task_ctrl.task_alarm_count && task_ctrl.tick_count == task_ctrl.tick_wake){
	// a task alarm has expired.
	task_t *task = task_ctrl.task_tab;
	// loop thru tasks.
	for (uint8_t i=0; i < TASK_NUM_TASKS;  i++, task++){
	    if( TASK_IS_ALARM_TICK(task) ){
		// only interested in tasks with tick alarm set
		if ( task->tick_alarm == task_ctrl.tick_count){
		    // alarm has expired, make task ready
		    TASK_READY(task);
		    TASK_UNSET_TICK_ALARM(task);
		    task_ctrl.task_alarm_count--;
	    	    TASK_LOG_DEBUG("%s:%u:%u ready on tick alarm: %u",__FILE__,__LINE__,task_ctrl.tick_count, i );
		}
	    }
	}
	task_set_tick_wake();
    }
}

void task_seconds_tick()
{
    task_ctrl.seconds_count++;
    task_t *task = task_ctrl.task_tab;
    // loop thru tasks
    for (uint8_t i=0; i < TASK_NUM_TASKS; i++, task++){
	if(TASK_IS_ALARM_SECONDS(task) && task->seconds_alarm == task_ctrl.seconds_count){
	    // make task ready to run
	    TASK_LOG_DEBUG("%s:%u:%u ready on seconds alarm: %u",__FILE__,__LINE__,task_ctrl.tick_count, i );
	    TASK_READY(task);
	    TASK_UNSET_SECONDS_ALARM(task);
	}
    }
}
