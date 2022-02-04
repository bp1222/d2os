.global arch_mutex_acquire
arch_mutex_acquire:
    mov     r1, #0x1
1:
    ldrex   r2, [r0]
    cmp     r2, r1
    beq     2f
    strexne r2, r1, [r0]
    cmpne   r2, #0x1
    beq     1b
    dmb
    bx      lr
2:
    wfe
    b 1b

.global arch_mutex_release
arch_mutex_release: 
    mov     r1, #0x0
    dmb
    str     r1, [r0]
    dmb
    sev
    bx      lr
