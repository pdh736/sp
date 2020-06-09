#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

//하위 디렉토리 탐색하며 Filter 함수에 있는 확장자만 가져오는 프로그램
//ref
//http://blog.naver.com/PostView.nhn?blogId=beyondlegend&logNo=110029848992&from=search

//linked list
typedef struct filelist{
    char path[100];
    struct filelist *next;
}FILELIST;

//extention filter func
int Filter(char * path){
    char *temp;
    temp=strrchr(path,'.');
    if(temp!=NULL){
        if( (!strcmp(temp,".txt")) || (!strcmp(temp,".TXT")) ){
            return 1;
        }
    }
    return 0;
}

FILELIST *ScanDir(char *path, FILELIST *head){
    DIR *dp;
    struct dirent *direntry;
    struct stat fstat;
    char newPath[500];
    FILELIST *list=head;


    if( (dp=opendir(path) )==NULL ){
        printf("error\n");
        return NULL;
    }

    while( (direntry=readdir(dp)) ){
        if( (!strcmp(direntry->d_name,".")) || (!strcmp(direntry->d_name,"..")) )
            continue;
        sprintf(newPath,"%s%s%s",path,"/",direntry->d_name);
        if(stat(newPath,&fstat)!=0){
            printf("stat error\n");
            continue;
        }
        if(S_ISDIR(fstat.st_mode)){//디렉토리
            list=ScanDir(newPath,list);
        }
        else{//파일
            if(Filter(direntry->d_name)){
                FILELIST *temp;
                temp=(FILELIST*)malloc(sizeof(FILELIST));
                strcpy(temp->path,newPath);
                temp->next=NULL;
                list->next=temp;
                list=temp;
            }
        }
    }
    closedir(dp);
    return list;
}

void PrintList(FILELIST *head){
    FILELIST *list=head->next;
    while(list!=NULL){
        printf("%s\n",list->path);
        list=list->next;
    }
}

void FreeList(FILELIST *head){
    FILELIST *temp;
    while(head!=NULL){
        temp=head->next;
        free(head);
        head=temp;
    }
}

int main(int argc, char *argv[]){
    FILELIST *list;
    list=(FILELIST*)malloc(sizeof(FILELIST));
    list->next=NULL;
    ScanDir(".",list);
    PrintList(list);
    FreeList(list);
    return 0;
}
