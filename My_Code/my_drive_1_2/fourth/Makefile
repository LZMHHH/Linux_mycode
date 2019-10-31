#驱动程序实际属于内核的一部分，那么在编译的时候就需要使用已经编译好的内核，来编译驱动程序，这点是  重点
#内核目录
KERN_DIR = /work/system/linux-2.6.22.6

#M=‘pwd’表示，生成的目标放在pwd命令的目录下
all:
	make -C $(KERN_DIR) M=`pwd` modules

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

#加载到module的编译链中，内核会编译生成出.ko文件，作为一个模块
obj-m	+= fourth.o
