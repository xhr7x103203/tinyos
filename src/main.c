#include "tinyos.h"

task_stack_t task1_stack[1024];
task_stack_t task2_stack[1024];

void task_stack_init(task_stack_t *stack)
{

}


void task_init(task_t *task, task_handle_t handle, void *param, 
    task_stack_t *stack, u32 len)
{
    task->stack = stack;
    task->stack_size = len;
    task->func = handle;
    task->param = param;
    task_stack_init(task->stack);
}

void task_schedule(void)
{

}

void delay(int count)
{
    while (--count > 0);
}

void task1_handle(void *param)
{
    for (;;) {
    
    }
}

void task2_handle(void *param)
{
    for (;;) {
    
    }
}


int main()
{    
    return 0;
}
