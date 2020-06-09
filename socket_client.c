#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main( int argc, char *argv[] )
{
    int sock;
    struct sockaddr_in addr;
    char buff[256];
    struct timeval tv;

    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    addr.sin_port=htons(9752);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    //for timeout
    //setsockipt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));
    
    sock=socket(AF_INET, SOCK_STREAM,0);
    if(connect(sock,(struct sockaddr*)&addr, sizeof(addr))<0)
    {
        perror("connect:");
        return -1;
    }

    memset(buff, 0, sizeof(buff));
    strcpy(buff, "Hello");
    write(sock, buff, sizeof(buff));
    if( read(sock, buff, sizeof(buff))<0 )
    {
        perror("read:");
    }
    else
    {
        printf("%s\n",buff);
    }
    close(sock);
}
