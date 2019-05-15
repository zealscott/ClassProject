
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "p5.h"

/* claim global variables */
char PathName[5][20]; /* globel Path Name parsed */

SUPERBLK SuperBlock; /* superblock will be used frequently */

DIR RootDir; /* Root Dir will be used frequently, max 18 entry */

INODE tmpInode;

int OpenFP[50]; /* at most 50 fp */

char *OpenFP_buffer[50]; /* the buffer of fp */

char *OpenFP_buffer_old[50]; /* the buffer of fp,the fp position unchanged */

char TmpBuf[BLOCKSIZE]; /* used for read/wirte block from disk */
/* ------------------------------------------ */

/* following functions are wapper function based on write_block()
 * just for convenience and clarity 
 */

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
    memcpy(tmp, block1, 1024);
    if (write_block(block_num, tmp) == 0)
        return 0;
    else
        return -1;
}

/* following functions are used to clean PathName
 * be careful need to clean PathName(global variable) each time read/write
 */

/* clean PathName */
void clean_pathname()
{
    for (int i = 0; i < 5; i++)
        memset(PathName[i], 0, sizeof(PathName[i]));
}

/* return how many dir */
int parse_path(const char *Path)
{
    char buf[50];
    int i = 0, j = 0;
    strcpy(buf, Path);
    char *temp = strtok(buf, "/");
    while (temp)
    {
        strcpy(PathName[i], temp);
        i++;
        temp = strtok(NULL, "/");
    }
    return i;
}

/* find file or dir block location from global PathName */
int find_file(int dir_num)
{
    if (dir_num == 0) /* root dir */
        return ROOT_DIR_BLOCK;
    if (dir_num < 0) /* should not happen */
        return -1;
    int disk_num = ROOT_DIR_BLOCK, i, j, dir_in_use; /* root dir location */
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
        if (j == dir_in_use) /* not found */
            return -1;
    }
    return disk_num;
}

/* find the free block from bitmap
 * set bitmap parameters
 * return find block location
 */
int bitmap_search_block(BITMAP *tmp)
{
    int i;
    for (i = 0; i < MAX_PER_BITMAP; i++)
    {
        if (tmp->bitmap[i] == 0)
        {
            tmp->bitmap[i] = 1;
            tmp->free_block--;
            return (i + tmp->start_block); /* there is a free block */
        }
    }
    printf("bitmap_search_block Error: unable to free space.\n");
    exit(1);
}

/* find free block from bitmap, 
 * set the location to superblock 
 * write SUPERBLK, bitmap back to disk
 * return free block location
 */
int find_free_block()
{
    int i, location;
    BITMAP tmpBitmap;
    if (SuperBlock.free_space <= 0) /* no more space */
        return -1;
    for (i = 3; i < BITMAP_NUM + BITMAP_START; i++)
    {
        if (SuperBlock.bitmap_usage[i] < MAX_PER_BITMAP)
            break;
    }
    /* set superblock parameters */
    SuperBlock.bitmap_usage[i]++;
    SuperBlock.free_space--;
    read_block(i, TmpBuf); /* read bitmap */
    memcpy(&tmpBitmap, TmpBuf, sizeof(tmpBitmap));
    location = bitmap_search_block(&tmpBitmap);
    Write_Bitmap(i, tmpBitmap); /* write bitmap to disk */
    return location;
}

/* allocate new block for dir, write back 
 * not success ,return -1
 * else, return block_location 
 */
int mk_new_dir(int parent_dir_disk_num)
{
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
    BITMAP Tmpbitmap;
    /* (501-1)/500 + 2 = 3, so in disk No.3 */
    bitmap_location = (free_block_num - 1) / MAX_PER_BITMAP + BITMAP_START;
    SuperBlock.bitmap_usage[bitmap_location]--;
    SuperBlock.free_space++;
    read_block(bitmap_location, TmpBuf); /* read bitmap from disk */
    memcpy(&Tmpbitmap, TmpBuf, sizeof(Tmpbitmap));
    Tmpbitmap.bitmap[free_block_num - Tmpbitmap.start_block] = 0; /* free space */
    Tmpbitmap.free_block++;
    Write_Bitmap(bitmap_location, Tmpbitmap);
    return;
}

/* free indirect block1 
 * and itself
 * used by i-node indirect block1 and indirect block2
 */
void free_indirect_block1(int indirect_block1_location)
{
    int i, block_locatin;
    INDIR_BLOCK block1;
    if (indirect_block1_location < (BITMAP_NUM + 2)) /* not exist */
        return;
    read_block(indirect_block1_location, TmpBuf);
    memcpy(block1, TmpBuf, sizeof(block1));
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if ((block_locatin = block1[i]) >= (BITMAP_NUM + 2)) /* if the block exist */
            free_space(block_locatin);
        else
            return;
    }
    free_space(indirect_block1_location); /* free itself */
}

/* free inode data block(indirect,direct)
 * free inode itself
 */
void free_inode(int inode_block_num)
{
    int block_locatin, i;
    INODE InodeTmp;
    INDIR_BLOCK block2;
    read_block(inode_block_num, TmpBuf);
    memcpy(&InodeTmp, TmpBuf, sizeof(InodeTmp));
    free_space(inode_block_num); /* free inode */
    /* free direct block */
    for (i = 0; i < INODE_DIRECT_MAX; i++)
    {
        if ((block_locatin = InodeTmp.dir_block[i]) >= (BITMAP_NUM + 2))
            free_space(block_locatin);
        else
            return;
    }
    /* free indirect block1 */
    free_indirect_block1(InodeTmp.indir_block_1);
    if (InodeTmp.indir_block_2 == 0)
        return;
    /* free indirect block2 */
    read_block(InodeTmp.indir_block_2, TmpBuf);
    memcpy(block2, TmpBuf, sizeof(block2));
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if ((block_locatin = block2[i]) >= (BITMAP_NUM + 2)) /* block exist */
            free_indirect_block1(block_locatin);
        else
            break;
    }
    free_space(InodeTmp.indir_block_2); /* free indirect block2 itself */
    return;
}

/*  read file from disk to buf */
void read_file(int disk_num, void *buf, int length)
{
    if (disk_num == 0)
    {
        printf("read_file Error.\n");
        exit(0);
    }
    read_block(disk_num, TmpBuf);
    memcpy(buf, TmpBuf, length);
}

/* read indirect block1 
 * return 0 for success
 * else return left size unread
 */
int read_indir_block1(int block1_disk_num, void *buf, int left)
{
    if (block1_disk_num == 0)
    {
        printf("read_indir_block1 Error: block1_disk_num == 0");
        exit(0);
    }
    int i;
    INDIR_BLOCK block1;
    read_block(block1_disk_num, TmpBuf);
    memcpy(block1, TmpBuf, 1024);
    /* for debug */
    if (block1[0] == 0)
    {
        printf("read_indir_block1 Error! block1 not exist.\n");
        exit(1);
    }
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if (left <= BLOCKSIZE)
        {
            read_file(block1[i], buf, left);
            return 0; /* read success */
        }
        read_file(block1[i], buf, BLOCKSIZE);
        left -= BLOCKSIZE;
        buf += BLOCKSIZE; /* add bias */
    }
    return left;
}

/* read file from DRAM buffer 
 * assume DRAM buffer exists
 * called by my_read()
 * return read bytes for success
 * else return -1
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

/* read file from disk to buffer
 * called by read/write 
 * assume file exists
 * return 1 for success
 */
int read_to_fpBuf(INODE inodeTmp, int fd_index)
{
    int left = inodeTmp.i_size, i;
    char *FilePointer = OpenFP_buffer[fd_index];
    INDIR_BLOCK block2;
    /* read from direct block */
    for (i = 0; i < INODE_DIRECT_MAX; i++)
    {
        if (left <= BLOCKSIZE)
        {
            read_file(inodeTmp.dir_block[i], FilePointer, left);
            return 1; /* read success */
        }
        read_file(inodeTmp.dir_block[i], FilePointer, BLOCKSIZE);
        left -= BLOCKSIZE;
        FilePointer += BLOCKSIZE;
    }
    /* read from indir_block_1 */
    left = read_indir_block1(inodeTmp.indir_block_1, FilePointer, left);
    FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX); /* add bias */
    if (left == 0)
        return 1; /* read complete */
    /* else, read from block2 */
    read_block(inodeTmp.indir_block_2, TmpBuf);
    memcpy(block2, TmpBuf, 1024);
    if (block2[0] == 0)
    {
        printf("read_to_fpBuf Error, block2 not exist.\n");
        exit(0);
    }
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        left = read_indir_block1(block2[i], FilePointer, left);
        if (left == 0)
            return 1; /* read success */
        FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX);
    }
    return 1;
}

/*  write file to disk */
void write_file(int disk_num, void *buf, int length)
{
    memcpy(TmpBuf, buf, length);
    write_block(disk_num, TmpBuf);
}

/* write indirect block1, allocate spcae for file 
 * return 0 for success
 * else return left size unwrite
 */
int write_indir_block1(int block1_disk_num, void *buf, int left)
{
    if (block1_disk_num == 0)
    {
        printf("write_indir_block1 Error: block1_disk_num = 0\n");
        exit(1);
    }
    int disk_num, i;
    INDIR_BLOCK block1;
    read_block(block1_disk_num, TmpBuf);
    memcpy(block1, TmpBuf, sizeof(block1));
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if (block1[i] == 0)
        {
            disk_num = find_free_block(); /* allocate space for block */
            if (disk_num == -1)
            {
                printf("my_write Error: fail to allocate new block.left = %d \n", left);
                exit(1);
            }
            block1[i] = disk_num;
        }
        if (left <= BLOCKSIZE)
        {
            write_file(block1[i], buf, left);
            Write_Inode_block(block1_disk_num, block1); /* write back to disk */
            return 0;                                   /* read success */
        }
        write_file(block1[i], buf, BLOCKSIZE);
        left -= BLOCKSIZE;
        buf += BLOCKSIZE;
    }
    Write_Inode_block(block1_disk_num, block1); /* write back to disk */
    return left;
}

/* write file from DRAM to disk
 * fd_index is the DRAM buffer index
 * return 1 for success
 */
int write_fpBuf_to_disk(int fd, INODE InodeTmp, int fd_index)
{
    int left = InodeTmp.i_size, i, disk_num;
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
        if (left <= BLOCKSIZE)
        {
            write_file(InodeTmp.dir_block[i], FilePointer, left);
            Write_Inode(fd, InodeTmp); /* store to disk */
            return 1;                  /* read success */
        }
        write_file(InodeTmp.dir_block[i], FilePointer, BLOCKSIZE);
        left -= BLOCKSIZE;
        FilePointer += BLOCKSIZE;
    }
    /* using indirect block1 */
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
    left = write_indir_block1(InodeTmp.indir_block_1, FilePointer, left);
    FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX);
    if (left == 0)
    {
        Write_Inode(fd, InodeTmp); /* store to disk */
        return 0;                  /* read complete */
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
    read_block(InodeTmp.indir_block_2, TmpBuf);
    memcpy(block2, TmpBuf, 1024);
    for (i = 0; i < INODE_DIRECT1_MAX; i++)
    {
        if (block2[i] == 0) /* allocate new space  */
        {
            disk_num = find_free_block();
            if (disk_num == -1)
            {
                printf("my_write Error:fail to allocate new block.\n");
                exit(1);
            }
            block2[i] = disk_num;
        }
        left = write_indir_block1(block2[i], FilePointer, left);
        FilePointer += (BLOCKSIZE * INODE_DIRECT1_MAX);/* add bias */
        if (left == 0)
        {
            Write_Inode_block(InodeTmp.indir_block_2, block2);
            Write_Inode(fd, InodeTmp); /* store to disk */
            return 1;                  /* read success */
        }
    }
    return 1;
}
