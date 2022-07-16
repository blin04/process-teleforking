#include<linux/ioctl.h>

#define IOCTL_ALL	_IO('a', 'a')
#define IOCTL_ONE	_IOW('a', 'b', int *)
