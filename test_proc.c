#include <stdio.h>

int main(int argc, char *argv[])
{
    char buff[10];
    for(int i=0;i<10;i++)
    {
        scanf("%s",buff);
        printf("test proc %s\n",buff);
    }
    return 0;
}
