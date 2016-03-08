
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
   int i=0; //for the loops

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

   //--CHILDS--//

   //First parent message
   printf("Parent: I am the PID=%d \n",(int)getpid());

   //loop to create the childs
   for (i; i < numChild; i++)
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
        free(childPointerOrigin);  //free the memory in the child
        printf("%d: ---Child: I wake up, my pid:%d \n",i,(int)getpid());
        sleep(timeInLive); 
        printf("---Child: %d exiting...\n",i);
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


  //--Free my memory --//

  printf("Parent: free memory \n"); 
  free(childPointerOrigin);  //free the parent memory
  printf("Parent: ending \n"); //goodbye
}










