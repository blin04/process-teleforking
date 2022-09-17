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

	/* continue process with this pid */
	int pid;
	printf("Enter proces PID: ");
	scanf("%d", &pid);

	// setup necessary task_struct in 
	// the device driver
	ioctl(dev, IOCTL_STP, &pid);

	ioctl(dev, IOCTL_CNT);

	close(dev);
}
