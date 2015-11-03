#include "xprintf.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
queue_t queue;

void *producer(void *argv)
{
	long i = -10000000000;
	unsigned long lu = 1000000000023423432;
	float f = 3.1415926;
	unsigned int ui = 19000;
	printf("Producer : Hello I am producer #%ld. Ready to produce data\n", pthread_self());

	pthread_mutex_lock(&mutex);
	printf("Producer...\n");

	xprintf(queue, "%i, %ld, haha, %c, %i \n", -33,i,'5', 100);

	xprintf(queue, "%f xixixi hahah  \n", f);

	xprintf(queue, "%s, %lu, %u\n", "hello world!", lu, ui);

	pthread_cond_signal(&cond);
	sleep(1);
	pthread_mutex_unlock(&mutex);

}

void *consumer(void *argv)
{

	FILE* fp;
	printf("Consumer : Hello I am consumer #%ld. Ready to consume Data\n", pthread_self());

	pthread_mutex_lock(&mutex);
	printf("Consumer...\n");

	while(get_length(queue) == 0)
	{
		pthread_cond_wait(&cond,&mutex);
	}
	fp = fopen("testing.txt", "w");
	xprintf_fini(queue, fp);
	fclose(fp);
	sleep(1);
	pthread_mutex_unlock(&mutex);

}


int main(){


	pthread_t thread[2];
	pthread_attr_t attributes;
	int tid1,tid2;
	queue =  xprintf_init();
	
	printf("\nMain thread started\n");
	pthread_attr_init(&attributes);
	if((tid1 = pthread_create(&thread[0],&attributes,producer,queue)))
	{
		printf("\nError in the producer thread\n");
		printf("\n");
	}

	if((tid2 = pthread_create(&thread[1],&attributes,consumer,queue)))
	{
		printf("\nERror in the consumer thread\n");
	}

	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	printf("\n\t******************* DONE *********************\n");
	return 0 ;


}