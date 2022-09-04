#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int boltz(int n)
{
	if(n%7 == 0)
		return 1;
	while(n)
	{
		if(n%10 == 7)
			return 1;
		n /= 10;
	}

	return 0;
}

void game(int pipes[][2], int index, int n)
{
	// reads the number and increments it
	int x, c;
	read(pipes[index][0], &x, sizeof(int));
	x++;
	// the last process sends the number to the 1st process
	if(index+1 == n)
	{
		write(pipes[0][1], &x, sizeof(int));
		index = 0;
	}
	// any other process sends the number to the next process
	else
	{
		write(pipes[index+1][1], &x, sizeof(int));
		index++;
	}
	// plays a round of the game
	c = rand()%3;
	if(boltz(x))
	{
		if(c > 0)
			printf("boltz\n");
		// fail
		else
		{
			printf("%d\n", x);
			x = 0;
			write(pipes[0][1], &x, sizeof(int));
			exit(0);
		}
	}
	else 
		printf("%d\n", x);
	// if the next process isn't the parent, fork()
	if(index != n){
		int f = fork();
		if(f == -1)
			perror("Error on fork\n");
		else if(f == 0)
			game(pipes, index, n);
		wait(0);
	}
	if(index > 0)
		exit(0);
}

int main(int argc, char **argv)
{
	int n, k, i;
	printf("n = ");
	scanf("%d", &n);
	srand(time(NULL));
	// open pipes
	int pipes[n][2];
	for(i = 0; i < n; i++)
	{
        if(pipe(pipes[i]) < 0)
		{
            perror("Error on opening pipe");
            exit(1);
        }
    }
	k = 1;
	write(pipes[0][1], &k, sizeof(int));
	// game loop
	while(1)
	{
		game(pipes, 0, n);
		read(pipes[0][0], &k, sizeof(int));
		if(k == 0)
			break;
	}
	// close pipes
	for(i = 0;i < n;i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
	return 0;
}

