#include <linux/init.h>
#include <linux/pid.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/highmem.h> 
//#include <linux/signal_types.h> 
//#include <linux/signal.h> 
#include <linux/latencytop.h> 
#include <linux/lockdep.h> 
#include <linux/perf_event.h> 
#include <linux/gfp.h> 
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
	struct task_struct *task; 
	struct task_struct *new_task = kmalloc(sizeof(struct task_struct), GFP_USER);
	struct kernel_siginfo info;
	pid_t process_pid;
	//int i;   
	//int ret;
	int fnd = 0;

	switch(cmd) {
		case IOCTL_TERM:
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
					
					/* make a new task_struct and try to start that process */
					
					memcpy(new_task->uclamp_req, task->uclamp_req, UCLAMP_CNT * sizeof(struct uclamp_se));
					memcpy(new_task->uclamp, task->uclamp, UCLAMP_CNT * sizeof(struct uclamp_se));
					memcpy(new_task->pid_links, task->pid_links, PIDTYPE_MAX * sizeof(struct hlist_node));
					memcpy(new_task->comm, task->comm, TASK_COMM_LEN * sizeof(char));
					memcpy(new_task->perf_event_ctxp, task->perf_event_ctxp, 
							perf_nr_task_contexts * sizeof(struct perf_event_context));
					memcpy(new_task->numa_faults_locality, task->numa_faults_locality, 3 * sizeof(unsigned long));
					
					printk(KERN_INFO "TasksModule: Copied data to the new task_struct");
					new_task->__state = TASK_INTERRUPTIBLE;
					printk(KERN_INFO "TasksModule: new process state is %d", new_task->__state);

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
