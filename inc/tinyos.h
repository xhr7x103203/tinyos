#ifndef __TINYOS_H__
#define __TINYOS_H__

#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

typedef uint32_t u32;

typedef u32 task_stack_t;

typedef void (*task_handle_t)(void *param);

typedef struct task_s {
    task_stack_t *stack;
    u32 stack_size;
    task_handle_t func;
    void *param;
} task_t;

extern task_t *cur_task;
extern task_t *next_task;

void os_start(void);
void os_schedule(void);
#endif
