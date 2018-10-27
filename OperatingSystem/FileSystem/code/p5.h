
#ifndef P5_H
#define P5_H
#include <time.h>
#define MAX_FILE_NAME_LENGTH 200
#define MAX_OPEN_FILES 10

/* file API */
extern int my_open(const char *path);
extern int my_creat(const char *path);
extern int my_read(int fd, void *buf, int count);
extern int my_write(int fd, const void *buf, int count);
extern int my_close(int fd);

extern int my_remove(const char *path);
extern int my_rename(const char *old, const char *new);
extern int my_mkdir(const char *path);
extern int my_rmdir(const char *path);

extern void my_mkfs();

/* provided by the lower layer */

#define BLOCKSIZE 1024
/* not used in any declaration, just a reminder that each block is 1KB */
/* and may be useful inside the code. */
typedef char block[BLOCKSIZE];

extern int dev_open();
extern int read_block(int block_num, char *block);
extern int write_block(int block_num, char *block);

/* provided by block function */

#define MAX_PER_BITMAP 500    /* max block covered of one bit map */
#define MAX_DIRENTRY 18       /* max directionary entry each block */
#define BITMAP_NUM 500        /* number of bit map needed*/
#define ROOT_DIR_BLOCK 502    /* number of bit map needed*/
#define BITMAP_START 2        /* where bitmap store in disk */
#define INODE_DIRECT_MAX 200  /* inode direct block max 200 */
#define INODE_DIRECT1_MAX 256 /* inode direct block max 200 */

/* super block struct, 1020 KB */
typedef struct SuperBlk
{
    int is_format;                      /* is this file system init? */
    short bitmap_usage[BITMAP_NUM + 5]; /*  usage of each bit map ,begin with index 2 */
    int free_space;                     /* free space */
} SUPERBLK;

/* each Dictionary Entry has 56 bytes, a block can have 18 entry */
typedef struct DirEntry
{
    char name[50]; /* file name max length */
    int i_node;    /* the i-node of file */
} DIRENTRY;

/* each Dictionary has 18 entry,  overall 1012 bytes */
typedef struct Dir
{
    int in_use; /* how many dir is using  */
    DIRENTRY entry[MAX_DIRENTRY];
} DIR;

/* inode size: 820 bytes */
typedef struct inode
{
    time_t ctime;                    /* last change time (size : 8 bytes)*/
    int i_size;                      /* file size:(bytes) */
    int dir_block[INODE_DIRECT_MAX]; /* direct block */
    int indir_block_1;               /* indirect block 1  */
    int indir_block_2;               /* indirect block 2  */
} INODE;

/* need 500 bitmap block for 250000 block
 * each bitmap 1008 btytes
 */
typedef struct bitmap
{
    int start_block;              /* start block of the bit map */
    int free_block;               /* free block in this bitmap  */
    short bitmap[MAX_PER_BITMAP]; /* bit map */
} BITMAP;

typedef int INDIR_BLOCK[INODE_DIRECT1_MAX]; /* indirect block, a block can fit 256 block pointer  */

extern int Write_Superblk(SUPERBLK sb);
extern int Write_Dir(int block_num, DIR dir);
extern int Write_Bitmap(int block_num, BITMAP btmp);
extern int Write_Inode(int block_num, INODE i_node);
extern int Write_Inode_block(int block_num, INDIR_BLOCK block1);
extern void clean_pathname();
extern int parse_path(const char *Path);
extern int find_file(int dir_num);
extern int bitmap_search_block(BITMAP *tmp);
extern int find_free_block();
extern int mk_new_dir(int parent_dir_disk_num);
extern int mk_new_inode();
extern void free_space(int free_block_num);
extern void free_indirect_block1(int indirect_block1_location);
extern void free_inode(int inode_block_num);
extern void read_file(int disk_num, void *buf, int length);
extern int read_indir_block1(int block1_disk_num, void *buf, int left);
extern int read_from_fpBuf(int fp_index, int count, void *buf);
extern int read_to_fpBuf(INODE inodeTmp, int fd_index);
extern void write_file(int disk_num, void *buf, int length);
extern int write_indir_block1(int block1_disk_num, void *buf, int left);
extern int write_fpBuf_to_disk(int fd, INODE InodeTmp, int fd_index);

/* global variables */
extern char PathName[5][20]; /* globel Path Name parsed */

extern SUPERBLK SuperBlock; /* superblock will be used frequently */

extern DIR RootDir; /* Root Dir will be used frequently, max 18 entry */

extern INODE tmpInode;

extern int OpenFP[50]; /* at most 50 fp */

extern char *OpenFP_buffer[50]; /* the buffer of fp */

extern char *OpenFP_buffer_old[50]; /* the buffer of fp,the fp position unchanged */

extern char TmpBuf[BLOCKSIZE]; /* used for read/wirte block from disk */

#endif /* P5_H */
