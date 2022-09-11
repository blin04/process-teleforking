// kako include-ovati fajl iz kernel/sched/sched.h

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "commands.h"

#define MAJOR_NUM	100

MODULE_LICENSE("Dual BSD/GPL");

static struct task_struct *find_task(pid_t pid)
{
	struct task_struct *task;
	for_each_process(task) {
		if (task->pid == pid) {
			return task;
		} 
	}
	return task;
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
	struct task_struct *task;
	struct files_struct *current_files;
	struct fdtable *files_table;
	struct path files_path;
	int i = 0;
	pid_t process_pid;

	char *final_path;
	char *buff = (char *)kmalloc(GFP_KERNEL, 100*sizeof(char));

	switch(cmd) {
		case IOCTL_CLN:
			if (copy_from_user(&process_pid, (pid_t *)arg, sizeof(process_pid))) {
				printk(KERN_ALERT "TasksModule: Couldn't copy PID from the user!\n");
				return -1;
			}

			printk(KERN_INFO "TasksModule: Copied PID sucessfuly, %d\n", process_pid);
			printk(KERN_INFO "TasksModule: Open files are... \n");

			
			task = find_task(process_pid);
			
			current_files = task->files;
			files_table = files_fdtable(current_files);
			while(files_table->fd[i] != NULL) {
				files_path = files_table->fd[i]->f_path;
				final_path = d_path(files_path.dentry, files_path.mnt, buff, 100*sizeof(char));

				printk(KERN_INFO "**** %s *****\n", final_path);
				++i;
			}
			
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
