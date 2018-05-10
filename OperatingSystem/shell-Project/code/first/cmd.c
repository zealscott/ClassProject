#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include <fcntl.h> /*open file*/


extern const int  MAXLINE ;

extern char buf[512];

extern void UpdateHistory();

extern void command();


void SIG_IGN_handler(int sig)  
{
	waitpid(-1, NULL, 0);
	return;
}


int main() {
	/*Command line*/
	signal(SIGCHLD, SIG_IGN_handler);

	while (1) {

		printf("cmd >");
		/*set buf to empty*/
		memset(buf, 0, sizeof(buf));
		/*Read from keyboard*/
		fgets(buf, MAXLINE, stdin);

		/*The function feof() tests the end-of-file indicator
		for the stream pointed to by stream,
		returning non-zero if it is set. */
		if (feof(stdin))
			exit(0);
		/*update the command history*/
		UpdateHistory();
		/*command exceve*/
		command();
	}
	return 0;/*used*/
}
