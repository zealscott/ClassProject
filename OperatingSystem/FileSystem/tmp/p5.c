

#include "block.c"
#include "blkFun.c"
#include <stdio.h>
// #include "p5.h"

/* open an exisiting file for reading or writing 
 * if exist, return -1
 * success, return file pointer
 */
int my_open(char *path)
{
  int path_num, i, disk_num;
  char TmpBuf[BLOCKSIZE];
  DIR ParentTmpDir;
  clean_pathname();
  path_num = parse_path(path);
  disk_num = find_file(path_num);
  /* store into fp list */
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == 0 || OpenFP[i] == disk_num) /* find an empty fp or has existed */
    {
      OpenFP[i] = disk_num;
      break;
    }
  }
  return disk_num;
}

/* open a new file for writing only 
 * if exist, return -1
 * success, return file pointer
 */
int my_creat(char *path)
{
  int path_num, i, disk_num, parent_disk_num, dir_in_use;
  char TmpBuf[BLOCKSIZE];
  DIR ParentTmpDir;
  clean_pathname();
  path_num = parse_path(path);
  parent_disk_num = find_file(path_num - 1); /* find parent dir */
  if (parent_disk_num == -1)                 /* previous dir not existed */
  {
    printf("parent Directionary is not exist.\n");
    return -1;
  }
  read_block(parent_disk_num, TmpBuf);
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));
  dir_in_use = ParentTmpDir.in_use;
  if (dir_in_use >= MAX_DIRENTRY)
  {
    printf("my_creat Error: cannot create more dir entry.\n");
    return -1;
  }
  for (i = 0; i < dir_in_use; i++)
  {
    if (strcmp(PathName[path_num - 1], ParentTmpDir.entry[i].name) == 0)
    {
      printf("my_creat Error: file exist.\n");
      return -1;
    }
  }
  /* create i-node for new file */
  disk_num = mk_new_inode();
  /* modify dir,set inode and in_use */
  strcpy(ParentTmpDir.entry[dir_in_use].name, PathName[path_num - 1]);
  ParentTmpDir.entry[dir_in_use].i_node = disk_num;
  ParentTmpDir.in_use++;
  Write_Dir(parent_disk_num, ParentTmpDir);
  printf("my_creat %s success: i-node in %d\n", PathName[path_num - 1], disk_num);
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == 0 || OpenFP[i] == disk_num)
    {
      OpenFP[i] = disk_num;
      break;
    }
  }
  return disk_num; /* not found  */
}

/* sequentially read from a file according to fd(i-node)
 * read bytes by btyes
 * if read success, return 0
 */
int my_read(int fd, void *buf, int count)
{
  /* fp_index is the buf index to read in DRAM*/
  int i, fp_index = -1;
  INODE InodeTmp;
  char TmpBuf[BLOCKSIZE];
  clean_pathname();
  if (fd <= 0)
  {
    printf("my_read Error: bad file descriptor.\n");
    return -1;
  }
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == fd)
    {
      fp_index = i;
      break;
    }
  }
  if (fp_index == -1)
    return -1; /* the file is not open */
  /* read from buffer */
  if (read_from_fpBuf(fp_index, count, buf) == count)
    return count;
  /* no buffer yet */
  read_block(fd, TmpBuf); /* read i-node from disk */
  memcpy(&InodeTmp, TmpBuf, sizeof(InodeTmp));
  OpenFP_buffer[fp_index] = (char *)malloc(25000);
  OpenFP_buffer_old[fp_index] = OpenFP_buffer[fp_index];
  /* read file from disk to DRAM */
  read_to_fpBuf(InodeTmp, fp_index);

  read_from_fpBuf(fp_index, count, buf);
  return count;
}

/* write string to DRAM file
 * store the file pointer and char *
 */
int my_write(int fd, void *buf, int count)
{
  /* left is the left unwrite bytes */
  /* index is the buf index to write */
  int i, fp_index, flag = 0;
  INODE InodeTmp;
  char TmpBuf[BLOCKSIZE];
  clean_pathname();
  if (fd <= 0)
  {
    printf("my_write Error: bad file descriptor.\n");
    return -1;
  }
  read_block(fd, TmpBuf); /* read i-node from disk */
  memcpy(&InodeTmp, TmpBuf, sizeof(InodeTmp));
  if (InodeTmp.i_size != 0)
    flag = 1;

  /* write to DRAM buffer */
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == fd)
    {
      fp_index = i;
      break;
    }
  }
  if (OpenFP_buffer[fp_index] == 0)
  {
    if (flag == 1)
    { /* buffer not in DRAM, but has file, first read */
      my_read(fd, buf, count);
    }
    else
    { /* buffer not in DRAM, no file */
      OpenFP_buffer[fp_index] = (char *)malloc(25000);
      OpenFP_buffer_old[fp_index] = OpenFP_buffer[fp_index];
    }
  }
  memcpy(OpenFP_buffer[fp_index], buf, count);
  OpenFP_buffer[fp_index] += count;
  InodeTmp.i_size = InodeTmp.i_size > count ? InodeTmp.i_size : count;
  Write_Inode(fd, InodeTmp);
  return count;
}

int my_close(int fd)
{
  int i, fp_index;
  INODE InodeTmp;
  char TmpBuf[BLOCKSIZE];
  clean_pathname();
  read_block(fd, TmpBuf);
  memcpy(&InodeTmp, TmpBuf, sizeof(InodeTmp));
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == fd)
    {
      fp_index = i;
      break;
    }
  }
  write_fpBuf_to_disk(fd, InodeTmp, fp_index);
  free(OpenFP_buffer_old[fp_index]);
  for (i = 0; i < 50; i++)
  {
    if (OpenFP[i] == fd)
    {
      OpenFP[i] = 0;
      OpenFP_buffer[i] = 0;
      OpenFP_buffer_old[i] = 0;
      break;
    }
  }
  return 0;
}

/* remove file from disk
 * if not exist, return -1
 * free ParentDir, inode, and data
 */
int my_remove(char *path)
{
  int path_num, parent_disk_num, i, j;
  char TmpBuf[BLOCKSIZE];
  DIR ParentTmpDir;
  clean_pathname();
  path_num = parse_path(path);
  parent_disk_num = find_file(path_num - 1); /* find parent dir */
  if (parent_disk_num == -1)                 /* previous dir not existed */
  {
    printf("parent Directionary is not exist.\n");
    return -1;
  }
  read_block(parent_disk_num, TmpBuf);
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));
  for (i = 0; i < ParentTmpDir.in_use; i++)
  {
    if (strcmp(PathName[path_num - 1], ParentTmpDir.entry[i].name) == 0)
    {
      /* free inode and its data block */
      free_inode(ParentTmpDir.entry[i].i_node);
      /* clean parent Directionary */
      for (j = i; j < (ParentTmpDir.in_use - 1); j++)
      {
        strcpy(ParentTmpDir.entry[j].name, ParentTmpDir.entry[j + 1].name);
      }
      /* free parent dir */
      ParentTmpDir.in_use--;
      Write_Dir(parent_disk_num, ParentTmpDir);
      printf("my_remove: remove %s success.\n", PathName[path_num - 1]);
      return 0; /* success */
    }
  }
  return -1; /* not found  */
}

/* rename a file
 * support move entitiy
 * if file not exist, return -1
 */
int my_rename(char *old, char *new)
{
  int old_path_num, new_path_num;
  int old_parent_dir_num, new_parent_dir_num, i, j, inode_disk_num;
  DIR ParentTmpDir;
  char TmpBuf[BLOCKSIZE];
  clean_pathname();
  /* find i-node */
  old_path_num = parse_path(old);
  old_parent_dir_num = find_file(old_path_num - 1);
  if (old_parent_dir_num == -1)
  {
    printf("parent Directionary is not exist.\n");
    return -1;
  }
  read_block(old_parent_dir_num, TmpBuf);
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));
  for (i = 0; i < ParentTmpDir.in_use; i++)
  {
    if (strcmp(PathName[old_path_num - 1], ParentTmpDir.entry[i].name) == 0)
    {
      inode_disk_num = ParentTmpDir.entry[i].i_node;
      /* clean dir */
      for (j = i; j < (ParentTmpDir.in_use - 1); j++)
      {
        strcpy(ParentTmpDir.entry[j].name, ParentTmpDir.entry[j + 1].name);
      }
      /* free parent dir */
      ParentTmpDir.in_use--;
      Write_Dir(old_parent_dir_num, ParentTmpDir);
    }
  }
  /* modify new dir */
  clean_pathname();
  new_path_num = parse_path(new);
  new_parent_dir_num = find_file(new_path_num - 1);
  if (new_parent_dir_num == -1)
  {
    printf("parent Directionary is not exist.\n");
    return -1;
  }
  read_block(new_parent_dir_num, TmpBuf);
  memset(&ParentTmpDir,0,sizeof(ParentTmpDir));
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));

  strcpy(ParentTmpDir.entry[ParentTmpDir.in_use].name, PathName[new_path_num - 1]);
  ParentTmpDir.entry[ParentTmpDir.in_use].i_node = inode_disk_num;
  ParentTmpDir.in_use++;
  Write_Dir(new_parent_dir_num, ParentTmpDir);
  printf("my_rename success: rename to %s\n",PathName[new_path_num - 1]);
  return 0;
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
  {
    printf("my_mkdir Error: parent Directionary is not exist.\n");
    return -1;
  }
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
    printf("make dir success: %s dir disk num in: %d\n", PathName[path_num - 1], block_location);
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
  int path_num, disk_num, dir_in_use, Parent_disk_num, i, j;
  char TmpBuf[BLOCKSIZE];
  DIR tmpDir, ParentTmpDir;
  clean_pathname();
  path_num = parse_path(path);
  disk_num = find_file(path_num);              /* find last dir */
  Parent_disk_num = find_file(path_num - 1);   /* find parent dir */
  if (disk_num == -1 || Parent_disk_num == -1) /*  dir not existed */
  {
    printf("rmdir Error: Directionary is not exist.\n");
    return -1;
  }

  /* read dir from disk */
  read_block(disk_num, TmpBuf);
  memcpy(&tmpDir, TmpBuf, sizeof(tmpDir));
  dir_in_use = tmpDir.in_use; /* how many file this dir contained */
  if (dir_in_use > 2)
  {
    printf("rmdir Error: dir is not empty.\n");
    return -1; /* not empty */
  }

  /* need remove dir from parent dir, change superblock,bitmap */
  read_block(Parent_disk_num, TmpBuf);
  memcpy(&ParentTmpDir, TmpBuf, sizeof(ParentTmpDir));
  for (i = 0; i < ParentTmpDir.in_use; i++)
  {
    if (strcmp(PathName[path_num - 1], ParentTmpDir.entry[i].name) == 0) /* remove from parent dir */
    {
      for (j = i; j < (ParentTmpDir.in_use - 1); j++)
      {
        strcpy(ParentTmpDir.entry[j].name, ParentTmpDir.entry[j + 1].name);
      }
      ParentTmpDir.in_use--;
      Write_Dir(Parent_disk_num, ParentTmpDir); /* write back */
      /* reduce superblock,bitmap */
      free_space(disk_num);
      printf("rmdir : %s ,success.\n", PathName[path_num - 1]);
      return 0;
    }
  }
  return -1; /* not found */
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
