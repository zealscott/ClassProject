

#include "block.c"
#include "blkFun.c"
#include <stdio.h>
// #include "p5.h"

/* open an exisiting file for reading or writing */
int my_open(char *path)
{
  clean_pathname();
  printf("my_open (%s) not implemented\n", path);
  return -1;
}

/* open a new file for writing only */
int my_creat(char *path)
{
  clean_pathname();
  printf("my_creat (%s) not implemented\n", path);
  return -1;
}

/* sequentially read from a file */
int my_read(int fd, void *buf, int count)
{
  clean_pathname();
  printf("my_read (%d, %x, %d) not implemented\n", fd, buf, count);
  return -1;
}

/* sequentially write to a file */
int my_write(int fd, void *buf, int count)
{
  clean_pathname();
  printf("my_write (%d, %x, %d) not implemented\n", fd, buf, count);
  return -1;
}

int my_close(int fd)
{
  clean_pathname();
  printf("my_close (%d) not implemented\n", fd);
  return -1;
}

int my_remove(char *path)
{
  clean_pathname();
  printf("my_remove (%s) not implemented\n", path);
  return -1;
}

int my_rename(char *old, char *new)
{
  clean_pathname();
  printf("my_remove (%s, %s) not implemented\n", old, new);
  return -1;
}

/* make a new dir
 * only works if all but the last component of the path already exists
 * return 0 success, otherwise -1
 */
int my_mkdir(char *path)
{
  int path_num, i, disk_num, parent_disk_num;
  char TmpBuf[BLOCKSIZE];
  DIR tmpDir;
  clean_pathname();
  path_num = parse_path(path);
  parent_disk_num = find_file(path_num - 1); /* find parent dir */
  if (parent_disk_num == -1)                 /* previous dir not existed */
    return -1;
  read_block(parent_disk_num, TmpBuf);
  memcpy(&tmpDir, TmpBuf, sizeof(tmpDir));
  for (i = 0; i < tmpDir.in_use; i++)
  {
    if (strcmp(PathName[path_num - 1], tmpDir.entry[i].name) == 0)
    {
      printf("Directionary exist.\n");
      return -1; /* dir existed */
    }
  }
  if (tmpDir.in_use < MAX_DIRENTRY)
  {
    /* allocate new block */
    int block_location = mk_new_dir(parent_disk_num);
    if (block_location == -1)
      return -1; /* not success */
    /* store dir name and i-node location */
    strcpy(tmpDir.entry[tmpDir.in_use].name, PathName[path_num - 1]);
    tmpDir.entry[tmpDir.in_use].i_node = block_location;
    tmpDir.in_use++;
    Write_Dir(parent_disk_num, tmpDir);
    printf("make dir success: %s i-node in: %d\n", PathName[path_num - 1], block_location);
    return 0;
  }
  return -1;
}

/* remove dir
 * remove parent dir entry, free space
 * only works when the dir is empty
 * return 0 success, otherwise -1
 */
int my_rmdir(char *path)
{
  int path_num, disk_num, dir_in_use, Parent_disk_num, i;
  char TmpBuf[BLOCKSIZE];
  DIR tmpDir, ParentTmpDir;
  clean_pathname();
  path_num = parse_path(path);
  disk_num = find_file(path_num);              /* find last dir */
  Parent_disk_num = find_file(path_num - 1);   /* find parent dir */
  if (disk_num == -1 || Parent_disk_num == -1) /*  dir not existed */
  {
    printf("Error: Directionary is not exist.\n");
    return -1;
  }

  /* read dir from disk */
  read_block(disk_num, TmpBuf);
  memcpy(&tmpDir, TmpBuf, sizeof(tmpDir));
  dir_in_use = tmpDir.in_use; /* how many file this dir contained */
  if (dir_in_use > 2)
  {
    printf("error: dir is not empty.\n");
    return -1; /* not empty */
  }

  /* need remove dir from parent dir, change superblock,bitmap */
  read_block(Parent_disk_num, TmpBuf);
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));
  for (i = 0; i < ParentTmpDir.in_use; i++)
  {
    if (strcmp(PathName[path_num - 1], ParentTmpDir.entry[i].name) == 0)
      break;
  }
  /* remove from parent dir */
  for (; i < (ParentTmpDir.in_use - 1); i++)
  {
    strcpy(ParentTmpDir.entry[i].name, ParentTmpDir.entry[i + 1].name);
  }
  ParentTmpDir.in_use--;
  Write_Dir(Parent_disk_num, ParentTmpDir); /* write back */
  /* reduce superblock,bitmap */
  free_space(disk_num);
  printf("rmdir : %s ,success.\n", PathName[path_num - 1]);
  return 0;
}

/* check to see if the device already has a file system on it,
 * and if not, create one.
 * superblock in No.1 block,
 * bitmap in 2~57 block,
 * root dir in 58 block
 */
void my_mkfs()
{
  char TmpBuf[BLOCKSIZE];
  BITMAP bitmapTmp;
  dev_open();                 /* open device,once only */
  for (int i = 0; i < 5; i++) /* init Path name */
    PathName[i] = (char *)malloc(50 * sizeof(char));
  if (read_block(1, TmpBuf) == 0) /* read first block */
  {
    memcpy(&SuperBlock, TmpBuf, sizeof(SuperBlock));
    if (SuperBlock.is_format != 1)
    {
      /* init Super Block */
      memset(&SuperBlock, 0, sizeof(SuperBlock));
      SuperBlock.is_format = 1;
      SuperBlock.free_space = (250000 - 58);
      for (int i = 2; i < BITMAP_NUM + BITMAP_START; i++)
        SuperBlock.bitmap_usage[i] = 0;
      SuperBlock.bitmap_usage[2] = 58; /* bitmap 2 has used 58 block */
      Write_Superblk(SuperBlock);
      /* init bit map */
      for (int i = 2; i <= 57; i++)
      {
        if (read_block(i, TmpBuf) == 0)
        {
          memcpy(&bitmapTmp, TmpBuf, sizeof(bitmapTmp));
          bitmapTmp.start_block = (i - 2) * MAX_PER_BITMAP + 1;
          bitmapTmp.free_block = 450; /* all free */
          memset(bitmapTmp.bitmap, 0,
                 sizeof(bitmapTmp.bitmap)); /* set bitmap zero(unused) */
          if (i == 2)
          {
            /* first 58 block has used*/
            for (int j = 0; j <= 57; j++)
              bitmapTmp.bitmap[j] = 1;
            bitmapTmp.free_block = 392; /* used 58 blocks */
          }
          Write_Bitmap(i, bitmapTmp);
        }
        else
          exit(1);
      }
      /* init root dir block */
      if (read_block(58, TmpBuf) == 0)
      {
        memcpy(&RootDir, TmpBuf, sizeof(RootDir));
        strcpy(RootDir.entry[0].name, ".");
        strcpy(RootDir.entry[1].name, "..");
        RootDir.entry[0].i_node = 58; /* root i-node is in No.59 block */
        RootDir.entry[1].i_node = 58;
        RootDir.in_use = 2; /* just . and .. */
        Write_Dir(58, RootDir);
      }
      else
        exit(1);
    }
    else
    {
      read_block(58, TmpBuf);
      memcpy(&RootDir, TmpBuf, sizeof(RootDir));
      /* read root dir from disk */
      printf("the device already has a file system!\n");
    }
  }
  else
    exit(1);
}
