#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char **argv)
{
	int p2c[2], c2p[2], i;
	if(argc < 2)
		printf("Give an integer as argument\n");
	else
	{
		pipe(p2c);
		pipe(c2p);
		srand(time(NULL));
		int n = atoi(argv[1]);
		int f = fork();
		if(f == -1)
		{
			perror("Error on fork ");
			exit(1);
		}
		// child
		else if(f == 0)
		{
			close(p2c[1]);
			close(c2p[0]);
			float avg = 0;
			int k;
			for(i = 0;i < n;i++)
			{
				if(read(p2c[0], &k, sizeof(int)) < 0)
				{
					perror("Error reading from parent ");
					close(p2c[0]);
					close(c2p[1]);
					exit(1);
				}
				avg += k;
			}
			avg /= n;
			if(write(c2p[1], &avg, sizeof(float)) < 0)
			{
				perror("Error writing to parent ");
				close(p2c[0]);
				close(c2p[1]);
				exit(1);
			}
			close(p2c[0]);
			close(c2p[1]);
			exit(0);
		}
		// parent
		else
		{
			close(p2c[0]);
			close(c2p[1]);
			int k;
			float avg;
			printf("The random numbers are: ");
			for(i = 0;i < n;i++)
			{
				k = random()%2000000;
				printf("%d ", k);
				if(write(p2c[1], &k, sizeof(int)) < 0)
				{
					perror("Error writing to child  ");
					close(p2c[1]);
					close(c2p[0]);
					exit(1);
				}
			}
			if(read(c2p[0], &avg, sizeof(float)) < 0)
			{
				perror("Error reading from child ");
				close(p2c[1]);
				close(c2p[0]);
				exit(1);
			}
			printf("\nAverage = %.2f\n", avg);
			close(p2c[1]);
			close(c2p[0]);
		}		
	}
	return 0;
}
