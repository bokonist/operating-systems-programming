//two way pipe

#include <stdio.h> 
#include <fcntl.h> //for the files
#include <string.h> //for strlen()
#include <unistd.h> //for pipe() and fork()
#include <sys/types.h> //for pid_t
int main()
{

	int pfd1[2],pfd2[2]; // the pipe file descriptors
	int x1,x2;
	pid_t pid;

	char msg1[]="From parent : Hey! This is your father!!";
	char msg2[]="From child : Hey! dad!!"; 
	char buf[200];
	
	x1=strlen(msg1);
	x2=strlen(msg2);

	if(pipe(pfd1))
	{
		printf ("Pipe 1 failed.\n"); //print error message on standard output
		return 1;
	}
	if(pipe(pfd2))
	{
		printf ("Pipe 1 failed.\n"); //print error message on standard output
		return 1;
	}
	pid=fork();	// now we have two identical processes, pid is 0 for the child, >=1 for the parent. parent runs first
	
	if(pid==0) //child branch (this executes second)
	{
		close(pfd1[1]);
		read(pfd1[0],buf,x1); 
		printf("Child recieved : %s\n",buf);
	}
	else if(pid>0) // parent branch (this executes first)
	{
		close(pfd1[0]); // close read end
		write(pfd1[1],msg1,x1); //write the parent's message to pipe 1
	}
	if(pid==0)//child branch (this executes third)
	{
		close(pfd2[0]); //close read end of pipe 2
		write(pfd2[1],msg2,x2); //write a message from child to parent on pipe 2
	}
	else if(pid>0) //parent branch (this executes fourth)
	{
		close(pfd2[1]); //close write end of pipe 2
		read(pfd2[0],buf,x2); // read x2 bytes from the read end to buffer
		printf("Parent recieved : %s\n",buf); // print what the parent just received
	}
	return 0;
}

