#include <stdio.h>
#include "ARMCM3.h"
#include "tinyos.h"


task_t *cur_task;
task_t *next_task;
task_t task1;
task_t task2;
task_stack_t task1_stack[1024];
task_stack_t task2_stack[1024];
int task1_flag;
int task2_flag;
task_t *task_table[2];

task_t task_idle;
task_stack_t idle_stack[1024];


void task_stack_init(task_t *task)
{
    task_stack_t *stack;
    
    stack = task->stack;
    *(--stack) = (unsigned long)(1 << 24);
    *(--stack) = (unsigned long)task->func;
    *(--stack) = (unsigned long)0x14;
    *(--stack) = (unsigned long)0x12;
    *(--stack) = (unsigned long)0x3;
    *(--stack) = (unsigned long)0x2;
    *(--stack) = (unsigned long)0x1;
    *(--stack) = (unsigned long)task->param;
    
    *(--stack) = (unsigned long)0x11;
    *(--stack) = (unsigned long)0x10;
    *(--stack) = (unsigned long)0x9;
    *(--stack) = (unsigned long)0x8;
    *(--stack) = (unsigned long)0x7;
    *(--stack) = (unsigned long)0x6;
    *(--stack) = (unsigned long)0x5;
    *(--stack) = (unsigned long)0x4;
    task->stack = stack;
}


void task_init(task_t *task, task_handle_t handle, void *param, 
    task_stack_t *stack, u32 len)
{
    task->stack = stack;
    task->stack_size = len;
    task->func = handle;
    task->param = param;
    task->delay_tick = 0;
    task_stack_init(task);
}

void task_schedule(void)
{
    u32 need_schedule;
    
    need_schedule = 1;
    if (cur_task == &task_idle) {
        if (task_table[0]->delay_tick == 0) {
            next_task = task_table[0];
        } else if (task_table[1]->delay_tick == 0) {
            next_task = task_table[1];
        } else {
            need_schedule = 0;
        }
    } else if (cur_task == task_table[0]){
        if (task_table[1]->delay_tick == 0) {
            next_task = task_table[1];
        } else if (cur_task->delay_tick != 0) {
            next_task = &task_idle;
        } else {
            need_schedule = 0;
        }
    } else if (cur_task == task_table[1]) {
        if (task_table[1]->delay_tick == 0) {
            next_task = task_table[1];
        } else if (cur_task->delay_tick != 0) {
            next_task = &task_idle;
        } else {
            need_schedule = 0;
        }
    }
    if (need_schedule) {
        os_schedule();
    }
}

void task_delay(u32 delay)
{
    cur_task->delay_tick = delay;
    os_schedule();
}

void delay(int count)
{
    while (--count > 0);
}

static void system_tick_init(u32 ms)
{
    SysTick->LOAD = ms * SystemCoreClock / 1000;
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk 
        | SysTick_CTRL_ENABLE_Msk;
}

void task_systick_handle(void)
{
    int i;
    
    for (i = 0; i < ARRAY_SIZE(task_table); i++) {
        if (task_table[i]->delay_tick > 0) {
            task_table[i]->delay_tick--;
        }
    }
    task_schedule();
}

void SysTick_Handler(void)
{
    task_systick_handle();
}

void idle_handle(void *param)
{
    int idle;
    
    idle = 0;
    for (;;) {
        idle++;
    }
}

void task1_handle(void *param)
{
    system_tick_init(10);
    for (;;) {
        task1_flag = 0;
        task_delay(10);
        task1_flag = 1;
        task_delay(10);
    }
}

void task2_handle(void *param)
{
    for (;;) {
        task2_flag = 0;
        task_delay(10);
        task2_flag = 1;
        task_delay(10);    
    }
}

int main()
{    
    task_init(&task1, task1_handle,(void *)0x1111, &task1_stack[1024], 
        ARRAY_SIZE(task1_stack));
    task_init(&task2, task2_handle,(void *)0x1111, &task2_stack[1024], 
        ARRAY_SIZE(task2_stack));
    task_init(&task_idle, idle_handle,(void *)0x1111, &idle_stack[1024], 
        ARRAY_SIZE(idle_stack));
    task_table[0] = &task1;
    task_table[1] = &task2;
    cur_task = NULL;
    next_task = task_table[0];
    os_start();
    return 0;
}
