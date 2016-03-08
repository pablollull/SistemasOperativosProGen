#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024
#define KEY_NUMBER  9876

int main (int argc, char *argv[])
{
   int shmid;
   key_t key;
   char *shm;
   char *s;

   //Assignment the key
   key= KEY_NUMBER;
   //Create the share memory
   shmid = shmget (key,SHM_SIZE, IPC_CREAT | 0666);
   //check if it is create well
   if (shmid<0)
   {
      perror("Error when try to create the share memory");
      exit(1);
   }

   shm = shmat (shmid, NULL,0);

   if (shm == (char *) -1)
   {
      perror("Error when try to do shmat");
      exit(1);      
   }

   //copy to the memory
   memcpy (shm,"Hello World",11);

   s= shm;
   s+=11;
   *s=0;

   while (*shm != '*')
      sleep(1);

   return 0;




}


