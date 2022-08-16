// kako include-ovati fajl iz kernel/sched/sched.h

#include <linux/init.h>
#include <linux/pid.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/fdtable.h>
#include <linux/dcache.h>
#include <linux/gfp.h>
#include <linux/perf_event.h> 
#include <linux/delay.h> 
#include "commands.h"

#define MAJOR_NUM	100

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
	//struct kernel_siginfo info;
	struct task_struct *task; 
	struct files_struct *current_files;
	struct fdtable *files_table;
	struct path files_path;
	unsigned int *fds;
	pid_t process_pid;
	int ret;
	int fnd = 0;
	int i = 0;

	// variable for printing file path
	char *cwd; 	
	// buffer used by d_path
	char *buff = (char *)kmalloc(100 * sizeof(char), GFP_KERNEL);

	switch(cmd) {
		case IOCTL_CLN:
			/*
			 * Print paths of files which the task, that has the 
			 * given PID, has opened
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
					/* print all open files of a process */
					printk(KERN_INFO "TasksModule: Task %d uses following files\n", task->pid);

					current_files = task->files;
					files_table = files_fdtable(current_files);

					while(files_table->fd[i] != NULL) {
						files_path = files_table->fd[i]->f_path;
						// make string of path
						cwd = d_path(&files_path, buff, 100*sizeof(char));

						printk(KERN_INFO "%d, %s", i, cwd);
						++i;
					}

/*			this chuck of code is going to be useful later		
					memset(&info, 0, sizeof(struct kernel_siginfo));
					info.si_signo = SIGSTOP;
					ret = send_sig_info(SIGSTOP, &info, task);
					if (ret < 0) {
						printk(KERN_INFO "error sending signal\n");
					}
					printk(KERN_INFO "TasksModule: sent stop signal\n");
*/
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
