
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int fd;  //声明设备描述符

void my_signal_fun(int signum)
{
	unsigned char key_val;
	read(fd, &key_val, 1);
	printf("key_val: 0x%x\n", key_val);
}

int main(int argc, char **argv)
{
	
	unsigned char key_val;
	int ret;
	int Oflags;
	
	
	signal(SIGIO, my_signal_fun);//相当于注册一个信号及处理函数。（参数类似中断号与中断处理函数）

	
	//根据设备描述符打开设备
	fd = open("/dev/buttons",O_RDWR);	
	if(fd < 0)
	{
		printf("can't open\n");
	}
	
	fcntl(fd, F_SETOWN, getpid());   //告诉fd驱动内核 信号要发到哪个进程 
	
	Oflags = fcntl(fd, F_GETFL); 
	fcntl(fd, F_SETFL, Oflags | FASYNC);  //添加 FASYNC 标志，即是让设备文件fd支持异步通知mode


	while (1)
	{
		sleep(1000);
	}
	
	return 0;
}


