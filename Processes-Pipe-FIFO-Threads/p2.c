#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void creator(int n)
{
	if(n > 0)
	{
		int f = fork();
		if(f == -1)
			perror("Error on fork\n");
		else if(f == 0)
		{
			printf("Process ID= %d Parent ID= %d\n", getpid(), getppid());
			creator(n-1);
			exit(0);
		}
		else
			wait(0);
	}
}

int main(int argc, char **argv)
{
	int n;
	printf("n = ");
	scanf("%d", &n);
	creator(n);
	return 0;
}

