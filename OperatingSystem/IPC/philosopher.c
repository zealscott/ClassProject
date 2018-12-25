#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define LEFT (i - 1 + N) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t s[N];


void think(int i)
{
    if (state[i] == THINKING)
    {
        printf("philosopher %d is thinking.......\n", i);
        sleep(3);
    }
}


void eat(int i)
{
    if (state[i] == EATING)
    {
        printf("philosopher %d is eating.......\n", i);
        sleep(3);
    }
}

void init()
{
    int i;
    for (i = 0; i < N; i++)
    {
        if (sem_init(&s[i], 1, 1) != 0)
        {
            printf("sem_init is wrong\n");
        }
    }
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        sem_post(&s[i]);
        return;
    }
}



void take_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex);
    sem_wait(&s[i]);
}

void put_forks(int i)
{
    pthread_mutex_lock(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex);
}


void philosopher(int i)
{
    while (1)
    {
        think(i);
        take_forks(i);
        eat(i);
        put_forks(i);
    }
}






int main()
{
    int i = 0;

    pthread_t id;
    int ret;
    init();

    for (i = 0; i < N; i++)
    {
        ret = pthread_create(&id, NULL, (void *)philosopher, i);
        if (ret != 0)
        {
            printf("Create pthread error!/n");
            return 1;
        }
    }
    pthread_join(id, NULL);
}