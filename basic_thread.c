#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *param)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*)param;

    pthread_mutex_lock(mutex);
    for(int i=0;i<10;i++)
    {
        printf("%d\n",i);
        sleep(1);
    }
    pthread_mutex_unlock(mutex);
}

int main( int argc, char *argv[] )
{
    pthread_t t_id;
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&t_id, NULL, thread_func, (void*)&mutex);
    
    pthread_join(t_id, NULL);
    return 0;
}
