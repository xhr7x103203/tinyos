#include "tinyos.h"
#include "ARMCM3.H"

#define NVIC_INT_CTRL 0xe000ed04
#define NVIC_PENDSVSET 0x10000000
#define NVIC_SYSPRI2    0xe000ed22
#define NVIC_PENDSV_PRI 0x000000ff

#define MEM32(addr) *(volatile unsigned long *)(addr)
#define MEM8(addr) *(volatile unsigned char *)(addr)

__asm void PendSV_Handler(void)
{
    IMPORT cur_task
    IMPORT next_task
    
    MRS R0, PSP
    CBZ R0, pendsv_handle_nosave
    STMDB R0!, {R4-R11}
    LDR R1, =cur_task
    LDR R1, [R1]
    STR R0, [R1]
pendsv_handle_nosave
    LDR R0, =cur_task
    LDR R1, =next_task
    LDR R2, [R1]
    STR R2, [R0]
    
    LDR R0, [R2]
    LDMIA R0!, {R4-R11}
    MSR PSP, R0
    ORR LR, LR, #0x04
    BX LR
}
void os_start(void)
{
    __set_PSP(0);
    MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void os_schedule()
{
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
