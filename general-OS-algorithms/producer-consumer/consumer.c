#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#define SHMSZ 10
int main()
{
	//Declare semaphore variables
	sem_t * full;
	sem_t * empty;
	sem_t * mutex;

	int shmid,count=0;
	char *shm, *s,temp;

	//Create semaphore of required name if doesn't exist already
	empty=sem_open("/empty",O_CREAT, 0644, SHMSZ);
	full=sem_open("/fuller",O_CREAT, 0644, 0);
	mutex=sem_open("/mutex",O_CREAT, 0644, 1);

	//Get shared memory id
	if((shmid=shmget(1234,SHMSZ,0666))<0){
		perror("shmget");
		exit(1);
	}

	//Attach shared memory
	if((shm=shmat(shmid,NULL,0))==-1){
		perror("shmat");
		exit(1);
	}

	s=shm;
	//Consumer
	while(1)
	{
		
		sem_wait(full);//Wait if semaphore /fuller is 0
		sem_wait(mutex);//Wait if semaphore /mutex is 0
		
		temp=s[count];//Read from shared memory. i.e consume character by character
		count=(count+1)%SHMSZ;
		printf("%c",temp);
		sem_post(mutex);//Increment /mutex
		sem_post(empty);//Increment /empty
	}
	return 0;
}
