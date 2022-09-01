obj-m += tasks.o

all:
	make -C /lib/modules/2.6.23/build M=$(PWD) modules

clean:
	make -C /lib/modules/2.6.23/build M=$(PWD) clean
