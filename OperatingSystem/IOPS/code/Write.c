#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

const int maxn = 1024 * 6 + 10; /* 6kB */
int iter = 10000;              /* 1m time*/
int filesize = 200 * 1024 * 1024;
char buff[maxn] = "hello world";

void WriteTest(int fp, int type, int block)
{
    int i = 0;
    for (; i < iter; i++)
    {
        write(fp, buff, block);
        if (type) /* is randomn write */
            lseek(fp, rand() % filesize, SEEK_SET);
    }
    lseek(fp, 0, SEEK_SET); /* reset file pointer */
}

int main()
{
    srand((unsigned)time(NULL));
    int ramfp, diskfp, block, i,FORKTIME;
    struct timeval start, end;
    for (i = 0; i < maxn;)
    {
        strncat(buff, "is your name?", 10);
        i += 10;
    }

    ramfp = open("/root/myram/read.txt", O_CREAT | O_RDWR | O_SYNC, 0755);
    diskfp = open("/usr/read.txt", O_CREAT | O_RDWR | O_SYNC, 0755);

    for (block = 64; block <= 4096;)
    {
        for (FORKTIME = 1; FORKTIME <= 31;)
        {
            gettimeofday(&start, NULL);
            for (i = 1; i <= FORKTIME; i++)
            {
                if (fork() == 0)
                {
                    // WriteTest(ramfp, 0, block); /* 1 for randomn */
                    // WriteTest(ramfp, 1, block);/* 1 for randomn */
                    WriteTest(diskfp, 0, block);/* 1 for randomn */
                    // WriteTest(diskfp, 1, block);/* 1 for randomn */
                    exit(1);
                }
            }
            while (wait(NULL) != -1)
            {
            }
            gettimeofday(&end, NULL);
            long alltime = (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec); /* us */
            double latency = (alltime / 1000) / (double)iter / FORKTIME;                          /* ms */

            double timeuse = latency / 1000;          /* second per iter */
            double block_kB = (double)block / 1024.0; /* convert to kB */
            double ops = block_kB / timeuse / 1024.0;
            printf("%d,%d,%f,%f\n", block, FORKTIME, latency, ops);

            FORKTIME += 6;
        }
        block *= 2;
    }
    return 0;
}
