#include<linux/ioctl.h>

#define IOCTL_STP	_IOW('a', 'a', int *)
#define IOCTL_CNT	_IO('a', 'b')
#define IOCTL_CPY	_IO('a', 'c')
#define IOCTL_RUN	_IO('a', 'd')
