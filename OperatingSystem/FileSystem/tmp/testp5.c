#include <stdio.h>
#include <stdlib.h>
#include "p5.h"

char buffer0 [] = "hello world";
char buffer1 [1000];		/* one thousand */
char buffer2 [10000];		/* ten thousand */
char buffer3 [100000];		/* a hundred thousand */
char buffer4 [1000000];		/* a million */
char buffer5 [50 * 1024 * 1024]; /* max size, 50MB */

static int test_file (char * path, char * buffer, int size, int max_size);
static void close_remove_file (char * path, int fd);

int main (int argc, char ** argv)
{
  int first_test = 1;
  int fd0, fd1, fd2, fd3, fd4, fd5;
  int i;
  int max_size = sizeof (buffer5);

  if ((argc > 1) && (atoi (argv [1]) > 0))
    max_size = atoi (argv [1]);

  printf ("testing files up to %d bytes\n", max_size);
  my_mkfs ();
  if (my_mkdir ("/foo") < 0) {  /* already tested before */
    printf ("second or subsequent test\n");
    first_test = 0;
  }
  if (my_rmdir ("/foo") < 0) {
    if (first_test) {
      printf ("unable to rmdir /foo\n");
      exit (1);
    }
  }
  if (! first_test) printf ("succeeded in removing directory /foo\n");
  if (my_mkdir ("/foo") < 0) {  /* already tested before */
    printf ("unable to mkdir /foo, aborting\n");
    exit (1);
  }
  printf ("succeeded in creating directory /foo\n");
  if (my_open ("/foo/bar") >= 0) {	/* file should not exist */
    printf ("error, opened nonexistent file /foo/bar, aborting\n");
    exit (1);
  }
  printf ("my_open correctly failed to open non-existent file /foo/bar\n");

  fd0 = test_file ("/foo/bar0", buffer0, sizeof (buffer0), max_size);
  if (fd0 >= 0)
    if (my_close (fd0) < 0) {
      printf ("error, unable to close /foo/bar0, aborting\n");
      exit (1);
    }
    else
      printf ("successfully closed /foo/bar0\n");

  /* some directory operations */
  if (my_rename ("/foo/bar0", "/qqq") < 0) {
    printf ("error, unable to rename /foo/bar, aborting\n");
    exit (1);
  }
  printf ("successfully renamed /foo/bar0 to /qqq\n");
  if (my_remove ("/qqq") < 0) {
    printf ("error, unable to remove /qqq, aborting\n");
    exit (1);
  }
  printf ("successfully removed /qqq\n");
  if (my_mkdir ("/foo/bar") < 0) {
    printf ("unable to create subdirectory /foo/bar, aborting\n");
    exit (1);
  }
  printf ("successfully created directory /foo/bar\n");
  if (my_rmdir ("/foo/bar") < 0) {
    printf ("unable to remove subdirectory /foo/bar, aborting\n");
    exit (1);
  }
  printf ("successfully removed directory /foo/bar\n");

  /* repeat the test on as many of the larger files as appropriate */
  fd1 = test_file ("/foo/bar1", buffer1, sizeof (buffer1), max_size);
  fd2 = test_file ("/foo/bar2", buffer2, sizeof (buffer2), max_size);
  fd3 = test_file ("/foo/bar3", buffer3, sizeof (buffer3), max_size);
  fd4 = test_file ("/foo/bar4", buffer4, sizeof (buffer4), max_size);
  fd5 = test_file ("/foo/bar5", buffer5, sizeof (buffer5), max_size);

  close_remove_file ("/foo/bar5", fd5);
  close_remove_file ("/foo/bar2", fd2);
  close_remove_file ("/foo/bar4", fd4);
  close_remove_file ("/foo/bar1", fd1);
  close_remove_file ("/foo/bar3", fd3);

  printf ("tests completed successfully\n");
  exit (0);
}

static int test_file (char * path, char * buffer, int size, int max_size)
{
  int i, large, fd;

  if (size > max_size)		/* skip larger tests */
    return -1;
  large = (size > 20);		/* small file is "hello world" */

  /* create a file, check that it was saved */
  if (large) {
    for (i = 0; i < size; i++)
      buffer [i] = i % 128;
    buffer [0] = 0x77;		/* change value in location 0 */
    if (size / 2 > 1025)
      buffer [1025] = 0x99;	/* change a value after the first block */
    if (size / 2 > 1024 * 256 + 21)
      buffer [1024 * 256 + 21] = 0x42; /* after first block of blocks */
  }

  if ((fd = my_creat (path)) < 0) { /* file should not exist */
    printf ("error, unable to recreate %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully created file %s\n", path);
  if (my_write (fd, buffer, size) != size) {
    printf ("error, unable to write %d bytes to %s, aborting\n", size, path);
    exit (1);
  }
  printf ("successfully wrote %d bytes to file %s\n", size, path);
  if (my_close (fd) < 0) {
    printf ("error, unable to close %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully closed file %s\n", path);

  /* reset the buffer contents to the values that are easy to check */
  if (large) {
    buffer [0] = 0;
    if (size / 2 > 1025)
      buffer [1025] = 1025 % 128;
    if (size / 2 > 1024 * 256 + 21)
      buffer [1024 * 256 + 21] = (1024 * 256 + 21) % 128;
  }

  /* now read and write */
  if ((fd = my_open (path)) < 0) { /* file should exist now */
    printf ("error, unable to reopen %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully reopened file %s\n", path);
  if (my_write (fd, buffer, size / 2) != size / 2) {
    printf ("error, unable to rewrite part of %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully wrote initial %d bytes to file %s\n", size / 2, path);
  /* clear top part of buffer */
  for (i = size / 2; i < size; i++)
    buffer [i] = 0;
  /* now replace it with the contents of what we read */
  if (my_read (fd, buffer + size / 2, size - size / 2) != size - size / 2) {
    printf ("error, read failed for %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully read final %d bytes from file %s\n",
	  size - size / 2, path);
  if (my_close (fd) < 0) {
    printf ("error, unable to close %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully closed file %s after reading\n", path);

  /* clear the bottom half of the buffer, and read it from the file again */
  for (i = 0; i < size / 2; i++)
    buffer [i] = 0;
  if ((fd = my_open (path)) < 0) {
    printf ("error, unable to re-reopen %s, aborting\n", path);
    exit (1);
  }
  printf ("successfully re-reopened file %s\n", path);
  if (my_read (fd, buffer, size / 2) != size / 2) {
    printf ("error, read failed for initial %d bytes of %s, aborting\n",
	    size / 2, path);
    exit (1);
  }
  printf ("successfully read initial %d bytes from file %s\n", size / 2, path);

  if (large) {
    for (i = 0; i < size; i++) {
      if (((buffer [i]) & 0xff) != ((i % 128) & 0xff)) {
        printf ("error at index %d (of %d), value %d, expected %d\n",
	        i, size, ((buffer [i]) & 0xff), ((i % 128) & 0xff));
        exit (1);
      }
    }
  } else {
    if (strcmp (buffer, "hello world") != 0) {
      printf ("error, value written was 'hello world', value returned %s\n",
	      buffer);
      exit (1);
    }
  }
  printf ("test completed successfully on a file of size %d\n", size);
  return fd;
}

static void close_remove_file (char * path, int fd)
{
  if (fd >= 0) {
    if (my_close (fd) < 0) {
      printf ("error, unable to close %s, aborting\n", path);
      exit (1);
    }
    printf ("successfully closed %s\n", path);
    if (my_remove (path) < 0) {
      printf ("error, unable to remove %s, aborting\n", path);
      exit (1);
    }
    printf ("successfully removed %s\n", path);
  }

}
