/*
Author: Elizabeth Ralyea
Date: 2/5/2018
Purpose: create a bounded buffer program
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

using namespace std;

typedef int buffer_item;

#define BUFFER_SIZE 5
#define TRUE 1

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

buffer_item buffer[BUFFER_SIZE];

int insert_item(buffer_item item){
	int try_;

	try_ = sem_trywait(&empty);
	if(try_ == -1)
		return -1; 

	sem_wait(&empty);//finding empty semaphore
	
	pthread_mutex_lock(&mutex);// getting mutex lock to protect buffer
	buffer[insertPointer++] = item;
	insertPointer = insertPointer % 5;

	pthread_mutex_unlock(&mutex);//releases mutex lock and full semaphore
	sem_post(&full);

	return 0; 

}

int remove_item(buffer_item *item){
	int try_;

	try_ = sem_trywait(&full);
	if(try_ == -1)
		return -1; 

	sem_wait(&full); // gets full semaphore

	pthread_mutex_lock(&mutex);// gets mutex lock to protect buffer
	*item = buffer[removePointer];
	buffer[removePointer++] = -1;
	removePointer = removePointer % 5;

	pthread_mutex_unlock(&mutex);//release mutex lock and empty emaphore
	sem_post(&empty);

	return 0;

}

int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <Main thread sleep time> <# of producer threads> <# of consumer threads>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	//Initialize the the locks
	printf("pthread_mutex_init: %d\n",pthread_mutex_init(&mutex, NULL));
	sem_init(&empty, 0, 5); //Initially, empty value should be larger than 0 
	sem_init(&full, 0, 0);
      printf("-------Initializing finished------------\n");
	srand(time(0));

	//Create the producer and consumer threads
	for(i = 0; i < producerThreads; i++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, producer, NULL);
		
	}

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, consumer, NULL);
	}

	//Sleep for user specified time
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);
		random = rand();

		if(insert_item(random))
			fprintf(stderr, "Error Producing \n");
		else
		printf("Producer produced %d \n", random);

	}

}

void *consumer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);

		if(remove_item(&random))
			fprintf(stderr, "Error Consuming \n");
		else
			printf("Consumer consumed %d \n", random);
	}
}
