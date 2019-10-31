

.text
.global _start

_start:
	/* 配置内存：sp 栈 */
	ldr sp, =4096       /* nand 启动（栈设置为顶部） */
	//ldr sp, =0x40000000 /* nor  启动（栈设置为顶部） */
	
	/* 调用app */
	mov r0, #4
	bl app
	
	/* 调用delay */
	ldr r0, =100000
	bl delay
	
	
	/* 调用app */
	mov r0, #5
	bl app
	
halt:
	b halt
	
	
	