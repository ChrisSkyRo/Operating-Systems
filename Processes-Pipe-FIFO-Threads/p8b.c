#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	// open fifos
	int fd_a2b = open("a2b", O_RDONLY);
	int fd_b2a = open("b2a", O_WRONLY);
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
	int x = 10;
	// execution loop
	while(x > 5)
	{
		// read from a
		if(read(fd_a2b, &x, sizeof(int)) < 0)
			perror("Error reading from a ");
		x /= 2;
		// send to a
		if(write(fd_b2a, &x, sizeof(int)) < 0)
			perror("Error writing to a ");
	}
	// close fifos
	close(fd_a2b);
	close(fd_b2a);
	return 0;
}

