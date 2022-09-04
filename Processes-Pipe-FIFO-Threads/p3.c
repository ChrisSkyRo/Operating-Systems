#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int f;

void parentWait(int signal)
{
	printf("Parent waiting for child to terminate\n");
	wait(0);
}

void handler(int signal)
{
	if(f == 0)
	{
		printf("Child received SIGUSR1 signal, terminating child\n");
		exit(0);
	}
	else
	{
		printf("Parent received SIGUSR1 signal, killing child\n");
		kill(f, SIGUSR1);
		wait(0);
		printf("Terminating parent\n");
		exit(0);
	}
}

int main(int argc, char **argv)
{
	f = fork();
	if(f == -1)
		perror("Error on fork\n");
	signal(SIGUSR1, handler);
	if(f != 0)
	{
		printf("Parent PID: %d Child PID: %d\n", getpid(), f);
		signal(SIGCHLD, parentWait);
	}
	while(1)
	{
		if(f == 0)
		{
			printf("Child running...\n");
			sleep(1);
		}
		else
		{
			printf("Parent running...\n");
			sleep(3);
		}
	}
	return 0;
}

