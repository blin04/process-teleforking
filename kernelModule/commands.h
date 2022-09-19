#include<linux/ioctl.h>

#define IOCTL_SETUP	_IOW('a', 'a', int *)
#define IOCTL_STOP	_IO('a', 'b')
#define IOCTL_CNT	_IO('a', 'c')
#define IOCTL_CPY	_IO('a', 'd')
#define IOCTL_RUN	_IO('a', 'e')
