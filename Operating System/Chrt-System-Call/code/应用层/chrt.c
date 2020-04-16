#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int chrt(long deadline)
{
  struct timespec time;
  message m;
  memset(&m, 0, sizeof(m));

  alarm((unsigned int)deadline);/* set the alarm */

  if (deadline > 0)
  {
    clock_gettime(CLOCK_REALTIME, &time);
    deadline = time.tv_sec + deadline;
  }
  m.m2_l1 = deadline; /* store the deadline */

  return (_syscall(PM_PROC_NR, PM_CHRT, &m));
}

/* #define PM_PROC_NR   ((endpoint_t) 0)	process manager */
