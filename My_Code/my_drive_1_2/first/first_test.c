
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd;  //声明设备描述符
	int val = 1;  //随便定义变量传入
	
	//根据设备描述符打开设备
	fd = open("/dev/first",O_RDWR);
	
	if(fd < 0)
	{
		printf("can't open\n");
	}
	
	write(fd,&val,4); //根据文件描述符调用write
	
	return 0;
}


