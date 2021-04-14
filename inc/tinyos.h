#ifndef __TINYOS_H__
#define __TINYOS_H__

#include <stdint.h>

typedef uint32_t u32;

typedef u32 task_stack_t;

typedef void (*task_handle_t)(void *param);

typedef struct task_s {
    task_stack_t *stack;
    u32 stack_size;
    task_handle_t func;
    void *param;
} task_t;

#endif