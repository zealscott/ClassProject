#include "varibles.h"
#include <fcntl.h> /*open file*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*update command history*/
void UpdateHistory() {
    char* temp;
    if (strcmp(buf, "\n") == 0)
        return;
    if (HistoryIndex > MAXLINE)
        HistoryIndex = 0;
    temp = (char*)malloc(sizeof(buf));
    strcpy(temp, buf);
    CommandHistory[HistoryIndex++] = temp;
    return;
}

/*print the command with n lines*/
void PrintCommand(int n) {
    int i, j = 0;
    if (n == -1) {
        for (i = 0; i < HistoryIndex; i++)
            printf("the %d command: %s\n", i, CommandHistory[i]);
    } else {
        if (n > HistoryIndex) {
            printf("Warning: the argument is too large.\n");
            return;
        }
        for (i = HistoryIndex - n; i < HistoryIndex; i++)
            printf("the %d command: %s\n", ++j, CommandHistory[i]);
    }
}

/*
  Parse the command line with space and get the argv array
*/
void parseline() {
    initStruct(&CommandInfo);

    buf[strlen(buf) - 1] = ' '; /*Replace trailing '\n' with space*/

    /*split buf with space*/
    char* token = strtok(buf, " ");
    while (token) {
        CommandInfo.argv[CommandInfo.argc++] = token;
        token = strtok(NULL, " ");
    }
    /*set the last command with NULL*/
    CommandInfo.argv[CommandInfo.argc] = NULL;

    /*empty command line*/
    if (CommandInfo.argc == 0)
        return;

    /*indicate whether its background Command*/
    CommandInfo.background = (*(CommandInfo.argv[CommandInfo.argc - 1]) == '&');
    if (CommandInfo.background)
        CommandInfo.argv[--CommandInfo.argc] = NULL;
    return;
}

/*if return 1, ignore the command*/
int IgnoreCommand() {
    /*if no command,continue*/
    if (CommandInfo.argc < 1)
        return 1;

    /*exit command*/
    if (strcmp(CommandInfo.argv[0], "exit") == 0)
        exit(0);

    /*history command*/
    if (strcmp(CommandInfo.argv[0], "history") == 0) {
        if (CommandInfo.argc == 1)
            /*print all the history*/
            PrintCommand(-1);
        else {
            PrintCommand(atoi(CommandInfo.argv[1])); /*convert string to int*/
        }
        return 1;
    }

    /*cd command to change directory*/
    if (strcmp(CommandInfo.argv[0], "cd") == 0) {
        if (CommandInfo.argc > 1) {
            if (chdir(CommandInfo.argv[1]) == -1) {
                printf("error directory!\n");
            }
        }
        return 1;
    }

    /*wrong command*/
    if (strcmp(CommandInfo.argv[CommandInfo.argc - 1], "<") == 0 ||
        strcmp(CommandInfo.argv[CommandInfo.argc - 1], ">") == 0 ||
        strcmp(CommandInfo.argv[CommandInfo.argc - 1], "|") == 0) {
        printf("Error:command error\n");
        return 1;
    }

    return 0;
}

int ReviseCommand() {
    /*
    if the command is empty or exit or cd or history, should ignore the command;
    */
    if (IgnoreCommand())
        return -1;

    int i, override = 0;

    /*search the command with special charactors,and store the file and type*/
    for (i = 0; i < CommandInfo.argc; i++) {
        if (strcmp(CommandInfo.argv[i], "<") == 0) {
            CommandInfo.argv[i] = NULL;
            CommandInfo.file = CommandInfo.argv[i + 1];
            CommandInfo.type[CommandInfo.index++] = IN_REDIRECT;
            override = 1;

        } else if (strcmp(CommandInfo.argv[i], ">") == 0) {
            /* if > is not the first command, should not set the file */
            CommandInfo.argv[i] = NULL;
            if (!override)
                CommandInfo.file = CommandInfo.argv[i + 1];
            CommandInfo.type[CommandInfo.index++] = OUT_REDIRECT;
            break;

        } else if (strcmp(CommandInfo.argv[i], ">>") == 0) {
            CommandInfo.argv[i] = NULL;
            if (!override)
                CommandInfo.file = CommandInfo.argv[i + 1];
            CommandInfo.type[CommandInfo.index++] = OUT_ADD;
            break;

        }
        /*multi - PIPE*/
        else if (strcmp(CommandInfo.argv[i], "|") == 0) {
            CommandInfo.type[CommandInfo.index++] = PIPE;
            CommandInfo.argv[i] = NULL;
        }
    }
    return 1;
}

void Execvp(char* arg0, char** arg) {
    if (execvp(arg0, arg) < 0) {
        printf("%s:Command not found\n", arg0);
        exit(0);
    }
}

/*special run for mult-pipes*/
void next_pipe(int pipefd[20][2], int pipenumber, int pointerindex[]) {
    char** P_temp;
    char buffer[MAXLINE];
    int commandnumber = CommandInfo.index; /*get the all command number*/
    int i = 1, pid, fd, run_index = 2; /*refer to the index of command to run*/
    int flag = !(pipenumber + 1 == commandnumber);
    int lastCommand = 0;

    /*if only one pipe and no other command, print it*/
    if (commandnumber == 1) {
        read(pipefd[1][0], buffer, MAXLINE);
        printf("%s", buffer);
        return;
    }

    /*loop to connect the pipe*/

    while (pipenumber--) {
        pid = fork();
        if (pid == 0) {
            /*this is the child, to run the command*/
            dup2(pipefd[i][0], 0);     /*read data*/
            close(pipefd[i][1]);       /*no need to write*/
            dup2(pipefd[i + 1][1], 1); /*write to the next pipe*/
            /*get the command*/

            P_temp = CommandInfo.argv + pointerindex[run_index];
            Execvp(P_temp[0], P_temp);
        } else {
            /*wait child completes the command*/
            waitpid(pid, NULL, 0);
            i++;         /*next pipe*/
            run_index++; /*next command*/
        }
    }

    /*if the last command is > or >>*/
    if (flag) {
        if (CommandInfo.override)
            lastCommand = CommandInfo.type[commandnumber];
        else
            lastCommand = CommandInfo.type[commandnumber - 1];
        /*if the command is >*/
        if (lastCommand == OUT_REDIRECT) {
            fd = open(CommandInfo.argv[pointerindex[commandnumber]],
                      O_WRONLY | O_CREAT | O_TRUNC, 0666);
        } else { /*the command is >>*/
            fd = open(CommandInfo.argv[pointerindex[commandnumber]],
                      O_RDWR | O_APPEND, 0666);
        }
        read(pipefd[i][0], buffer, MAXLINE); /*read from pipe*/
        write(fd, buffer, strlen(buffer));   /*write into file*/
    } else {
        read(pipefd[i][0], buffer, MAXLINE); /*read from pipe*/
        fprintf(stdout,"%s", buffer);                /*print into screen*/
    }
    return;
}

/*take care of pipe*/
void pipe_command() {
    /*pointerindex is the index of each Command*/
    int i = 1, j = 0, pointerindex[20], commandnumber = CommandInfo.index;
    int pipenumber = 0, pid, pipefd[20][2];
    char** P_temp;
    pointerindex[0] = 0; /*the first command location*/

    /*get all command's index*/
    while (i <= commandnumber) {
        if (CommandInfo.argv[j] == NULL) {
            pointerindex[i] = j + 1;
            i++;
        }
        j++;
    }

    /*if the last command is > or >>*/
    //     pipenumber -= 1;

    for (i = 0; i <= CommandInfo.index; i++) {
        if (CommandInfo.type[i] == PIPE)
            pipenumber++;
    }

    /*init the pipe*/
    for (i = 0; i < 20; i++)
        pipe(pipefd[i]);

    i = 1;

    if (fork() != 0) {
        /*this is the father, need to read*/
        dup2(pipefd[0][0], 0);
        close(pipefd[0][1]);

        P_temp = CommandInfo.argv + pointerindex[1];
        /*fork a process*/
        pid = fork();

        if (pid != 0) {
            /*this is the father,wait child return*/
            waitpid(pid, NULL, 0);
            close(pipefd[1][1]); /*no need to write*/
            next_pipe(pipefd, pipenumber - 1, pointerindex);
        } else {
            /*read the result from pipe and write to the next pipe*/
            dup2(pipefd[1][1], 1);
            close(pipefd[1][0]); /*no need to read*/
            Execvp(P_temp[0], P_temp);
        }
    } else {
        /*this is the child, need to write*/
        dup2(pipefd[0][1], 1);
        close(pipefd[0][0]);
        Execvp(CommandInfo.argv[0], CommandInfo.argv);
    }
}

void EditInfo() {
    int i, location;
    CommandInfo.index--;
    /* find the first command -- NULL */
    for (i = 1; i < CommandInfo.argc; i++) {
        if (CommandInfo.argv[i] == NULL) {
            location = i;
            break;
        }
    }
    /* override the used command*/
    for (i = location; i < CommandInfo.argc; i++) {
        CommandInfo.argv[i] = CommandInfo.argv[i + 2];
    }
    CommandInfo.override = 1;
}

/*
        the >  command must be the last command,
        so | command is ahead
*/
void ChildCommand() {
    int fd;
    switch (CommandInfo.type[0]) {
        case NORMAL:
            Execvp(CommandInfo.argv[0], CommandInfo.argv);
            break;

        case IN_REDIRECT: /* < command*/
            fd = open(CommandInfo.file, O_RDONLY);
            if (fd == -1) {
                printf("Error: wrong input!\n");
                break;
            }
            dup2(fd, STDIN_FILENO);

            if (CommandInfo.type[1] == PIPE) {
                EditInfo();
                pipe_command();
            }
            Execvp(CommandInfo.argv[0], CommandInfo.argv);
            break;

        case OUT_REDIRECT: /* > command*/
            fd = open(CommandInfo.file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd, STDOUT_FILENO);
            Execvp(CommandInfo.argv[0], CommandInfo.argv);
            break;

        case OUT_ADD: /* >> command*/
            fd = open(CommandInfo.file, O_WRONLY | O_APPEND | O_CREAT, 0666);
            dup2(fd, STDOUT_FILENO);
            Execvp(CommandInfo.argv[0], CommandInfo.argv);
            break;

        case PIPE: /* | command*/
            pipe_command();
            break;
    }
}

void command() {
    pid_t pid;
    int indicator = 0;

    parseline();

    /*re-edit command  and get the file*/
    indicator = ReviseCommand();

    if (indicator == -1)
        return;

    pid = fork();
    if (!pid) {
        /*the background process should not be
        disturbed by CTRL+C and CTRL+\*/
        /*sigaction(SIGINT, SIG_IGN, NULL);
        sigaction(SIGQUIT, SIG_IGN, NULL);*/
        ChildCommand();
        exit(0);
    } else {
        if (!CommandInfo.background)
            waitpid(pid, NULL, 0);
        else {
            /*if background process, the father should ignore the signal
               let init to reap it */
            printf("there is a background process\n");
        }
    }
    return;
}
