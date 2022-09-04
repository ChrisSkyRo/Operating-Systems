#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	if(argc < 2)
		printf("Enter at least one command\n");
	else
	{
		struct timeval t0, t1;
		float duration;
		gettimeofday(&t0, NULL);
		int f = fork();
		if(f == -1)
		{
			perror("Error on fork\n");
			exit(1);
		}
		if(f == 0){
			int cmd = execvp(argv[1], argv+1);
			if(cmd == -1)
			{
				perror("Error running the command\n");
				exit(1);
			}
			exit(0);
		}
		else
		{
			wait(0);
			gettimeofday(&t1, NULL);
			duration = ((t1.tv_sec-t0.tv_sec)*1000.0 + (t1.tv_usec-t0.tv_usec)/1000.0)/1000.0;
			printf("Duration: %f seconds\n", duration);
		}
	}
	return 0;
}
