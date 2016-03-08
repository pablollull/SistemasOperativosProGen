
#include   <stdio.h>
#include   <stdlib.h>

int main(int argc,char *argv[])
{

      //argc - argv explanation
      // argc refers to the number of command line 
      // arguments passed in, which includes the 
      // actual name of the program, as invoked by the user. 
      // argv contains the actual arguments, starting with 
      // index 1. Index 0 is the program name.
 

    if(argc!=3)/*3 because even the executables name string is on argc*/
    {
    printf("unexpected number of arguments\n");
    return -1;
    }

    printf("you entered in reverse order:\n");

    int a=atoi(argv[1]);
    int b=atoi(argv[2]);

        printf("%d\n",a);
        printf("%d\n",b);
    

return 0;
}