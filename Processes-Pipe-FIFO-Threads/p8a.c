#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	// create fifos
	if(mkfifo("a2b", 0600) < 0 || mkfifo("b2a", 0600) < 0)
	{
		perror("Error creating fifo ");
		exit(1);
	}
	// open fifos
	int fd_a2b = open("a2b", O_WRONLY);
	int fd_b2a = open("b2a", O_RDONLY);
	if(fd_a2b < 0)
	{
		perror("Error opening a2b ");
		exit(1);
	}
	if(fd_b2a < 0)
	{
		perror("Error opening b2a ");
		exit(1);
	}
	// generate number
	srand(getpid());
	int x = random()%151+50;
	printf("Generated %d\n", x);
	// execution loop
	while(x > 5)
	{
		if(x%2 == 1)
			x++;
		// send to b
		if(write(fd_a2b, &x, sizeof(int)) < 0)
			perror("Error writing to b ");
		printf("Sent %d to b\n", x);
		// receive from b
		if(read(fd_b2a, &x, sizeof(int)) < 0)
			perror("Error reading from b");
		printf("Received %d from b\n", x);
	}
	// close and delete fifos
	close(fd_a2b);
	close(fd_b2a);
	if(unlink("a2b") < 0)
		perror("Error unlinking a2b ");
	if(unlink("b2a") < 0)
		perror("Error unlinking b2a ");
	return 0;
}

