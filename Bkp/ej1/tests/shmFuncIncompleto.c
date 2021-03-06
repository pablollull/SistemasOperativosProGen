
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

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


//size: size of the memory
//key: the key
//return: The addres - if there are and error return -1

void *createKeyMemory (key_t key, size_t size)
{
   char *shm;  //to store the pointer to the memory
   int shmid;  //to save the return identify 
   //Create the share memory
   shmid = shmget (key,size, IPC_CREAT | 0666);
   //check if it is create well
   if (shmid<0)
   {
      perror("Error when try to create the share memory");
      return (int *)(-1);
   }
   //Attach the memory to a pointer
   shm = shmat (shmid, NULL,0);
   if (shm == (char *) -1)
   {
      perror("Error when try to do shmat");
      return (int *)(-1);  
   }

   return (int *)(shm);

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

   int numChild = atoi (argv[1]);
   int timeInLive = atoi (argv[2]);

   // //welcome Parent message 
   // printf("I am the parent, my PID is %d \n", (int)getpid());
   // printf("--I will create childs \n");


   //Create the childs
   int i=0;
   pid_t pid;

    
   ///-----------------MEMORY----------------///
    //Pointer to the key memory
   char *shmPointer;    
   shmPointer=createKeyMemory (KEY_NUMBER, SHM_SIZE);
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
          for (seconds = 1; seconds <= timeInLive; seconds++)
          {
            sleep(1);
            printf("I am alive: %d seconds\n",seconds);

          }
        memcpy (shmPointer,"I am the child from key1memory",30);
        *shmPointer='e';
        *(shmPointer+1)='n';
        *(shmPointer+2)='d';
        memcpy (shmPointer,"I am the child from key2memory",30);
        *shmPointer='e';
        *(shmPointer+1)='n';
        *(shmPointer+2)='d';

        printf("Child %d exiting...\n",i);
        exit(0);
     }

     printf("I am the parent %d creating a child:%d\n",(int)getpid(),i);
     //sleep(1);
    
   }

  sleep(2); //wait two second, only to debug easily  


  //The goodbye of the child - print the info in the memory
     char *s;
     s=shmPointer;
     int it=1;
   //while(*s!='e' && *(s+1)!='n' && *(s+2)!='d')
   while (it<2)
   { 
     it++;
     for (s; *s != '\0'; s++)
     printf("%c",*s );
    printf("\n");
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
  

  //sleep(5);

}










