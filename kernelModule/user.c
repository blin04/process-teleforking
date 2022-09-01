#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "commands.h"

int main()
{
	int dev = open("/dev/tasks", O_RDONLY);
	if (dev == -1) {
		printf("Opening the file wasn't possible\n");
		return 0;
	}
	printf("Device file opened sucessfuly!\n");

	// IOCTL_CLN - argument should be PID of a process
	// for now, the argument is an arbitrary number
	int temp = 5;
	ioctl(dev, IOCTL_CLN, &temp);

	close(dev);
}
