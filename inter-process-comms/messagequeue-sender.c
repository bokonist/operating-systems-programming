#include <sys/types.h> //for type variables
#include <sys/ipc.h> //for interprocess comms
#include <sys/msg.h> // for message queues
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSIZE 128

void die(char *s) // utility function to print error message and exit unsuccessfully
{
  perror(s);
  exit(1);
}

struct msgbuf // struct that holds the message
{
    long mtype; //this struct should always have a long variable as the first one.
    char mtext[MAXSIZE];
};

int main()
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    struct msgbuf sbuf;
    size_t buflen;

    key = 1234; //should be a unique key

    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for the given key
        die("msgget"); //if it fails, print error message and exit 

    //Message Type
    sbuf.mtype = 1; // make mtype as 1 to send

    printf("Enter a message to add to message queue : ");
    scanf("%[^\n]",sbuf.mtext); //scan till \n
    getchar();
    buflen = strlen(sbuf.mtext) + 1 ;
    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) 
    //send to message queue with ID = msqid, the contents of sbuf, with bufferlength = buflen and flag = IPC_NOWAIT
    //IPC_NOWAIT will make the function return immediately if no message of the requested type is in the queue.
    {
        printf ("%d, %lo , %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
        die("msgsnd");
    }

    else
        printf("Message Sent\n"); //successful msgsnd()
    exit(0);
}



