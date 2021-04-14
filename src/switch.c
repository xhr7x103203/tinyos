__asm void PendSV_Handler(void)
{
#if 0
    IMPORT stack_ptr
    LDR R0, =stack_ptr
    LDR R0, [R0]
    LDR R0, [R0]
    
    STMDB R0!, {R4-R11}
    LDR R1, =stack_ptr
    LDR R1, [R1]
    STR R0, [R1]
    
    ADD R4, R4, #1
    ADD R5, R5, #1
    LDMIA R0!, {R4-R11}
    STR R0, [R1]
    
    BX LR
#endif
}