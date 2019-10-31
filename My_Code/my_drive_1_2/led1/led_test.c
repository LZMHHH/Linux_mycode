
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

//argc: 元素个数
//argv: 具体元素（包括命令本身）
//例如：#root$ ./first abc
//      argc = 2; argv[0] = #root$ ./first  的起始地址
//                argv[1] = abc             的起始地址
int main(int argc, char **argv)
{
	if(argc != 4  && argc != 3)  //因为要操作三个LED，如果传入的参数不是四个，程序打印帮助信息
	{
		if(argc > 0)
		{
			printf("argv[0] : %s \n",argv[0]);
		}
		if(argc > 1)
		{
			printf("argv[1] : %s \n",argv[1]);
		}
		if(argc > 2)
		{
			printf("argv[2] : %s \n",argv[2]);
		}
		if(argc > 3)
		{
		    printf("argv[3] : %s \n",argv[3]);
		}
		printf("The count of arguments is wrong\nUsage: \n");
        printf("%s LED1_state LED2_state LED3_state\n", argv[1]);
        printf("\t%s on off on\n means: turn on the LED1 and LED3; turn off the LED2", argv[1]);
		
		return -1;
	}
	if(argc == 3 && (strcmp("off", argv[2]) && strcmp("on", argv[2])))
	{
		printf("This command shoud be : %s n k\n",argv[0]);
		printf("'n' could be 0 、1、2 or 3. \n");
		printf("'k' could be off or on. \n");
		return -2;
	}
	
	
	
	int fd;       //声明设备描述符
	int val[3];
	
	
	
	
	
	if(argc == 4)
	{
		//根据设备描述符打开设备
		fd = open("/dev/leds",O_RDWR);  //读写模式打开
		
		if(fd < 0)
		{
			printf("can't open\n");
		}
		
		printf("open successfully. the file description is %d\n", fd);
		
		//参数处理
		int i = 0;
		for(;i < argc - 1; i++)
		{
			if(!strcmp("on", argv[i+1]))
			{
				// 1亮灯
				val[i] = 1;
			}
			else if(!strcmp("off", argv[i+1]))
			{
				// 1灭灯
				val[i] = 0;
			}
			else
			{
				//出错
				val[i] = -1;
				printf("arguments error. the device description is %d\n", i+4);
			}
		}
	
		write(fd, val, 12);   //把数据写入设备文件
	}
	else if(argc == 3)
	{
		fd = open(argv[1], O_RDWR);
		if (fd < 0)
		{
			printf("error, can't open %s\n", argv[1]);
			return 0;
		}
		
		//参数处理
		val[0] = (int)*argv[1] - 48;  //设备号
		//printf("%d \n",val[0]);
		if(!strcmp("on", argv[2]))
		{
			// 1亮灯
			val[1] = 1;
		}
		else if(!strcmp("off", argv[2]))
		{
			// 1灭灯
			val[1] = 0;
		}
		
		write(fd, val, 8);   //把数据写入设备文件
	}
	
	return 0;
}


