#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//오름차순
int int_compare(const void *a, const void *b)
{
    int num1 = *(int*)a;
    int num2 = *(int*)b;
    if(num1>num2)
        return 1;
    else if(num1==num2)
        return 0;
    else
        return -1;
}

int string_compare(const void *a, const void *b)
{
    return strcmp( (char*)a,(char*)b );
}
int main(int argc, char *argv[])
{
    int int_arr[10]={1,6,8,9,5,2,3,7,4,0};
    char string_arr[10][8]={"india", "delta", "bravo", "kilo", "alfa","echo", "hotel", "juliett", "golf", "charlie"};
    //갯수, 요소 크기
    qsort(int_arr,sizeof(int_arr)/sizeof(int),sizeof(int),int_compare);
    for(int i=0;i<10;i++){
        printf("%d\n",int_arr[i]);
    }

    qsort(string_arr, sizeof(string_arr)/sizeof(string_arr[0]), sizeof(string_arr[0]), string_compare);
    for(int i=0;i<10;i++){
        printf("%s\n",string_arr[i]);
    }
    
    return 0;
}
