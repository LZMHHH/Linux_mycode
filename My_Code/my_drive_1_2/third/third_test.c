
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd;  //声明设备描述符
	unsigned char key_val;
	
	//根据设备描述符打开设备
	fd = open("/dev/buttons",O_RDWR);
	
	if(fd < 0)
	{
		printf("can't open\n");
	}
	
	while(1)
	{
		read(fd, &key_val, 1);
		printf("key_val = 0x%x\n", key_val);
		sleep(3);
	}
	
	return 0;
}


