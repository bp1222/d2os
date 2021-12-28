.globl	invalidate_data_cache
invalidate_data_cache:
	push	{r4-r5, r7, r9-r11}
	dmb					@ ensure ordering with previous memory accesses
	mrc	p15, 1, r0, c0, c0, 1		@ read clidr
	mov	r3, r0, lsr #23			@ move LoC into position
	ands	r3, r3, #7 << 1			@ extract LoC*2 from clidr
	beq	5f				@ if loc is 0, then no need to clean
	mov	r10, #0				@ start clean at cache level 0
1:	add	r2, r10, r10, lsr #1		@ work out 3x current cache level
	mov	r1, r0, lsr r2			@ extract cache type bits from clidr
	and	r1, r1, #7			@ mask of the bits for current cache only
	cmp	r1, #2				@ see what cache we have at this level
	blt	4f				@ skip if no cache, or just i-cache
#ifdef CONFIG_PREEMPT
	mrs	r9, cpsr			@ make cssr&csidr read atomic
	cpsid	i
#endif
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	isb					@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0		@ read the new csidr
#ifdef CONFIG_PREEMPT
	msr	cpsr_c, r9
#endif
	and	r2, r1, #7			@ extract the length of the cache lines
	add	r2, r2, #4			@ add 4 (line length offset)
	movw	r4, #0x3ff
	ands	r4, r4, r1, lsr #3		@ find maximum number on the way size
	clz	r5, r4				@ find bit position of way size increment
	movw	r7, #0x7fff
	ands	r7, r7, r1, lsr #13		@ extract max number of the index size
2:	mov	r9, r7				@ create working copy of max index
3:	orr	r11, r10, r4, lsl r5		@ factor way and cache number into r11
	orr	r11, r11, r9, lsl r2		@ factor index number into r11
	mcr	p15, 0, r11, c7, c6, 2		@ invalidate by set/way
	subs	r9, r9, #1			@ decrement the index
	bge	3b
	subs	r4, r4, #1			@ decrement the way
	bge	2b
4:	add	r10, r10, #2			@ increment cache number
	cmp	r3, r10
	bgt	1b
5:	mov	r10, #0				@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb	st
	isb
	pop	{r4-r5, r7, r9-r11}
	bx	lr

.globl	clean_data_cache
clean_data_cache:
	push	{r4-r5, r7, r9-r11}
	dmb					@ ensure ordering with previous memory accesses
	mrc	p15, 1, r0, c0, c0, 1		@ read clidr
	mov	r3, r0, lsr #23			@ move LoC into position
	ands	r3, r3, #7 << 1			@ extract LoC*2 from clidr
	beq	5f				@ if loc is 0, then no need to clean
	mov	r10, #0				@ start clean at cache level 0
1:	add	r2, r10, r10, lsr #1		@ work out 3x current cache level
	mov	r1, r0, lsr r2			@ extract cache type bits from clidr
	and	r1, r1, #7			@ mask of the bits for current cache only
	cmp	r1, #2				@ see what cache we have at this level
	blt	4f				@ skip if no cache, or just i-cache
#ifdef CONFIG_PREEMPT
	mrs	r9, cpsr			@ make cssr&csidr read atomic
	cpsid	i
#endif
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	isb					@ isb to sych the new cssr&csidr
	mrc	p15, 1, r1, c0, c0, 0		@ read the new csidr
#ifdef CONFIG_PREEMPT
	msr	cpsr_c, r9
#endif
	and	r2, r1, #7			@ extract the length of the cache lines
	add	r2, r2, #4			@ add 4 (line length offset)
	movw	r4, #0x3ff
	ands	r4, r4, r1, lsr #3		@ find maximum number on the way size
	clz	r5, r4				@ find bit position of way size increment
	movw	r7, #0x7fff
	ands	r7, r7, r1, lsr #13		@ extract max number of the index size
2:	mov	r9, r7				@ create working copy of max index
3:	orr	r11, r10, r4, lsl r5		@ factor way and cache number into r11
	orr	r11, r11, r9, lsl r2		@ factor index number into r11
	mcr	p15, 0, r11, c7, c10, 2		@ clean by set/way
	subs	r9, r9, #1			@ decrement the index
	bge	3b
	subs	r4, r4, #1			@ decrement the way
	bge	2b
4:	add	r10, r10, #2			@ increment cache number
	cmp	r3, r10
	bgt	1b
5:	mov	r10, #0				@ swith back to cache level 0
	mcr	p15, 2, r10, c0, c0, 0		@ select current cache level in cssr
	dsb	st
	isb
	pop	{r4-r5, r7, r9-r11}
	bx	lr