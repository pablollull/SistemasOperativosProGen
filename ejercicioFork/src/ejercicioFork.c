
/*************************************************************************//**

  @file     ej1_fork.c
  @brief    EJERCICIO 1 - Fork
  @author   Marcos Darino (MD)

 ******************************************************************************/


/*==================[inclusions]=============================================*/
#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <unistd.h>
#include    <sys/ipc.h>
#include    <sys/shm.h>
#include    <sys/sem.h>
#include    <string.h>
#include    <sys/types.h>
#include    <errno.h>

/*==================[macros and definitions]=================================*/

#define SHM_SIZE    32768
#define KEY_NUMBER  9876

  typedef struct  STR_Shm
{
  key_t     key;    //to store the key
  size_t    size;   //to store the size of the shm
  char      *shm;    //to store the pointer to the memory
  int       shmid;  //to save the return identify
}shm_t;

  typedef struct  STR_Sem
{
  key_t     key;    //to store the key
  int       number;   //to store the number of the semaphores
  int       semid;  //to save the return identify
}semphore_t;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/**
 * @brief Create the semaphore
 * @param number   How many semaphore
 * @param key      
 * @param sem   Struct of the semaphore   
 */

void semCreate(semphore_t *sem, key_t key, int number)
{
   sem->key=key;
   sem->number=number;
   sem->semid = semget(sem->key,sem->number, IPC_CREAT | 0666);
   printf("Allocating the semaphore: %s\n",strerror(errno));
   int semval = 1;

   semctl(sem->semid,0,SETVAL,semval);
   printf("Setting semaphore value to %d: %s\n",semval,strerror(errno));

   semval = semctl(sem->semid,0,GETVAL);
   printf("Initialized Semaphore value to %d: %s\n",semval,strerror(errno));
}

void semWait(semphore_t *sem)
{
   static struct sembuf wait;

   wait.sem_num = 0;
   wait.sem_op = 0;
   wait.sem_flg = SEM_UNDO;

   semop(sem->semid,&wait,1);
}

void semInc(semphore_t *sem)
{
   static struct sembuf inc;

   inc.sem_num = 0;
   inc.sem_op = 1;
   inc.sem_flg = SEM_UNDO; 

   semop(sem->semid,&inc,1);
}

void semDec(semphore_t *sem)
{
   static struct sembuf dec;
   //decr
   dec.sem_num = 0;
   dec.sem_op = -1;
   dec.sem_flg = SEM_UNDO; 

   semop(sem->semid,&dec,1);
}


void semDelete(semphore_t *sem)
{
   semctl(sem->semid,0,IPC_RMID);
}
 


/**
 * @brief Create the share memory
 * @param shMemory   Struct of the share memory
 * @param key      
 * @param size   
 */

void *shmCreate(shm_t *shMemory, key_t key, size_t size)
{
  shMemory->key=key;
  shMemory->size=size;
  //Create the share memory
  shMemory->shmid = shmget (key,size, IPC_CREAT | 0666);
  if (shMemory->shmid<0)
   {
      perror("Error when try to create the share memory");
      return (int *)(-1);
   }
   //Attach the memory to a pointer
   shMemory->shm = shmat (shMemory->shmid, NULL,0);
   if (shMemory->shm == (char *) -1)
   {
      perror("Error when try to do shmat");
      return (int *)(-1);  
   }

   return (int *)(shMemory->shm);
}


/**
 * @brief Delete the share memory
 * @param shMemory   Struct of the share memory
 */

void shmDelete(shm_t *shMemory)
{
   shmdt(shMemory->shm); //clean the attach
   shmctl(shMemory->shmid, IPC_RMID,NULL); //clean the memory
}




/*==================[external functions definition]==========================*/



/*==============================[MAIN]=======================================*/


int main (int argc, char *argv[])
{
   
   ///Start message
   printf("--------WELCOME--------\n");

   
   //--Take the main arguments--//

   //--First check the quantity
    if(argc!=3)/*3 because even the executables name string is on argc*/
    {
      printf("unexpected number of arguments\n");
      return -1;
    }

   int numChild = atoi (argv[1]);
   int timeInLive = atoi (argv[2]);

   //--pid and wait variables--//

   pid_t pid;  //store the PID
   pid_t waitPIDnumber; //store the wait() return
   int i; //for the loops

   //Count of Child die - for the end loop
   int countChildDie=0;

   //Pointer to save all the child PID
   int *childPointer;
   //Pointer to store the origin of the PID pointer
   int *childPointerOrigin;
   //Auxiliary variable to store the PID 
   int pidCheckAux;
   //Status of the wait pid
   int status;

   //Allocate the bytes for save the child PIDs
   childPointer=(int *) malloc(numChild*sizeof(int));
   //Copy the origin, to free the memory in the child 
   childPointerOrigin=childPointer; 

   ///-----------------SEMAPHORE----------------///
   semphore_t  sem;
   
   semCreate(&sem,KEY_NUMBER,1);
   
   ///-----------------MEMORY----------------///
   //Create the struct of the memory
   shm_t shMem;
   //Pointer to the key memory
   char *shmPointer;    
   shmPointer=shmCreate (&shMem,KEY_NUMBER, SHM_SIZE);
   if (shmPointer==(char *) -1)
   {
     perror("Error with the key memory");
     exit(1);
   }


   //--CHILDS--//

   //First parent message
   printf("Parent: I am the PID=%d \n",(int)getpid());

   //loop to create the childs
   for (i=0; i < numChild; i++)
   {
         
     pid=fork();
     
     //check if there is an error
     if (pid < 0)
     {
        perror ("Fork Failed \n");
        exit(EXIT_FAILURE);
     }

     //Child
     if (pid == 0)
     {
          
          //sleep(2);
          printf("%d: I am the child with pid %d \n",i,(int)getpid());
          //sleep(timeInLive);
          //CHILD Work
          int seconds=1;
          #define BUFFER_SIZE 40
          char buffer[BUFFER_SIZE];
          for (seconds = 1; seconds <= timeInLive; seconds++)
          {
            
            printf("CHILD %d - pid:%d: timer: %d sec\n",i,(int)getpid(),seconds);
            snprintf(buffer,BUFFER_SIZE,"CHILD %d - pid:%d: timer: %d sec\n",i,(int)getpid(),seconds);
            semDec(&sem); //decrease 1 - release the semaphore
            strncat(shmPointer,buffer,BUFFER_SIZE);
            semInc(&sem); //increase 1 - take the semaphore
            sleep(1);
          }
        
        printf("Child %d exiting...\n",i);
        exit(0);
     }

     printf("Parent: Creating a child: PID=%d Number:%d\n",pid,i);
     //Save the PID of the child create
     *childPointer=pid; 
     //Increment the pointer
     childPointer++;

   }

  //--CHECK THE CHILDS STATES--//

  
  printf("Parent: Waiting my childs\n");

  while(countChildDie<numChild)
  {
    //wait function - block the program here until a signal
    waitPIDnumber = waitpid(-1,&status,0);
    //check if there is a error
    if (waitPIDnumber == -1) 
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    
    //Check if some child die by signal
    if( WIFSIGNALED(status))
      printf("%d - die by signal \n",WTERMSIG(status));  //print the number
    //if ( WIFEXITED(status))
      //printf("die in natural situation \n");

    //reset the pointer to the begin
    childPointer=childPointerOrigin;
    //loop fir all the pid store numbers
    for (i=0;i<numChild;i++)
    {
      pidCheckAux=*childPointer;  //strore the value
      childPointer++; //increase the pointer
      if(waitPIDnumber==pidCheckAux) //if equal add one 
      {
        printf("Parent: The child: %d RIP \n",pidCheckAux);
        countChildDie++; //one child die
      }
    }
  }

   printf("LOG:\n");
   printf("%s\n",shmPointer);


  //--Free the memory --//

  printf("Parent: free memory \n"); 
  free(childPointerOrigin);  //free the parent memory
  shmDelete(&shMem);//Delete the share memory
  semDelete(&sem); //Delete the semaphore
  printf("Parent: ending \n"); //goodbye

  return (0);

}










