#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char **argv)
{
	int p2c[2], c2p[2];
	if(pipe(p2c) == -1 || pipe(c2p) == -1)
	{
		perror("Error on creating pipe ");
		exit(1);
	}
	int f = fork();
	int x = 0;
	if(f == -1)
		perror("Error on fork ");
	else
	{
		// child
		if(f == 0)
		{
			srand(getpid());
			close(p2c[1]);
			close(c2p[0]);
			// read number from parent
			if(read(p2c[0], &x, sizeof(int)) < 0)
			{
				perror("Error reading from parent ");
				close(p2c[0]);
				close(c2p[1]);
				exit(1);
			}
			while(x != 10)
			{
				// generate number
				x = random()%10+1;
				// send number to parent
				if(write(c2p[1], &x, sizeof(int)) < 0)
				{
					perror("Error writing to parent ");
					close(p2c[0]);
					close(c2p[1]);
					exit(1);
				}
				if(x == 10)
					break;
				// read number from parent
				if(read(p2c[0], &x, sizeof(int)) < 0)
				{
					perror("Error reading from parent");
					close(p2c[0]);
					close(c2p[1]);
					exit(1);
				}
			}
			close(p2c[0]);
			close(c2p[1]);
			exit(0);
		}
		// parent
		else
		{
			srand(getpid());
			close(p2c[0]);
			close(c2p[1]);
			while(x != 10)
			{
				// generate number
				x = random()%10+1;
				printf("%d ", x);
				// send number to child
				if(write(p2c[1], &x, sizeof(int)) < 0)
				{
					perror("Error writing to child ");
					close(p2c[1]);
					close(c2p[0]);
					exit(1);
				}
				if(x == 10)
					break;
				// read number from child
				if(read(c2p[0], &x, sizeof(int)) < 0)
				{
					perror("Error reading from child ");
					close(p2c[1]);
					close(c2p[0]);
					exit(1);
				}
				printf("%d ", x);
			}
			close(p2c[1]);
			close(c2p[0]);
			wait(0);
		}
	}
	return 0;
}

