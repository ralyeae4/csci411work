/*
	Author: ELizabeth Ralyea
	Date: 2/26/18
	This program is based on my understanding out the program given by Dr. Doman and previous knowledge.

*/
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _MAILBOX 1200    

struct {
long priority;        //message priority
int temp;            //temperature
int pid;               //process id
int stable;          //boolean for temperature stability
} msgp, cmbox;

//MAIN function
int main(int argc, char *argv[]) {

  
   if(argc != 3) {
      printf("USAGE: Too few arguments, needs Temp UID");
      exit(0);
   }

   int unstable = 1;
   int result, length, status;
   int initTemp = atoi(argv[1]);
   int uid = atoi(argv[2]);

   
   int msqidC = msgget(_MAILBOX, 0600 | IPC_CREAT);

   
   int msqid = msgget((_MAILBOX + uid), 0600 | IPC_CREAT);

   //Initialize the message to be sent
   cmbox.priority = 1;
   cmbox.pid = uid;
   cmbox.temp = initTemp;
   cmbox.stable = 1;

   length = sizeof(msgp) - sizeof(long);

   //While all the processes have different temps
   while(unstable == 1){
      
      result = msgsnd( msqidC, &cmbox, length, 0);

      
      result = msgrcv( msqid, &msgp, length, 1, 0);

      //If the new message indicates all the processes have the same temp
      //break the loop and print out the final temperature
      if(msgp.stable == 0) {
         unstable = 0;
         printf("\nProcess %d Temp: %d\n", cmbox.pid, cmbox.temp);
      }
      else { 
         int newTemp = (cmbox.temp * 3 + 2 * msgp.temp) / 5;
         cmbox.temp = newTemp;
	 printf("\nProcess %d Temp: %d\n", cmbox.pid, cmbox.temp);
      }
   }

   //Remove the mailbox
   status = msgctl(msqid, IPC_RMID, 0);

   //Validate nothing when wrong when trying to remove mailbox
   if(status != 0){
      printf("\nERROR closing mailbox\n");
   }
}
