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
    int pipefd[2];//[0]읽기전용 [1]쓰기전용

    if(pipe(pipefd) < 0){
        printf("pipe open error\n");
        return -1;
    }

    pid = fork();//return if child 0, if parent pid of child
    if(pid < 0){
        printf("fork error\n");
        return -1;
    }
    if(pid == 0){//0일때 자식프로세스
        char buff[10];
        int ret;
        printf("run child : %d\n", getpid());
        dup2(pipefd[0], 0);//2번 파라미터를 1번 파라미터로 바꿈
        close(pipefd[1]);//쓰기전용 파이프 닫음
        ret = execl("./test","",NULL);//arg 넣는대로 파라미터로 전달 NULL로 arg 끝 표현
        if(ret<0){
            printf("exe error\n");
        }
        /*
        for(int i=0;i<10;i++){
            scanf("%s",buff);
            printf("parent said : %s",buff);
        }
        */
    }
    else{//부모일때 자식프로세스 pid 값 들어감
        printf("run parent : %d\n",getpid());
        close(pipefd[0]);//읽기전용 파이프 닫음
        for(int i=0;i<10;i++){
            write(pipefd[1],"hello\n",6);
        }
        //kill(pid,SIGKILL);
        wait(&status);
    }
    return 0;
}
