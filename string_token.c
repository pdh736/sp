#include <stdio.h>
#include <string.h>

int string_token(const char *src, char (*bus)[10], int *dist, int *cnt){
    char str[128];
    char *temp;

    strcpy(str,src);
    temp=strtok(str,"#");
    while(temp!=NULL){
        sscanf(temp, "%5s,%5d",bus[*cnt], &dist[*cnt]);
        temp=strtok(NULL,"#");
        *cnt=*cnt+1;
    }
    return 0;
}

int main( int argc, char *argv[] ){
    char *src="10:00:03#BUS01,02395#BUS02,00815#BUS03,07854\n";
    char new[128];
    int h,m,s;

    sscanf(src, "%2d:%2d:%2d",&h,&m,&s);
    memset(new, 0, 128);
    strcpy(new, src+8);
    new[strlen(new)-1]='\0';
    int cnt = 0;
    char bus[12][10];
    int dist[10];
    string_token(new,bus, dist, &cnt);
    
    //cnt 갯수 한개 많게 옴
    for(int i=0;i<cnt;i++){
        printf("%s#%05d\n",bus[i],dist[i]);
    }

    return 0;
}
