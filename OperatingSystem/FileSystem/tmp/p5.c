

#include <stdio.h>
#include "blkFun.c"
#include "block.c"
// #include "p5.h"


SUPERBLK SuperBlock; /* superblock will be used frequently */

DIRENTRY RootDir[MAX_DIRENTRY]; /* Root Dir will be used frequently, max 18 entry */

INODE tmpInode;

/* open an exisiting file for reading or writing */
int my_open(char *path)
{
  printf("my_open (%s) not implemented\n", path);
  return -1;
}

/* open a new file for writing only */
int my_creat(char *path)
{
  printf("my_creat (%s) not implemented\n", path);
  return -1;
}

/* sequentially read from a file */
int my_read(int fd, void *buf, int count)
{
  printf("my_read (%d, %x, %d) not implemented\n", fd, buf, count);
  return -1;
}

/* sequentially write to a file */
int my_write(int fd, void *buf, int count)
{
  printf("my_write (%d, %x, %d) not implemented\n", fd, buf, count);
  return -1;
}

int my_close(int fd)
{
  printf("my_close (%d) not implemented\n", fd);
  return -1;
}

int my_remove(char *path)
{
  printf("my_remove (%s) not implemented\n", path);
  return -1;
}

int my_rename(char *old, char *new)
{
  printf("my_remove (%s, %s) not implemented\n", old, new);
  return -1;
}

/* only works if all but the last component of the path already exists */
int my_mkdir(char *path)
{
  printf("my_mkdir (%s) not implemented\n", path);
  return -1;
}

int my_rmdir(char *path)
{
  printf("my_rmdir (%s) not implemented\n", path);
  return -1;
}

/* check to see if the device already has a file system on it,
 * and if not, create one. */
void my_mkfs()
{
  char TmpBuf[BLOCKSIZE];
  BITMAP *bitmapPointer = NULL;
  dev_open();                     /* open device,once only */
  if (read_block(1, TmpBuf) == 0) /* read first block */
  {
    memcpy(&SuperBlock, TmpBuf, sizeof(SuperBlock));
    if (SuperBlock.is_format != 1)
    {
      /* init Super Block */
      memset(&SuperBlock, 0, sizeof(SuperBlock));
      SuperBlock.is_format = 1;
      SuperBlock.next_free_block = 59; /* 1-58 will be bitmap,root dir */
      /* init bit map */
      for (int i = 2; i <= 57; i++)
      {
        if (read_block(i, TmpBuf) == 0)
        {
          bitmapPointer = (BITMAP *)TmpBuf;
          bitmapPointer->start_block = (i - 2) * MAX_PER_BITMAP + 1;
          memset(bitmapPointer->bitmap, 0, sizeof(bitmapPointer->bitmap)); /* set bitmap zero(unused) */
          if (i == 2)
          {
            /* first 59 block has used*/
            for (int j = 0; j <= 58; j++)
              bitmapPointer->bitmap[j] = 1;
          }
        }
        else
          exit(1);
      }
      /* init root dir block */
      if (read_block(58, TmpBuf) == 0)
      {
        memcpy(&RootDir, TmpBuf, sizeof(RootDir));
        strcpy(RootDir[0].name, ".");
        strcpy(RootDir[1].name, "..");
        RootDir[0].i_node = 59; /* root i-node is in No.59 block */
        RootDir[1].i_node = 59;
      }
      else
        exit(1);
      /* init root i-node */
      if (read_block(59, TmpBuf) == 0)
      {
        memcpy(&tmpInode, TmpBuf, sizeof(tmpInode)); /* get i-node */
        memset(&tmpInode, 0, sizeof(tmpInode));
        tmpInode.ctime = time(NULL);                 /* change create time */
        memcpy(TmpBuf, &tmpInode, sizeof(tmpInode)); /* write back */
        write_block(59, TmpBuf);
      }
      else
        exit(1);
      printf("init file system success!\n");
    }
    else
    {
      printf("the device already has a file system!\n");
    }
  }
  else
    exit(1);
}
