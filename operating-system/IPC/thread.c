#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int sharedi = 0;
int A = 10000;
int B = 10000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread1()
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        A += 100;
        // sleep(1);
        B -= 100;
        pthread_mutex_unlock(&mutex);
        printf("thread1: A  = %d , B = %d\n", A, B);
        
        
    }
}

void thread2()
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        A += 100;
        // sleep(1);
        B -= 100;
        pthread_mutex_unlock(&mutex);
        printf("thread2: A  = %d , B = %d\n", A, B);
        
    }
}



int main()
{
    int ret;
    pthread_t thrd1, thrd2;

    ret = pthread_create(&thrd1, NULL, (void *)thread1, NULL);
    ret = pthread_create(&thrd2, NULL, (void *)thread2, NULL);
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);

    return 0;
}
