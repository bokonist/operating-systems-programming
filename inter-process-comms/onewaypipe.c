/*create a file "output.txt" for this program to work*/

#include <sys/types.h> // for pid_t
#include <unistd.h> // unix standard. for pipe() and fork()
#include <stdio.h> // fileno(), fdopen()
//#include <stdlib.h>
/* Read characters from the pipe and put in file in parent process, read the file from the child process. */
void read_from_pipe (int file) // pass the file descriptor of the file
{
		FILE *stream; //open a stream
		int c;
		stream = fdopen (file, "r"); //open file to read
		printf("the contents of the file written by parent is :\n");
		while ((c = fgetc (stream)) != EOF) // as long as it's not EOF
			putchar (c); //print the character on screen. pass the ASCII value of the char
		fclose (stream);
}

/* Write some random text to the pipe. */

void write_to_pipe (int file)
{
		FILE *stream; //open stream
		stream = fdopen (file, "w"); // open file to write (stdout)
		fprintf (stream, "hello, world!\n"); // write text
		fprintf (stream, "goodbye, world!\n"); //write text
		fclose (stream); //close stream
}

int main()
{
		pid_t pid; // to hold the unsigned integer process ID
		int mypipe[2]; // integer file descriptors of the write and read end of the pipe
		FILE *fi, *fo;
		fi= fdopen(fileno( fopen("output.txt", "r")) , "r");
		fo= fdopen(fileno( fopen("output.txt", "w")) , "w");
	 	/* Create the pipe. */
		if (pipe (mypipe)) // returns 1 if failed. mypipe[0] is read end, [1] is write end
		{
				printf ("Pipe failed.\n"); //print error message on standard output
				return 1;
		}
		/* Create the child process. */
		pid = fork (); // now we have two identical processes, pid is 0 for the child, >=1 for the parent.
		/* The statements after fork() runs once for each process spawned, including the parent. parent runs first */
		if (pid == (pid_t) 0 ) // 0 is success, denotes the child, >=1 denotes parent
		{
				printf("fork() returned %d (is child)\n", pid);
			 	/* This is the child process.
					Close other end first. */
				close (mypipe[1]); // close the write end
				//mypipe[0]= fileno(fi); // // get file descriptor from file stream
				read_from_pipe (mypipe[0]); // pass the read end
				printf("Successful Read from the piped input \n");
				fclose(fo);
				return 0;
		}
		else if (pid < (pid_t) 0) // -ve value is fail
		{
				printf("fork() returned %d (is fail)\n", pid);
			 	/* The fork failed. */
				printf ("Fork failed.\n");
				return 1;
		}
		else //parent
		{
				printf("fork() returned %d (is parent)\n", pid);
				 /* This is the parent process.
					Close other end first. */
			 	close (mypipe[0]); // close the read end
			 	//mypipe[1]= fileno(fo); // get file descriptor from file stream
			 	write_to_pipe (mypipe[1]); // pass the write end. output.txt
			 	printf("Successful Write to output.txt which is piped to the child.\n");
			 	fclose(fo);
			 	return 0;
		}
}