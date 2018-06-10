
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define MAX_PER_BITMAP 450 /* max block covered of one bit map */
#define MAX_DIRENTRY 18    /* max directionary entry each block */
#define BITMAP_NUM 56      /* number of bit map needed*/

/* super block struct,976 KB */
typedef struct SuperBlk
{
    int is_format;                         /* is this file system init? */
    int bitmap_is_full[BITMAP_NUM + 10]; /*  each bit map is full?*/
    int next_free_block;                   /* next free block can be used*/
}SUPERBLK;

/* each Dictionary Entry has 56 bytes, a block can have 20 entry */
typedef struct DirEntry
{
    char name[50]; /* file name max length */
    int i_node;    /* the i-node of file */
} DIRENTRY;

/* inode size: 824 bytes */
typedef struct inode
{
    time_t ctime;           /* last change time (size : 8 bytes)*/
    unsigned int i_size;    /* file size:(bytes) */
    unsigned int block_num; /* how many block it use */
    int dir_block[200];     /* direct block */
    int indir_block_1;      /* indirect block 1  */
    int indir_block_2;      /* indirect block 2  */
} INODE;

/* need 56 bitmap block for 25000 block*/
typedef struct bitmap
{
    int start_block;            /* start block of the bit map */
    short is_full;                /* is the bitmap full? */
    short bitmap[MAX_PER_BITMAP]; /* bit map */
} BITMAP;
