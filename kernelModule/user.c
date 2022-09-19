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

	// starting desired process in zombie state
	int pid = fork();
	if (pid == 0) {
		int run = 1;
		while(run) {
			sleep(1);
		}
	/*	char *args[] = {NULL};
		execv("./proces", args); */ 
	}
	printf("User program PID is %d\n", getpid());

	int proc_pid;
	printf("Enter proces PID: ");
	scanf("%d", &proc_pid);

	// setup task_struct of the running  
	// process in the device driver
	ioctl(dev, IOCTL_SETUP, &proc_pid);

	// stop the running process
	ioctl(dev, IOCTL_STOP);

	// copy task_struct of the stopped process to
	// task_struct of the zombie process 
	ioctl(dev, IOCTL_CPY);

	// start the zombie process 
	ioctl(dev, IOCTL_RUN);

	close(dev);
}
