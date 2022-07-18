#include <linux/init.h>
#include <linux/pid.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/highmem.h> 
#include "commands.h"

#define MAJOR_NUM	100
#define MAX_TASKS	512
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
	struct task_struct* tasks[MAX_TASKS];
	struct task_struct* iterate_task;
	int tasks_cnt = 0;
	int i;

	int32_t process_pid;
//	char process_name[MAX_LENGTH];

	switch(cmd) {
		case IOCTL_ALL:

			for_each_process(iterate_task) {
				tasks[tasks_cnt++] = iterate_task;
			}

			printk(KERN_INFO "TasksModule: Stored data about following processes: \n");

			for (i = 0; i < tasks_cnt; i++) {
				printk(KERN_INFO "PID: %d, NAME: %s\n", tasks[i]->pid, tasks[i]->comm);
			}

			break;
		case IOCTL_ONE:
			// print data about process, using it's pid
			if (copy_from_user(&process_pid, (int32_t *)arg, sizeof(process_pid))) {
				printk(KERN_ALERT "TasksModule: Error copying data from user!\n");
				return -1;
			}
			else {
				printk(KERN_INFO "TasksModule: Sucessfuly copied process name from the user\n");
				printk(KERN_INFO "TasksModule: Process PID is %d", process_pid);
				
				for_each_process(iterate_task) {
					if (iterate_task->pid == process_pid) {
//					if (!strcmp(iterate_task->comm, process_name)) {
						printk(KERN_INFO "TasksModule: Found the process, the name is %s\n", iterate_task->comm);
						printk(KERN_INFO "TasksModule: PID is %d\n", iterate_task->pid);
						return 0;
					}
					else {
	//					printk(KERN_INFO "TasksModule: ")
					}
				}
				printk(KERN_INFO "TasksModule: Process isn't running currently\n");
			}

			break;
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
