#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>

#define KEY 1234

int main(int argc,char *argv[]){

  
  int semid;
  
  unsigned int semval;

  struct sembuf wait,inc,dec;

  dec.sem_num = 0;
  dec.sem_op = -1;
  dec.sem_flg = SEM_UNDO;

  wait.sem_num = 0;
  wait.sem_op = 0;
  wait.sem_flg = SEM_UNDO;

  inc.sem_num = 0;
  inc.sem_op = 1;
  inc.sem_flg = SEM_UNDO;

  semid = semget(1452,1,IPC_CREAT | 0666);
  printf("---SEND:Allocating the semaphore: %s\n",strerror(errno));

  semval = 1;
  semctl(semid,0,SETVAL,semval);
  printf("---SEND:Setting semaphore value to %d: %s\n",semval,strerror(errno));

  semval = semctl(semid,0,GETVAL);
  printf("---SEND:Initialized Semaphore value to %d: %s\n",semval,strerror(errno));


  

  while(1){

   
   printf("Press return to lock: ");
   getchar();
   printf("Trying to lock...\n");
   if (semop(semid, &dec, 1) == -1) {
     printf("No locked");
     perror("semop");
     semctl(semid,0,IPC_RMID);
     exit(1);
   }
   printf("Locked.\n");




  }

  
  semctl(semid,0,GETVAL,&semval);
  printf("---SEND: Finished with the semaphore: %d\n",semval);

  sleep(5);
  semctl(semid,0,IPC_RMID);
  printf("---SEND: Semaphore removed from the System = %s\n",strerror(errno));
  return 0;
}
