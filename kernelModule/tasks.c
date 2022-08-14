// kako include-ovati fajl iz kernel/sched/sched.h

#include <linux/init.h>
#include <linux/pid.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/perf_event.h> 
#include <linux/delay.h> 
#include "commands.h"

#define MAJOR_NUM	100
#define MAX_LENGTH	255

MODULE_LICENSE("Dual BSD/GPL");

static int tasks_open(struct inode *device_file, struct file *instance) {
	printk(KERN_INFO "TasksModule: opened device file!\n");
	return 0;
}

static int tasks_close(struct inode *device_file, struct file *instance) {
	printk(KERN_INFO "TasksModule: closed device file!\n");
	return 0;
}

static long int tasks_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	// defining variables used in switch cases
	struct kernel_clone_args kargs;
	struct task_struct *task; 
	struct kernel_siginfo info;
	pid_t process_pid;
	int ret;
	int fnd = 0;

	switch(cmd) {
		case IOCTL_CLN:
			/*
			 * Find process with the given PID
			 * and clone it
			 */

			if (copy_from_user(&process_pid, (pid_t *)arg, sizeof(process_pid))) {
				printk(KERN_ALERT "TasksModule: Error copying data from user!\n");
				return -1;
			}
			else {
				printk(KERN_INFO "TasksModule: Sucessfuly copied process PID from the user\n");
		
				/*	Finding the process... */	
				for_each_process(task) {
					if (task->pid == process_pid) {
						printk(KERN_INFO "TasksModule: Found the process \n");
						fnd = 1;
						break;
					}
				}
				
				// check if process was found;
				if (fnd) {
					/* stop found process and restart it later */

/*					memset(&info, 0, sizeof(struct kernel_siginfo));
					info.si_signo = SIGSTOP;
					ret = send_sig_info(SIGSTOP, &info, task);
					if (ret < 0) {
						printk(KERN_INFO "error sending signal\n");
					}
					printk(KERN_INFO "TasksModule: sent stop signal\n");
					task->__state = __TASK_STOPPED;
					printk(KERN_INFO "TasksModule: Process state is %d\n", task->__state); */


					kargs.exit_signal = SIGCHLD;
					kargs.parent_tid = &(task->pid);

					kernel_clone(&kargs);

/*					ret = wake_up_process(task);
					if (ret == 0) {
						printk(KERN_INFO "TasksModule: process was already running\n");
					}
					else printk(KERN_INFO "TasksModule: process woken up\n");

					msleep(8000);

					memset(&info, 0, sizeof(struct kernel_siginfo));
					info.si_signo = SIGCONT;
					ret = send_sig_info(SIGCONT, &info, task);
					if (ret < 0) {
						printk(KERN_INFO "error sending signal\n");
					}
					printk(KERN_INFO "TasksModule: sent cont signal\n"); */

					return 0;
				}
			} 
		default:
			return -1;
	}
	return 0;
}


struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = tasks_open,
	.release = tasks_close,
	.unlocked_ioctl = tasks_ioctl
};

/*------------------------------------*/

static int hello_init(void)
{
	printk(KERN_ALERT "TasksModule: initialization function\n");

	int number;
	number = register_chrdev(MAJOR_NUM, "tasks", &fops);
	if (number < 0) {
		printk(KERN_ALERT "TasksModule: Failed to register the device. Change the major number!\n");
		return -1;
	}
	printk(KERN_INFO "TasksModule: Sucessful registration, Major Number is: %d", number);

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "TasksModule: exit function\n");
	unregister_chrdev(MAJOR_NUM, "tasks");
}

module_init(hello_init);
module_exit(hello_exit);
