
#include   <stdio.h>
#include   <stdlib.h>

int main(int argc,char *argv[])
{

    if(argc!=3)/*3 because even the executables name string is on argc*/
    {
    printf("unexpected number of arguments\n");
    return -1;
    }

    printf("you entered in reverse order:\n");

    while(argc--)
    {
        printf("%s\n",argv[argc]);
    }

return 0;
}