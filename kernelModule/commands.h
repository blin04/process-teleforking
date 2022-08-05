#include<linux/ioctl.h>

#define IOCTL_ALL	_IO('a', 'a')
#define IOCTL_TERM	_IOW('a', 'b', int *)
