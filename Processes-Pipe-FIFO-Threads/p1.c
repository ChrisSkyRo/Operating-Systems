#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int n, i;
	printf("n = ");
	scanf("%d", &n);
	for(i = 0;i < n;i++)
	{
		int f = fork();	
		if(f == -1)
			perror("Error on fork ");
		else if(f == 0)
		{
			printf("Process ID: %d Parent ID: %d\n", getpid(), getppid());
			exit(0);
		}
		else
			printf("Child ID: %d Process ID: %d\n", f, getpid());
	}
	for(i = 0;i < n;i++)
		wait(0);
	return 0;
}

