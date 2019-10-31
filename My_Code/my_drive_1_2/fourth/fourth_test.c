
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>

int main(int argc, char **argv)
{
	int fd;  //声明设备描述符
	unsigned char key_val;
	int ret;
	
	struct pollfd fds[1];
	
	//根据设备描述符打开设备
	fd = open("/dev/buttons",O_RDWR);	
	if(fd < 0)
	{
		printf("can't open\n");
	}
	
	fds[0].fd     = fd;
	fds[0].events = POLLIN;   //it have data to read.
	while(1)
	{
		ret = poll(fds, 1, 5000);
		if(ret == 0)
		{
			printf("time out\n");
		}
		else
		{
			read(fd, &key_val, 1);
			printf("key_val = 0x%x\n", key_val);
		}
		
	}
	
	return 0;
}


