#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Hello, this is a test program.\n");
	printf("What's your name?\n");

	char name[50];
	scanf("%s", name);

	printf("Hello, %s\n", name);

}
