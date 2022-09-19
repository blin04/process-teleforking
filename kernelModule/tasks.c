// kako include-ovati fajl iz kernel/sched/sched.h

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/lockdep.h>
#include <linux/list.h>
#include "commands.h"

#define MAJOR_NUM	100

MODULE_LICENSE("Dual BSD/GPL");

/* --------- global variables ----------- */

// pointer to process that is started in stopped state
static struct task_struct *task;
// struct of a process that should be started as a copy of the stopped one
static struct task_struct new_task;		


static struct task_struct *find_task(pid_t pid)
{
	struct task_struct *tmp_task;
	for_each_process(tmp_task) {
		if (tmp_task->pid == pid) {
			return tmp_task;
		} 
	}
	return tmp_task;
}

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
	// defining variables that are used in various switch cases
	pid_t process_pid;

	switch(cmd) {
		case IOCTL_SETUP:
			// set's up the pointer to the task that has the given PID

			if (copy_from_user(&process_pid, (pid_t *)arg, sizeof(process_pid))) {
				printk(KERN_ALERT "TasksModule: Couldn't copy PID from the user!\n");
				return -1;
			}
			printk(KERN_INFO "TasksModule: Copied PID sucessfuly, %d\n", process_pid);

			task = find_task(process_pid);

			return 0;
		case IOCTL_STOP:
			kill_proc(task->pid, SIGSTOP, 1);	
			printk(KERN_INFO "TasksModule: Name - %s\n", task->comm);
			printk(KERN_INFO "TasksModule: State - %ld\n", task->state);

			return 0;
		case IOCTL_CNT:
			printk(KERN_INFO "TasksModule: Name - %s\n", task->comm);
			printk(KERN_INFO "TasksModule: State - %ld\n", task->state);

			// let's try to continue this process
			kill_proc(task->pid, SIGCONT, 1);
			printk(KERN_INFO "TasksModule: Process should have continued\n");
			printk(KERN_INFO "TasksModule: Name - %s\n", task->comm);
			printk(KERN_INFO "TasksModule: State - %ld\n", task->state);
			
			return 0;
		case IOCTL_CPY:
			// copies task struct

			new_task = *task;
			memcpy(new_task.pids, task->pids, PIDTYPE_MAX * sizeof(struct pid_link));
			memcpy(new_task.cpu_timers, task->cpu_timers, 3 * sizeof(struct list_head));
			memcpy(new_task.comm, task->comm, TASK_COMM_LEN * sizeof(char));
			
			printk(KERN_INFO "TasksModule: New task PID is %d\n", new_task.pid);

			return 0;
		case IOCTL_RUN:
			// starts the new task
			kill_proc(new_task.pid, SIGCONT, 1);
			printk(KERN_INFO "TasksModule: Process should have continued\n");
			printk(KERN_INFO "TasksModule: Name - %s\n", new_task.comm);
			printk(KERN_INFO "TasksModule: State - %ld\n", new_task.state);
			
			return 0;
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
	printk(KERN_INFO "TasksModule: Sucessful registration\n");

	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "TasksModule: exit function\n");
	unregister_chrdev(MAJOR_NUM, "tasks");
}

module_init(hello_init);
module_exit(hello_exit);
