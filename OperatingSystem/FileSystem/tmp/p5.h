
#ifndef P5_H
#define P5_H

#define MAX_FILE_NAME_LENGTH 200
#define MAX_OPEN_FILES 10

/* file API */
extern int my_open (const char * path);
extern int my_creat (const char * path);
extern int my_read (int fd, void * buf, int count);
extern int my_write (int fd, const void * buf, int count);
extern int my_close (int fd);

extern int my_remove (const char * path);
extern int my_rename (const char * old, const char * new);
extern int my_mkdir (const char * path);
extern int my_rmdir (const char * path);

extern void my_mkfs ();

/* provided by the lower layer */

#define BLOCKSIZE 1024
/* not used in any declaration, just a reminder that each block is 1KB */
/* and may be useful inside the code. */
typedef char block [BLOCKSIZE];

extern int dev_open ();
extern int read_block (int block_num, char * block);
extern int write_block (int block_num, char * block);

#endif /* P5_H */

