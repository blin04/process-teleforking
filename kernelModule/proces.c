#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
	printf("Hello, this is a test program.\n");
	printf("My PID is: %d\n", getpid());
/*	printf("What's your name?\n");

	char name[50];
	scanf("%s", name);

	printf("Hello, %s\n", name);*/

	int run = 1;
	while(run) {
		sleep(1);
		printf("nesto\n");
	}

}
