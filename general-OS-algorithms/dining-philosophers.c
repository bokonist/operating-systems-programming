//dining philosophers problem for 5 philosophers.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *func(int n); //the start routine for each thread

pthread_t philosopher[5]; //a thread for each philosopher
pthread_mutex_t chopstick[5]; // mutually exclusive variables for 5 chopsticks

void *func(int n) //function to find who is eating and who is thinking
{
  	printf("Philosopher %d is thinking \n",n); //n is thinking
  	pthread_mutex_lock(&chopstick[n]); // lock the right chopstick
 	pthread_mutex_lock(&chopstick[(n+1)%5]); //lock the left chopstick
  	printf("Philosopher %d is eating now\n",n);
  	sleep(1); //eat period
  	pthread_mutex_unlock(&chopstick[n]); // unlock right chopstick
  	pthread_mutex_unlock(&chopstick[(n+1)%5]); //unlock left chopstick
  	printf("Philosopher %d finished eating \n",n); //done
}

int main()
{
	int i,k;
	void *msg;

	for(i=1;i<=5;i++)
	{
		k=pthread_mutex_init(&chopstick[i],NULL); //mutex variable initialization
		if(k==-1)
		{
			printf("Mutex initialization failed\n");
			exit(1);
		}
	}
	
	for(i=1;i<=5;i++)
	{
		k=pthread_create(&philosopher[i],NULL,(void *)func,(int *)i); //creating new thread
		if(k!=0)
		{
			printf("pthread_create error \n");
			exit(1);
		}
	}

	for(i=1;i<=5;i++)
	{
		k=pthread_join(philosopher[i],&msg); //thread joining. params are thread and the **retval which is passed onto next thread
		if(k!=0)
 		{
			printf("\n pthread_join failed \n");
			exit(1);
		}
	}
	printf("Everyone has finished eating\n");
	for(i=1;i<=5;i++)
	{
		k=pthread_mutex_destroy(&chopstick[i]);  //destroy the mutex
		if(k==0)
		{
			printf("Mutex of chopstick %d Destroyed \n",i);
		}
	}
	return 0;
}