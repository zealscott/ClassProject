
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct SuperBlk
{
    int is_format;
    int block_map[250000]; //磁盘块位图
    int next_block;        //已被使用的i节点数目
    int root_block;        //已被使用的磁盘块数目
} SuperBlock;

int main(int argc, char const *argv[])
{
    // SuperBlock.is_format = 1;
    int fd;
    // char *tp = (char *)(&SuperBlock);
    // printf("superblock address = %p\n",&SuperBlock);
    // printf("tp address = %p\n",tp);
    // printf("is_format = %d\n",*tp);

    fd = open("/usr/simulated_device", O_RDWR);

    // if (write(fd, tp, 1024) != 1024)
    // {
    //     perror("write");
    //     return -1;
    // }
    // close(fd);
    // fd = open("/usr/simulated_device", O_RDWR);

    char a[1024];
    if (read(fd, a, 1024) != 1024)
    {
        perror("read");
        return -1;
    }
    close(fd);
    struct SuperBlk * blk = (struct SuperBlk *)a;
    printf("%d\n",blk->is_format);
    return 0;
}
