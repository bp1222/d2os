.global mutex_acquire
mutex_acquire:
    mov     r1, #0x1
_mutex_acquire_attempt:
    ldrex   r2, [r0]
    cmp     r2, r1
    beq     _mutex_acquire_wait
    strexne r2, r1, [r0]
    cmpne   r2, #0x1
    beq     _mutex_acquire_attempt
    dmb
    bx      lr

_mutex_acquire_wait:
    wfe
    b _mutex_acquire_attempt


.global mutex_release
mutex_release: 
    mov     r1, #0x0
    dmb
    str     r1, [r0]
    sev
    bx      lr
