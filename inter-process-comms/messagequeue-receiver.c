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
    int msgflg= 0666;
    key_t key;
    struct msgbuf rbuf;
    char str[100], temp;
    int i, j = 0;
    key = 1234; //same unique key that is used to send

    if ((msqid = msgget(key, 0666)) < 0) //get the m-queue ID with this key
      die("msgget()");

    //Receive an answer of message type 1.
    // ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
    if (msgrcv(msqid, &rbuf, MAXSIZE, 1, 0) < 0) //put flag IPC_NOWAIT to fail if no space is available. 0 will wait till space is available
      die("msgrcv");
    printf("Message received from message queue is : ");
    printf("%s\n", rbuf.mtext);
    exit(0);
}
