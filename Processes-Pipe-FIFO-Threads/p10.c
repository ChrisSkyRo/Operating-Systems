#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n, len, *ar;
pthread_mutex_t m;
pthread_cond_t cond;

void *f(void *a)
{
	int x;
	// p is parity
	// 0 even, 1 odd
	int p = *((int*)a);
	// lock the mutex for pthread_cond_wait
	pthread_mutex_lock(&m);
	while(len%2 != p)
		pthread_cond_wait(&cond, &m);
	while(len != n)
	{
		// generate number
		x = random()%51*2+p;
		// insert the number in the array
		ar[len++] = x;
		// signal the other thread after one iteration
		pthread_cond_signal(&cond);
		while(len%2 != p && len < n)
			pthread_cond_wait(&cond, &m);
	}
	// do one more pthread_cond_signal to unlock the other thread
	pthread_cond_signal(&cond);
	// unlock mutex
	pthread_mutex_unlock(&m);
	return NULL;
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	// read n and create array
	printf("n = ");
	scanf("%d", &n);
	ar = (int*)malloc(n*sizeof(int));
	// create threads
	int p1 = 0, p2 = 1;
	pthread_t t1, t2;
	pthread_mutex_init(&m, NULL);
	pthread_create(&t1, NULL, f, &p1);
	pthread_create(&t2, NULL, f, &p2);
	// join threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	// print array
	int i;
	for(i = 0;i < n;i++)
		printf("%d ", ar[i]);
	printf("\n");
	// destroy mutex and free memory
	pthread_mutex_destroy(&m);
	free(ar);
	return 0;
}

