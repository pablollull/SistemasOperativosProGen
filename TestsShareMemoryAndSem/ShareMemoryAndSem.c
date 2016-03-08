
/*************************************************************************//**

  @file     ej1_fork.c
  @brief    EJERCICIO 1 - Fork
  @author   Marcos Darino (MD)

 ******************************************************************************/


/*==================[inclusions]=============================================*/
#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <unistd.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <string.h>
#include  <sys/types.h>



/*==================[macros and definitions]=================================*/

#define SHM_SIZE    1024
#define KEY_NUMBER  9876

  typedef struct  STR_Shm
{
  key_t     key;    //to store the key
  size_t    size;   //to store the size of the shm
  char      *shm;    //to store the pointer to the memory
  int       shmid;  //to save the return identify
}shm_t;


/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


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





int main (int argc, char *argv[])
{
   
   ///Start message
   printf("Startling...\n");

   //--Take the main arguments--//¡

   //--First check the quantity
    if(argc!=3)/*3 because even the executables name string is on argc*/
    {
      printf("unexpected number of arguments\n");
      return (-1);
    }

    //copy the arguments
   int numChild = atoi (argv[1]);
   int timeInLive = atoi (argv[2]);

   //Create the childs
   int i=0;
   pid_t pid;

    
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
   

   for (i; i < numChild; i++)
   {
         
     pid=fork();
     //printf("fork returned %d\n", pid);

     //check if there is an error
     if (pid < 0)
     {
        perror ("Fork Failed \n");
        exit(EXIT_FAILURE);
     }

     //Child
     if (pid == 0)
     {
          printf("%d: I am the child with pid %d \n",i,(int)getpid());
          //sleep(timeInLive);
          //CHILD Work
          int seconds=1;
          #define BUFFER_SIZE 40
          char buffer[BUFFER_SIZE];
          for (seconds = 1; seconds <= timeInLive; seconds++)
          {
            sleep(1);
            printf("CHILD %d - pid:%d: timer: %d sec\n",i,(int)getpid(),seconds);
            snprintf(buffer,BUFFER_SIZE,"CHILD %d - pid:%d: timer: %d sec\n",i,(int)getpid(),seconds);
            strncat(shmPointer,buffer,BUFFER_SIZE);
          }
        
        printf("Child %d exiting...\n",i);
        exit(0);
     }

     printf("I am the parent %d creating a child:%d\n",(int)getpid(),i);
     //sleep(1);
    
   }


  //We must to be the parent
   printf("--I am the parent, waiting for child to end \n");
   //We wait until the child end to aboid zombis
   i=0;
   for (i; i < numChild; i++)
   {
    wait(NULL);
   }
   printf("--Parent ending \n");

   printf("LOG:\n");
   printf("%s\n",shmPointer);

   //Delete the share memory
   shmDelete(&shMem);

}










