#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "commands.h"

int main()
{
	int dev = open("/dev/tasks", O_RDONLY);
	if (dev == -1) {
		printf("Opening the file wasn't possible\n");
		return 0;
	}
	printf("Device file opened sucessfuly!\n");

	ioctl(dev, IOCTL_ALL);
	printf("First IOCTL call...\n");

	char process_name[100];
//	int	process_pid;
	printf("Enter PID of the process: ");
	//scanf("%d", &process_pid);
	scanf("%s", process_name);

	//ioctl(dev, IOCTL_ONE, process_pid);
	ioctl(dev, IOCTL_ONE, process_name);
	printf("Second IOCTL call...\n");

	close(dev);
}
