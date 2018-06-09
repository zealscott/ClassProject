

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "p5.h"

/* only open the file once */
static int fd = -1;
static int devsize = 0;

/* returns the device size (in blocks) if the operation is successful,
 * and -1 otherwise */
int dev_open ()
{
  struct stat st;

  if (fd < 0) {
    fd = open ("simulated_device", O_RDWR);
    if (fd < 0) {
      perror ("open");
      return -1;
    }
    if (fstat (fd, &st) < 0) {
      perror ("fstat");
      return -1;
    }
    devsize = st.st_size / BLOCKSIZE;
  }
  return devsize;
}

/* returns 0 if the operation is successful, and -1 otherwise */
int read_block (int block_num, char * block)
{
  if (block_num >= devsize) {
    printf ("block number requested %d, maximum %d", block_num, devsize - 1);
    return -1;
  }
  if (lseek (fd, block_num * BLOCKSIZE, SEEK_SET) < 0) {
    perror ("lseek");
    return -1;
  }
  if (read (fd, block, BLOCKSIZE) != BLOCKSIZE) {
    perror ("read");
    return -1;
  }
  return 0;
}

/* returns 0 if the operation is successful, and -1 otherwise */
int write_block (int block_num, char * block)
{
  if (block_num >= devsize) {
    printf ("block number requested %d, maximum %d", block_num, devsize - 1);
    return -1;
  }
  if (lseek (fd, block_num * BLOCKSIZE, SEEK_SET) < 0) {
    perror ("lseek");
    return -1;
  }
  if (write (fd, block, BLOCKSIZE) != BLOCKSIZE) {
    perror ("write");
    return -1;
  }
  if (fsync (fd) < 0)
    perror ("fsync");		/* but return success anyway */
  return 0;
}

