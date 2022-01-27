#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "../include/ioctl.h"

using namespace std;

int main(int argc, char *argv[])
{
	int fd, i, ret;
    char buf[4096*16];
	fd = 0;

	fprintf(stderr, "%s:%d: Usage: %s cmd\n", __FILE__, __LINE__, *argv);
	fprintf(stderr, "1 cmd: gree len switch on\n");
	fprintf(stderr, "2 cmd: gree len switch off\n");
	fprintf(stderr, "3 cmd: red len switch on\n");
	fprintf(stderr, "4 cmd: red len switch off\n");

	bool green_on_cmd = false;
	bool red_on_cmd = false;
	std::string file_name = DEVICE_NAME;

    if(argc >= 2)
    {
		int argument = atoi(argv[1]);
		switch(argument)
		{
			case 1:
			{
				green_on_cmd = true;
				break;
			}
			case 2:
			{
				green_on_cmd = false;
				break;
			}
			case 3:
			{
				red_on_cmd = true;
				break;
			}
			case 4:
			{
				red_on_cmd = false;
				break;
			}
		}
	}


	fd = open(file_name.c_str(), O_RDWR);
    if (fd == -1) {
		fprintf(stderr, "%s:%d: Can't open %s\n", __FILE__, __LINE__, file_name.c_str());
		return 1;
    }

	if(green_on_cmd )
    {
		if (ioctl(fd, GREEN_LEN_ON) == -1)
		{
			perror("query_apps ioctl");
		}
    }
	else
	{
		if (ioctl(fd, GREEN_LEN_OFF) == -1)
		{
			perror("query_apps ioctl");
		}
	}

	if(red_on_cmd )
	{
		if (ioctl(fd, RED_LEN_ON) == -1)
		{
			perror("query_apps ioctl");
		}
	}
	else
	{
		if (ioctl(fd, RED_LEN_OFF) == -1)
		{
			perror("query_apps ioctl");
		}
	}

    close(fd);

    return 0;
}
