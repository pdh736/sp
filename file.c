#include <stdio.h>

int main( int argc, char *argv[])
{
    FILE *fp;
    char buff[64];

    fp = fopen("test.txt","r+");
    if(!fp)
    {
        printf("file open fail\n");
        return -1;
    }
   
   //read untill lf 
    if(fgets(buff, sizeof(buff), fp)!=NULL)
    {
        printf("%s\n",buff);
    }
    //끊어서 서식 문자 별로 읽고 싶은경우 fscanf 사용
    
    fclose(fp);
    return 0;
}
