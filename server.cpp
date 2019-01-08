/*
	Author: ELizabeth Ralyea
	Date: 2/16/18
	This program is based on my understanding out the program given by Dr. Doman and previous knowledge.

*/
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _MAILBOX 1200    
#define NUM_PROCESSES 4         

struct {
long priority;         
int temp;             
int pid;              
int stable;           
} msgp, cmbox;

//MAIN function
int main(int argc, char *argv[]) {

   
   if(argc != 2) {
      printf("USAGE: Too few arguments, needs Temp");
      exit(0);
   }

   printf("\nStarting Server...\n");

   
   int i,result,length,status;             
   int uid = 0;                            
   int initTemp = atoi(argv[1]);        
   int msqid[NUM_PROCESSES];       
   int unstable = 1;          
   int tempAry[NUM_PROCESSES];
   float cycles = 1;
	
   int msqidC = msgget(_MAILBOX, 0600 | IPC_CREAT);

   for(i = 1; i <= NUM_PROCESSES; i++){
      msqid[(i-1)] = msgget((_MAILBOX + i), 0600 | IPC_CREAT);
   }

   //Initialize the message to be sent
   msgp.priority = 1;
   msgp.pid = uid;
   msgp.temp = initTemp;
   msgp.stable = 1;

   length = sizeof(msgp) - sizeof(long);

   //While the processes have different temperatures
   while(unstable == 1){
      int sumTemp = 0;        
      int stable = 1;           

      // Get new messages from the processes
      for(i = 0; i < NUM_PROCESSES; i++){
         result = msgrcv( msqidC, &cmbox, length, 1, 0);

        
         if(tempAry[(cmbox.pid - 1)] != cmbox.temp) {
            stable = 0;
            tempAry[(cmbox.pid - 1)] = cmbox.temp;
         }

	 printf("Recieved New Temp: %d from Process %d \n", cmbox.temp, cmbox.pid);

         //Add up all the temps as we go for the temperature algorithm
         sumTemp += cmbox.temp;
      }

    
      if(stable){
         printf("Temperature Stabilized: %d\n", msgp.temp);
	 printf("It took %f cycles for the temperature to stabilize. \n", cycles);
         unstable = 0;
         msgp.stable = 0;
      }
      else { //Calc a new temp and set the temp field in the message
         int newTemp = (2 * msgp.temp + sumTemp) / 6;
         msgp.temp = newTemp;
	 cycles += 1;
      }

      for(i = 0; i < NUM_PROCESSES; i++){
         result = msgsnd( msqid[i], &msgp, length, 0);
      }
   }

   printf("\nShutting down Server...\n");

   //Remove the mailbox
   status = msgctl(msqidC, IPC_RMID, 0);

   //Validate nothing when wrong when trying to remove mailbox
   if(status != 0){
      printf("\nERROR closing mailbox\n");
   }

}
