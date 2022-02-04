/* Barebones ARM/Raspberry Pi Standalone (kernel) linker script */
/* http://wiki.osdev.org/Raspberry_Pi_Bare_Bones		*/

/* Entry point */
ENTRY(_start)

__svc_stack_size = 0x08000;
 
SECTIONS
{
	/* Sets current address to 0x8000 */
	. = 0x10000;

	/* Sets start and text_start to the current address (0x8000) */
	__start = .;

	/************************ Text Section ********************/
	__text_start__ = .;
	.text :
	{
		/* Put boot section first */
		KEEP(*(.text.boot))
		
		/* Then put .text */
		*(.text)
	}
	/* align to page size */
	. = ALIGN(4096); 
	__text_end__ = .;
 
	/****************** Read-Only Data Section ********************/
	__rodata_start__ = .;
	.rodata :
	{
		/* Include all rodata sections */
		*(.rodata)
	}
	. = ALIGN(4096); /* align to page size */
	__rodata_end__ = .;

	/****************** Data Section ********************/
	__data_start__ = .;
	.data :
	{
		/* Include all data sections */
		*(.data)
	}
	. = ALIGN(4096); /* align to page size */
	__data_end__ = .;
 
	/****************** BSS Section ********************/
	__bss_start__ = .;
	.bss :
	{
		bss = .;
		*(.bss)
	}
	. = ALIGN(4096); /* align to page size */
	__bss_end__ = .;
	__end_kernel = .;

	/****************** MODE STACKS ********************/
	. = ALIGN(4096);
	. = . + __svc_stack_size; 
	__svc_stack_top_core0 = .;

	. = ALIGN(4096);
	. = . + __svc_stack_size; 
	__svc_stack_top_core1 = .;

	. = ALIGN(4096);
	. = . + __svc_stack_size; 
	__svc_stack_top_core2 = .;

	. = ALIGN(4096);
	. = . + __svc_stack_size; 
	__svc_stack_top_core3 = .;

	/****************** End of executable **************/
	__end = .;
}
