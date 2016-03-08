#include   <stdio.h>
#include   <stdlib.h>
#include   <sys/types.h>
#include   <sys/wait.h>
#include   <unistd.h>


int main (int argc, char *argv[])
{

   pid_t pid;
   int i=1;
   for (i; i <=2; i++)
   {
      
      pid=fork();

      if(pid==0){
      printf("Soy Hijo - Mi variable es %d  - ",i);
      printf("%d: My PID = %d\n",i,(int)getpid() );

      }
      else
      {
       printf("Soy Papa ");
       printf("%d: My PID = %d\n",i,(int)getpid() );  
      }

   }

 sleep(1);
 printf("My PID = %d\n",(int)getpid());

}






//ps -a    ->Muestra todos los processos que hice correr yo
//ps aux   ->Muestra los procesos y sus estado
//pstree -p   ->Muestra el arbol