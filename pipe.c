#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;
	//parent to child
    int pipefd[2];//[0]read [1]write
	//child to parent
    int pipefd2[2];//[0]read [1]write

    if(pipe(pipefd) < 0){
        printf("pipe open error\n");
        return -1;
    }
    if(pipe(pipefd2) < 0){
        printf("pipe open error\n");
        return -1;
    }

    pid = fork();//return if child 0, if parent pid of child
    if(pid < 0){
        printf("fork error\n");
        return -1;
    }
    if(pid == 0){
        int ret;
        //printf("run child : %d\n", getpid());
        dup2(pipefd[0], 0);
        close(pipefd[1]);

        dup2(pipefd2[1], 1);
        close(pipefd2[0]);

        ret = execl("./test","./test", NULL);
        if(ret<0){
            printf("exe error\n");
        }
    }
    else{
        //printf("run parent : %d\n",getpid());
        close(pipefd[0]);
        close(pipefd2[1]);

        char buf[20] = {0,};
        int ret;
        ret = read(pipefd2[0], buf, 20);
        printf("child send : %s\n", buf);

        write(pipefd[1], "hello\n", 6);
    }
    
    return 0;
}

//////////////////////////////////
//test tool
/*
    char buf[20] ={0,};

    printf("%s\n", "abc");
    fflush(stdout);
    FILE* fp = fopen("./abc", "wb");

    scanf("%s",  buf);
    fprintf(fp , "%s", buf);

    fclose(fp);
    return 0;
*/
