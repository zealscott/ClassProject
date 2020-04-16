#include <stdio.h>
#include <stdlib.h>
#include "limits.h"
#include "p5.c"

void test_bitmap()
{
    BITMAP TmpBitmp;
    char tmpbuf[BLOCKSIZE];
    for (int i = 2; i <= 57; i++)
    {
        read_block(i, tmpbuf);
        memcpy(&TmpBitmp, tmpbuf, sizeof(TmpBitmp));
        if (TmpBitmp.free_block != MAX_PER_BITMAP)
            printf("bitmap = %d, free_block = %d\n", i, TmpBitmp.free_block);
    }
}

int main(int argc, char const *argv[])
{
    BITMAP bittmp;
    INODE i_nodetmp;
    DIR dirTmp;
    printf("sizeof (superblock) = %d\n", sizeof(SuperBlock));
    printf("sizeof (BITMAP) = %d\n", sizeof(bittmp));
    printf("sizeof (INODE) = %d\n", sizeof(i_nodetmp));
    printf("sizeof (DIR) = %d\n", sizeof(dirTmp));
    // char *buf = "asdfghjkl";
    // char buf2[50];
    // my_mkdir("/b");
    // int fd = my_creat("/b/s.txt");
    // // int fd = my_open("/b/s.txt");
    // my_write(fd, buf, 20);
    // my_read(fd, buf2, 20);
    // printf("buf2 = %s\n",buf2);
    // test_bitmap();
    return 0;
}

// /* test mkdir */
//     char tmp[BLOCKSIZE];
//     BITMAP test;
//     my_mkfs();
//     read_block(2, tmp);
//     memcpy(&test, tmp, sizeof(test));
//     printf("%d", test.free_block);

/* test parse path name */
// my_mkfs();
// char *path = "/foo/bar5sdfdasf";
// int many = parse_path(path);
// printf("how many = %d",many);
// for (int i = 0; i <many;i++)
// {
//     printf("%s\n",PathName[i]);
// }

// /* test mkdir */
//     my_mkfs();
//     my_mkdir("/a");
//     my_mkdir("/foo/abc");

/* test rmdir */
//     my_mkfs();
// my_mkdir("/dfsf");
// my_creat("/b/tmdfp.txt");
// my_creat("/dfsf/adfgad");
// my_rmdir("/b");
// my_rmdir("/b");
// my_rmdir("/dfsf");