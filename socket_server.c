#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main( int argc, char *argv[] )
{
    int serverSock;
    int clientSock;
    int clientLen;
    struct sockaddr_in serverAddr, clientAddr;
    char buff[256];
    int reuseflag=1;

    serverSock=socket(AF_INET,SOCK_STREAM,0);
    if(serverSock<0)
    {
        perror("sock error:");
        return -1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddr.sin_port=htons(9752);

    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,(char*)&reuseflag,sizeof(reuseflag));
    if( bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr))<0 )
    {
        perror("bind error:");
        return -1;
    }

    if( listen(serverSock,5)<0 )
    {
        perror("listen error:");
        return -1;
    }

    //if multi client 
    //while(1)
    clientLen=sizeof(clientAddr);
    clientSock=accept(serverSock, (struct sockaddr*)&clientAddr, &clientLen);

    memset(buff,0,sizeof(buff));
    if(read(clientSock,buff,sizeof(buff))<=0)
    {
        perror("read error:");
    }
    else
    {
        printf("%s\n",buff);
        write(clientSock,buff,sizeof(buff));
    }

    close(serverSock);
    close(clientSock);

    return 0;
}
