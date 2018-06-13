
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define MAX_PER_BITMAP 450    /* max block covered of one bit map */
#define MAX_DIRENTRY 18       /* max directionary entry each block */
#define BITMAP_NUM 56         /* number of bit map needed*/
#define BITMAP_START 2        /* where bitmap store in disk */
#define INODE_DIRECT_MAX 200  /* inode direct block max 200 */
#define INODE_DIRECT1_MAX 256 /* inode direct block max 200 */

/* super block struct, 976 KB */
typedef struct SuperBlk
{
    int is_format;                     /* is this file system init? */
    int bitmap_usage[BITMAP_NUM + 10]; /*  usage of each bit map ,begin with index 2 */
    int free_space;                    /* free space */
} SUPERBLK;

/* each Dictionary Entry has 56 bytes, a block can have 18 entry */
typedef struct DirEntry
{
    char name[50]; /* file name max length */
    int i_node;    /* the i-node of file */
} DIRENTRY;

typedef struct Dir
{
    int in_use; /* how many dir is using  */
    DIRENTRY entry[MAX_DIRENTRY];
} DIR;

/* inode size: 824 bytes */
typedef struct inode
{
    time_t ctime;                    /* last change time (size : 8 bytes)*/
    int bias;                        /* bias of fd, bytes */
    int i_size;                      /* file size:(bytes) */
    int dir_block[INODE_DIRECT_MAX]; /* direct block */
    int indir_block_1;               /* indirect block 1  */
    int indir_block_2;               /* indirect block 2  */
} INODE;

/* need 56 bitmap block for 25000 block*/
typedef struct bitmap
{
    int start_block;              /* start block of the bit map */
    int free_block;               /* free block in this bitmap  */
    short bitmap[MAX_PER_BITMAP]; /* bit map */
} BITMAP;

typedef int INDIR_BLOCK[INODE_DIRECT1_MAX];

/* ------------------------------------------ */
/* claim global variables */
char *PathName[5]; /* globel Path Name parsed */

SUPERBLK SuperBlock; /* superblock will be used frequently */

DIR RootDir; /* Root Dir will be used frequently, max 18 entry */

INODE tmpInode;

int OpenFP[50]; /* at most 50 fp */

char *OpenFP_buffer[50]; /* the buffer of fp */

char *OpenFP_buffer_old[50]; /* the buffer of fp,the fp position unchanged */
/* ------------------------------------------ */

/* clean PathName */
void clean_pathname()
{
    for (int i = 0; i < 5; i++)
        memset(PathName[i], 0, sizeof(PathName));
}

/* write superblock back to disk */
int Write_Superblk(SUPERBLK sb)
{
    char tmp[BLOCKSIZE];
    memcpy(tmp, &sb, sizeof(sb));
    if (write_block(1, tmp) == 0)
        return 0;
    else
        return -1;
}

/* write Dir back to disk */
int Write_Dir(int block_num, DIR dir)
{
    char tmp[BLOCKSIZE];
    memcpy(tmp, &dir, sizeof(dir));
    if (write_block(block_num, tmp) == 0)
        return 0;
    else
        return -1;
}

/* write bitmap back to disk */
int Write_Bitmap(int block_num, BITMAP btmp)
{
    char tmp[BLOCKSIZE];
    memcpy(tmp, &btmp, sizeof(btmp));
    if (write_block(block_num, tmp) == 0)
        return 0;
    else
        return -1;
}

/* write i-node back to disk */
int Write_Inode(int block_num, INODE i_node)
{
    char tmp[BLOCKSIZE];
    memcpy(tmp, &i_node, sizeof(i_node));
    if (write_block(block_num, tmp) == 0)
        return 0;
    else
        return -1;
}

/* write i-node block back to disk */
int Write_Inode_block(int block_num, INDIR_BLOCK block1)
{
    char tmp[BLOCKSIZE];
    memcpy(tmp, block1, sizeof(block1));
    if (write_block(block_num, tmp) == 0)
        return 0;
    else
        return -1;
}

/* return how many dir */
int parse_path(char *Path)
{
    char *p = NULL;
    int i = 0;
    for (p = strtok(Path, "/"); p != NULL; p = strtok(NULL, "/"))
    {
        memcpy(PathName[i++], p, strlen(p));
    }
    return i;
}

/* find file or dir block location from global PathName */
int find_file(int dir_num)
{
    if (dir_num == 0)
        return 58;
    if (dir_num < 0)
        return -1;
    int disk_num = 58, i, j, dir_in_use; /* root dir location */
    char TmpBuf[BLOCKSIZE];
    DIR tmpDir;
    for (i = 0; i < dir_num; i++)
    {
        read_block(disk_num, TmpBuf);
        memcpy(&tmpDir, TmpBuf, sizeof(tmpDir));
        dir_in_use = tmpDir.in_use; /* how many file this dir contained */
        for (j = 0; j < dir_in_use; j++)
        {
            if (strcmp(PathName[i], tmpDir.entry[j].name) == 0)
            {
                disk_num = tmpDir.entry[j].i_node;
                break;
            }
        }
        if (j == dir_in_use)
            return -1; /* not found */
    }
    return disk_num;
}

/* find the free block from bitmap, set bitmap para*/
int bitmap_search_block(BITMAP *tmp)
{
    int i;
    if (tmp->free_block == 0)
        return -1; /* no more space */
    for (i = 0; i < MAX_PER_BITMAP; i++)
    {
        if (tmp->bitmap[i] == 0)
        {
            tmp->bitmap[i] = 1;
            tmp->free_block--;
            return (i + tmp->start_block); /* there is a free block */
        }
    }
    return -1;
}

/* find free block from bitmap, 
 * set the location to superblock 
 * write SUPERBLK, bitmap back to disk
 */
int find_free_block()
{
    int i, location;
    char TmpBuf[BLOCKSIZE];
    BITMAP tmpBitmap;
    if (SuperBlock.free_space == 0)
        return -1; /* no more space */
    for (i = 2; i < BITMAP_NUM + BITMAP_START; i++)
    {
        if (SuperBlock.bitmap_usage[i] < MAX_PER_BITMAP)
            break;
    }

    /* set superblock parameters */
    SuperBlock.bitmap_usage[i]++;
    SuperBlock.free_space--;
    Write_Superblk(SuperBlock); /* write back */
    /* read bitmap */
    read_block(i, TmpBuf);
    memcpy(&tmpBitmap, TmpBuf, sizeof(tmpBitmap));
    location = bitmap_search_block(&tmpBitmap); /* find free block location */
    /* write back */
    Write_Bitmap(i, tmpBitmap);
    return location;
}

/* allocate new block for dir, write back 
 * not success ,return -1,else, return block_location 
 */
int mk_new_dir(int parent_dir_disk_num)
{
    char TmpBuf[BLOCKSIZE];
    int block_location;
    DIR tmpDir;
    if ((block_location = find_free_block()) == -1)
        return -1;
    if (read_block(block_location, TmpBuf) == 0)
    {
        memcpy(&tmpDir, TmpBuf, sizeof(tmpDir));
        strcpy(tmpDir.entry[0].name, ".");
        strcpy(tmpDir.entry[1].name, "..");
        tmpDir.entry[0].i_node = block_location;
        tmpDir.entry[1].i_node = parent_dir_disk_num;
        tmpDir.in_use = 2; /* just . and .. */
        Write_Dir(block_location, tmpDir);
        return block_location;
    }
    else
        return -1;
}

/* allocate new block for i-node, write back 
 * not success ,return -1,else, return block_location 
 */
int mk_new_inode()
{
    char TmpBuf[BLOCKSIZE];
    int block_location;
    INODE tmpInode;
    if ((block_location = find_free_block()) == -1)
        return -1;
    read_block(block_location, TmpBuf);
    /* init i-node */
    memcpy(&tmpInode, TmpBuf, sizeof(tmpInode));
    memset(&tmpInode, 0, sizeof(tmpInode));
    tmpInode.ctime = time(NULL);
    Write_Inode(block_location, tmpInode);
    return block_location;
}

/* free block 
 * change superblock and bitmap
 */
void free_space(int free_block_num)
{
    int bitmap_location, i;
    char TmpBuf[BLOCKSIZE];
    BITMAP Tmpbitmap;
    /* (450-1)/450 + 2 = 2, so in disk No.2 */
    bitmap_location = (free_block_num - 1) / MAX_PER_BITMAP + BITMAP_START;
    SuperBlock.bitmap_usage[bitmap_location]--;
    SuperBlock.free_space++;
    read_block(bitmap_location, TmpBuf); /* read bitmap from disk */
    memcpy(&Tmpbitmap, TmpBuf, sizeof(Tmpbitmap));
    Tmpbitmap.bitmap[free_block_num - Tmpbitmap.start_block] = 0; /* free space */
    Tmpbitmap.free_block++;
    /* write back  */
    Write_Superblk(SuperBlock);
    Write_Bitmap(bitmap_location, Tmpbitmap);
}

/* free indirect block1
 * and itself
 */
void free_indirect_block1(int indirect_block1_location)
{
    int i, block_locatin;
    char TmpBuf[BLOCKSIZE];
    INDIR_BLOCK block1;
    if (indirect_block1_location >= 58)
    {
        read_block(indirect_block1_location, TmpBuf);
        memcpy(block1, TmpBuf, sizeof(block1));
        for (i = 0; i < INODE_DIRECT1_MAX; i++)
        {
            if ((block_locatin = block1[i]) >= 59)
                free_space(block_locatin);
            else
                break;
        }
        /* free itself */
        free_space(indirect_block1_location);
        /* clean block */
        memset(block1, 0, sizeof(block1));
        Write_Inode_block(indirect_block1_location, block1);
    }
    return;
}

/* free inode  data block(indirect,direct)
 * set inode to zero
 * free inode itself
 */
void free_inode(int inode_block_num)
{
    int block_locatin, i;
    char TmpBuf[BLOCKSIZE];
    INODE InodeTmp;
    INDIR_BLOCK block2;
    read_block(inode_block_num, TmpBuf);
    memcpy(&InodeTmp, TmpBuf, sizeof(InodeTmp));
    /* free direct block */
    for (i = 0; i < INODE_DIRECT_MAX; i++)
    {
        if ((block_locatin = InodeTmp.dir_block[i]) >= 59)
            free_space(block_locatin);
        else
            break;
    }
    /* free indirect block and itself */
    free_indirect_block1(InodeTmp.indir_block_1);
    /* free indirect block2 */
    if (InodeTmp.indir_block_2 >= 58)
    {
        read_block(InodeTmp.indir_block_2, TmpBuf);
        memcpy(block2, TmpBuf, sizeof(block2));
        for (i = 0; i < INODE_DIRECT1_MAX; i++)
        {
            if ((block_locatin = block2[i]) >= 59)
                free_indirect_block1(block_locatin);
            else
                break;
        }
        free_space(InodeTmp.indir_block_2); /* free indirect block2 itself */
        memset(block2, 0, sizeof(block2));
        Write_Inode_block(InodeTmp.indir_block_2, block2);
    }
    /* free inode itself */
    free_space(inode_block_num);
    memset(&InodeTmp, 0, sizeof(InodeTmp));
    Write_Inode(inode_block_num, InodeTmp);
    return;
}

/** read file from disk to buf
 * 
 */
void read_file(int disk_num, void *buf, int length)
{
    char TmpBuf[BLOCKSIZE];
    read_block(disk_num, TmpBuf);
    memcpy(buf, TmpBuf, length);
}

/** read indirect block1 
 *  return 0 for success
 *  else return left size unread
 */
int read_indir_block1(int block1_disk_num, void *buf, int left)
{
    int index = 0, i;
    INDIR_BLOCK block1;
    char TmpBuf[BLOCKSIZE];
    read_block(block1_disk_num, TmpBuf);
    memcpy(block1, TmpBuf, sizeof(block1));
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if (left < BLOCKSIZE)
        {
            read_file(block1[i], buf + index, left);
            return 0; /* read success */
        }
        read_file(block1[i], buf + index, BLOCKSIZE);
        left -= BLOCKSIZE;
        index += BLOCKSIZE;
    }
    return (left > 0 ? left : 0);
}

/** read file from DRAM buffer 
 *  return read bytes for success
 *  else return -1
 */
int read_from_fpBuf(int fp_index, int count, void *buf)
{
    int i;
    if (OpenFP_buffer[fp_index] != 0)
    {
        memcpy(buf, OpenFP_buffer[fp_index], count);
        OpenFP_buffer[fp_index] += count; /* add bias */
        return count;
    }
    return -1;
}

/** read file from disk to buffer
 *  return 1 for success
 */
int read_to_fpBuf(INODE inodeTmp, int fd_index)
{
    char TmpBuf[BLOCKSIZE];
    int left = inodeTmp.i_size, i;
    char *FilePointer = OpenFP_buffer[fd_index];
    /* read from direct block */
    for (i = 0; i < INODE_DIRECT_MAX; i++)
    {
        if (left < BLOCKSIZE)
        {
            read_file(inodeTmp.dir_block[i], FilePointer, left);
            return 1; /* read success */
        }
        read_file(inodeTmp.dir_block[i], FilePointer, BLOCKSIZE);
        left -= BLOCKSIZE;
        FilePointer += BLOCKSIZE;
    }
    if (left > 0) /* using indirect block1 */
    {
        left = read_indir_block1(inodeTmp.indir_block_1, FilePointer, left);
        FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX); /* add bias */
        if (left == 0)
            return 1; /* read complete */
        /* else, read from block2 */
        INDIR_BLOCK block2;
        read_block(inodeTmp.indir_block_2, TmpBuf);
        memcpy(block2, TmpBuf, sizeof(block2));
        for (i = 0; i < INODE_DIRECT1_MAX; i++)
        {
            left = read_indir_block1(block2[i], FilePointer, left);
            if (left == 0)
                return 1; /* read success */
            FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX);
        }
    }
    return 1;
}

/** write file to disk
 * 
 */
void write_file(int disk_num, void *buf, int length)
{
    char TmpBuf[BLOCKSIZE];
    memcpy(TmpBuf, buf, length);
    write_block(disk_num, TmpBuf);
}

/** write indirect block1, allocate spcae for file 
 *  return 0 for success
 *  else return left size unwrite
 */
int write_indir_block1(int block1_disk_num, void *buf, int left)
{
    int index = 0, disk_num, i;
    INDIR_BLOCK block1;
    char TmpBuf[BLOCKSIZE];
    read_block(block1_disk_num, TmpBuf);
    memcpy(block1, TmpBuf, sizeof(block1));
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        /* allocate space for block */
        disk_num = find_free_block();
        if (disk_num == -1)
        {
            printf("my_write Error: fail to allocate new block.\n");
            exit(1);
        }
        block1[i] = disk_num;
        if (left < BLOCKSIZE)
        {
            write_file(disk_num, buf + index, left);
            return 0; /* read success */
        }
        write_file(disk_num, buf + index, BLOCKSIZE);
        left -= BLOCKSIZE;
        index += BLOCKSIZE;
    }
    Write_Inode_block(block1_disk_num, block1); /* write back to disk */
    return (left > 0 ? left : 0);
}

/** write file from DRAM to disk
 *  return 1 for success
 */
int write_fpBuf_to_disk(int fd, INODE InodeTmp, int fd_index)
{
    int left = InodeTmp.i_size, i, index = 0, disk_num;
    char *FilePointer = OpenFP_buffer_old[fd_index];
    INDIR_BLOCK block2;
    /* read from direct block */
    for (i = 0; i < INODE_DIRECT_MAX; i++)
    {
        if (InodeTmp.dir_block[i] == 0) /* if block not allocated */
        {
            disk_num = find_free_block();
            if (disk_num == -1)
            {
                printf("my_write Error:fail to allocate new block.\n");
                exit(1);
            }
            InodeTmp.dir_block[i] = disk_num; /* store block location */
        }
        if (left < BLOCKSIZE)
        {
            write_file(InodeTmp.dir_block[i], FilePointer + index, left);
            Write_Inode(fd, InodeTmp); /* store to disk */
            printf("my_write success, i-node = %d\n", fd);
            return 1; /* read success */
        }
        write_file(InodeTmp.dir_block[i], FilePointer + index, BLOCKSIZE);
        left -= BLOCKSIZE;
        index += BLOCKSIZE;
    }
    /* using indirect block1 */
    if (left > 0)
    {
        if (InodeTmp.indir_block_1 == 0) /* indir_block_1 not allocated */
        {
            disk_num = find_free_block();
            if (disk_num == -1)
            {
                printf("my_write Error:fail to allocate new block.\n");
                exit(1);
            }
            InodeTmp.indir_block_1 = disk_num;
        }
        left = write_indir_block1(InodeTmp.indir_block_1, FilePointer + index, left);
        index += (BLOCKSIZE * INODE_DIRECT1_MAX);
        if (left == 0)
        {
            Write_Inode(fd, InodeTmp); /* store to disk */
            printf("my_write success, i-node = %d, using indir_block_1\n", fd);
            return 0; /* read complete */
        }
        /* else, read from block2 */
        if (InodeTmp.indir_block_2 == 0)
        {
            disk_num = find_free_block();
            if (disk_num == -1)
            {
                printf("my_write Error:fail to allocate new block.\n");
                return -1;
            }
            InodeTmp.indir_block_2 = disk_num; /* store block2 location */
        }

        memset(block2, 0, sizeof(block2));
        for (i = 0; i < INODE_DIRECT1_MAX; i++)
        {
            disk_num = find_free_block();
            if (disk_num == -1)
            {
                printf("my_write Error:fail to allocate new block.\n");
                exit(1);
            }
            block2[i] = disk_num;
            left = write_indir_block1(block2[i], FilePointer + index, left);
            if (left == 0)
            {
                Write_Inode_block(InodeTmp.indir_block_2, block2);
                printf("my_write success, i-node = %d, using indir_block_2\n", fd);
                return 1; /* read success */
            }
            index += (BLOCKSIZE * INODE_DIRECT1_MAX);
        }
    }
    Write_Inode_block(InodeTmp.indir_block_2, block2);
    return 1;
}
