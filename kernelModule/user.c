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

	int pid = fork();
	if (pid == 0) {
		// starting desired process in zombie state
		char *args[] = {NULL};
		execv("./proces", args); 
	}
	printf("User program PID is %d\n", getpid());

	/* continue process with this pid */
	int proc_pid;
	printf("Enter proces PID: ");
	scanf("%d", &proc_pid);

	// setup necessary task_struct in 
	// the device driver
	ioctl(dev, IOCTL_STP, &proc_pid);

	// copy to new task 
	ioctl(dev, IOCTL_CPY);

	// start the new task
	ioctl(dev, IOCTL_RUN);

	close(dev);
}
