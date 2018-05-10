#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t db; /* control the access of database  */
int rc;   /* the process want to access the database */

void init()
{
    rc = 0;
    if (sem_init(&db, 1, 1) != 0)
    {
        printf("sem_init is wrong\n");
    }
}

void read_data(int i)
{
    printf("person %d is reading.......\n", i);
    sleep(5);
}

void finish_read(int i)
{
    printf("person %d  finish reading!\n", i);
}

void write_data()
{
    printf("person is writing.......\n");
    sleep(3);
}

void finish_write()
{
    printf("person finish writing!\n");
}

void reader()
{
    while (1)
    {
        sleep(1); /* wait for reading ... */
        pthread_mutex_lock(&mutex);
        rc += 1;
        if (rc == 1)
            sem_wait(&db);
        pthread_mutex_unlock(&mutex);

        read_data(rc);

        pthread_mutex_lock(&mutex);
        rc -= 1;
        finish_read(rc);
        if (rc == 0)
            sem_post(&db);
        pthread_mutex_unlock(&mutex);
        sleep(3); /* wait for another reading ... */
    }
}

void writer()
{
    while (1)
    {
        sem_wait(&db);
        write_data();
        finish_write();
        sem_post(&db); /* restore the access */
        sleep(3);      /* wait for another writing ... */
    }
}

int main()
{
    int i = 0;

    pthread_t id1, id2;
    int ret1, ret2;
    init();

    for (i = 0; i < N; i++)
    {
        ret1 = pthread_create(&id1, NULL, (void *)reader, NULL);
        ret2 = pthread_create(&id2, NULL, (void *)writer, NULL);
        if (ret1 != 0 || ret2 != 0)
        {
            printf("Create pthread error!/n");
            return 1;
        }
    }
    pthread_join(id1, NULL);
    pthread_join(id2, NULL);
    return 1;
}