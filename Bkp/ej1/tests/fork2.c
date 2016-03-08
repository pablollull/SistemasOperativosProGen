#include   <stdio.h>
#include   <stdlib.h>
#include   <sys/types.h>
#include   <sys/wait.h>
#include   <unistd.h>


int main (int argc, char *argv[])
{

   printf("Soy: %d\n", (int) getpid());

   pid_t pid = fork();
   printf("fork returned %d\n", pid);

   if (pid < 0)
   {
      perror ("Fork Failed");
      exit(EXIT_FAILURE);
   }

   if (pid == 0)
   {
      printf("I am the child with pid %d\n",(int)getpid());
      sleep(5);
      printf("Child exiting...\n");
      exit(0);
   }

   //We must to be the parent
   printf("I am the parent, waiting for child to end ");
   //we wait until the child end
   wait(NULL);
   printf("Parent ending\n");


}

