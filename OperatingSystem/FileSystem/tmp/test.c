#include <stdio.h>
#include <stdlib.h>
#include "p5.c"

int main(int argc, char const *argv[])
{


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