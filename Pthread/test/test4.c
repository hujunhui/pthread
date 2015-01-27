#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

static volatile int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *mythread_fun(void *arg);

int main(int argc, char *argv[])
{
	pthread_t pthid1, pthid2;
	int ret;
	
	printf("main begin: counter = %d\n", counter);	

	ret = pthread_create(&pthid1, NULL, mythread_fun, "A");
	assert(ret == 0);
	ret = pthread_create(&pthid2, NULL, mythread_fun, "B");
	assert(ret == 0);

	ret = pthread_join(pthid1, NULL);	
	assert(ret == 0);
	ret = pthread_join(pthid2, NULL);
	assert(ret == 0);

	printf("main end:   counter = %d\n", counter);	

	exit(EXIT_SUCCESS);
}

void *mythread_fun(void *arg)	// 该函数是线程安全的，但不是可重入
{
	int index;
	
	printf("%s begin:\n", (char *)arg);
	for (index = 0; index < 1e7; index++)
	{
		pthread_mutex_lock(&mutex);
		mythread_fun((void *)"C");
		counter++;
		pthread_mutex_unlock(&mutex);
	}
	printf("%s end:  \n", (char *)arg);

	return NULL;
}