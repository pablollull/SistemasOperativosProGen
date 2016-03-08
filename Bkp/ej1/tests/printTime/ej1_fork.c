
/*************************************************************************//**

  @file     ej1_fork.c
  @brief    EJERCICIO 1 - Fork
  @author   Marcos Darino (MD)

 ******************************************************************************/


/*==================[inclusions]=============================================*/
#include   <stdio.h>
#include   <stdlib.h>
#include   <sys/types.h>
#include   <sys/wait.h>
#include   <unistd.h>
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


void childWork (int lifeTime)
{
  int seconds=1;
  for (seconds = 1; seconds <= lifeTime; seconds++)
  {
    sleep(1);
    printf("I am alive: %d\n",seconds);
  }

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
      return -1;
    }

   int numChild = atoi (argv[1]);
   int timeInLive = atoi (argv[2]);

   // //welcome Parent message 
   // printf("I am the parent, my PID is %d \n", (int)getpid());
   // printf("--I will create childs \n");

   //Create the childs

   int i=0;
   pid_t pid;

   char commandToEnd[]="end";


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
        childWork(timeInLive);
        printf("Child %d exiting...\n",i);
        exit(0);
     }

     printf("I am the parent %d creating a child:%d\n",(int)getpid(),i);
     //sleep(1);
    
   }

  //sleep(5);  


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










