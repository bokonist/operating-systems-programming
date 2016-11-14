#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#define SHMSZ 10 //shared memory size
int main()
{
	//Declare semaphore variables
	sem_t *full; 
	sem_t *empty;
	sem_t *mutex;

	char c;
	int shmid,count=0;
	int i;
	char *shm, *s;

	//Remove semaphores if they already exist
	sem_unlink("/empty");
	sem_unlink("/fuller");
	sem_unlink("/mutex");

	//Create semaphore of required name
	//sem_open - initialize and open a named semaphore
	//sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
	//On success, sem_open() returns the address of the new semaphore;

	empty=sem_open("/empty",O_CREAT | O_EXCL, 0644, SHMSZ);
	full=sem_open("/fuller",O_CREAT | O_EXCL,0644, 0);
	mutex=sem_open("/mutex",O_CREAT, 0644, 1);

	//Get shared memory id
	//shmget - allocates a System V shared memory segment
	//int shmget(key_t key, size_t size, int shmflg);
	if((shmid=shmget(1234,SHMSZ,IPC_CREAT|0666))<0)
	{
		perror("shmget error");
		exit(1);
	}
	
	//Attach shared memory
	//shmat() attaches the shared memory segment identified by shmid to the address space of the calling process.
	//void *shmat(int shmid, const void *shmaddr,int shmflg);
	//If shmaddr is NULL, the system chooses a suitable (unused) address at which to attach the segment. 
	shm=shmat(shmid, NULL, 0);
	if(*shm == -1){
		perror("shmat error");
		exit(1);
	}
	
	s=shm;
	printf("Enter characters one by one (produce) : ");
	//Producer
	while(1)
	{
		sem_wait(empty);//wait if semaphore empty is 0
		/*	sem_wait() decrements (locks) the semaphore pointed to by sem.  If the semaphore
       		currently has the value zero, then the call blocks until it
			becomes possible to perform the decrement	*/
		sem_wait(mutex);//Wait if semaphore mutex is 0
		scanf("%c",&c); //produce character by character
		shm[count]=c; //Load to shared memory
		count=(count+1)%SHMSZ;
		sem_post(mutex);//Increment mutex
		/*	sem_post() increments (unlocks) the semaphore pointed to by sem.  If
			the semaphore's value consequently becomes greater than zero, then
			another process or thread blocked in a sem_wait() call will be woken
			up and proceed to lock the semaphore.					*/
		sem_post(full);//Increment full
		
	}
	return 0;
}
