

.text
.global _start

_start:
	/* 配置内存：sp 栈 */
	ldr sp, =4096       /* nand 启动（栈设置为顶部） */
	//ldr sp, =0x40000000 /* nor  启动（栈设置为顶部） */
	
	
	/* 调用main */
	bl main
	
halt:
	b halt
	
	
	