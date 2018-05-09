#include <fcntl.h> /*open file*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*the max character*/
const int MAXLINE = 512;

/*the command buffer*/
char buf[MAXLINE];

/*the enum stand for different command*/
enum specify { NORMAL, OUT_REDIRECT, IN_REDIRECT, OUT_ADD, PIPE };

/*the index of shell Command History*/
int HistoryIndex;

/*store the command History*/
char* CommandHistory[MAXLINE + 1];

struct CommandInfomation {
    char* argv[512]; /*store the command after Parsing*/
    int argc;        /*the number of argv,split with space*/
    int index;       /*store the index of special character*/
    int background;  /*whether it is a background command*/
    enum specify type[50];
    int override; /* in case after < command has muti pipes */
    char* file;
};

void initStruct(struct CommandInfomation* a) {
    a->argc = 0;
    a->index = 0;
    a->background = 0;
    a->override = 0;
    a->file = NULL;
    memset(a->type, 0, sizeof(a->type));
}

/*new struct */
struct CommandInfomation CommandInfo;

void UpdateHistory();

void PrintCommand(int n);

void pipe_command();

void next_pipe(int pipefd[20][2], int pipenumber, int pointerindex[]);

/*
  Parse the command line with space and get the argv array
*/
void parseline();

int IgnoreCommand();

int ReviseCommand();

void SIG_IGN_handler(int sig);

void command();

void Execvp(char* arg0, char** arg);

void ChildCommand();
