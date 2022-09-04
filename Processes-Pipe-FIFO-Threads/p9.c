#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int rows, cols;
int **mat;

struct arg_t
{
	int row, sum;
};

void* sum(void* a)
{
	int i, sum = 0;
	struct arg_t *x = (struct arg_t*)a;
	for(i = 0;i < cols;i++)
		sum += mat[x->row][i];
	x->sum = sum;
	return NULL;
}

int main(int argc, char **argv)
{
	FILE* f = fopen("matrix.txt", "r");
   	int i, j;
	if(f == NULL)
	{
		perror("Failed to open input file ");
		exit(1);
	}
	// read from file
	fscanf(f, "%d %d", &rows, &cols);
	mat = (int**)malloc(rows*sizeof(int*));
	for(i = 0;i < rows;i++)
	{
		mat[i] = (int*)malloc(cols*sizeof(int));
		for(j = 0;j < cols;j++)
			fscanf(f, "%d", &mat[i][j]);
			
	}
	fclose(f);
	// create a thread for each row
	struct arg_t *args = (struct arg_t*)malloc(rows*sizeof(struct arg_t));
	pthread_t *threads = (pthread_t*)malloc(rows*sizeof(pthread_t));
	for(i = 0;i < rows;i++)
	{
		args[i].row = i;
		pthread_create(&threads[i], NULL, sum, &args[i]);
	}
	for(i = 0;i < rows;i++)
		pthread_join(threads[i], NULL);
	// calculate sum
	int sum = 0;
	for(i = 0;i < rows;i++)
		sum += args[i].sum;
	printf("%d\n", sum);
	// free memory
	for(i = 0;i < rows;i++)
		free(mat[i]);
	free(mat);
	free(threads);
	free(args);
	return 0;
}

