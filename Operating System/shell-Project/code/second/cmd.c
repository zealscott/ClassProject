#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAXN 128

const char functions[][3] = {"<", ">", ">>", "|"};
char* historys[MAXN];
int his_number = 0;

/* Functional prototypes */
void sig_child(int signal);
void Evaluate(char* input);
int Parse(char* buf,
          char** argv,
          int* argc,
          int* func,
          int* pos,
          int* func_num);
void IO(int fd, int num);
int Change_IO(char** argv,
              int argc,
              int* func,
              int* pos,
              int func_num,
              int* index);
void Pipe(char** argv, int* func, int* pos, int index);

int main() {
    while (1) { /* endless while */
        printf("cmd > ");
        char* input = malloc(sizeof(char) * MAXN); /* store command line info */
        fgets(input, MAXN, stdin);
        if (feof(stdin))
            exit(0);

        if (input[0] ==
            '\n') /* if no command, then continue waiting for new cmd_line*/
            continue;

        historys[his_number++] = input;
        input[strlen(input) - 1] = '\0'; /* get rid of \n */

        /* Evaluate input */
        Evaluate(input);
    }

    return 0;
}

/* reap defunct */
void sig_child(int signal) {
    pid_t pid;
    int stat;
    wait(&stat);
}

/* Evaluate a command line */
void Evaluate(char* input) {
    char modified_input[MAXN]; /* copy right for input */
    char* argv[MAXN];          /* arguments for execv */
    int argc = 0;              /* the number of argvs */
    int func_num = 0;          /* the number of special functions */
    int func[MAXN];            /* index of functions */
    int pos[MAXN];             /* position of function notation */
    int background = 0;        /* default: foreground cmd */
    pid_t pid;                 /* process id */
    int status;                /* signal status */
    int index = 0; /* if with a pipe, then need to execvp the last program */
    int flag = 0;  /* flag=1 with a pipe, flag=0 without a pipe */

    strcpy(modified_input, input);
    background = Parse(modified_input, argv, &argc, func, pos, &func_num);

    if (background == -1) /* there is no cmd / cd / history */
        return;
    else if (background == 1) /* reap zombie */
        signal(SIGCHLD, sig_child);

    if ((pid = fork()) == 0) { /* child process */

        /* four special functions, judging whether need to change stdin or
         * stdout */
        if (func_num)
            flag = Change_IO(argv, argc, func, pos, func_num, &index);

        if (flag == 1) {
            /* there is a pipe */
            if ((execvp(argv[pos[index] + 1], argv + (pos[index] + 1))) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        } else if (flag == 0) {
            if ((execvp(argv[0], argv)) < 0) {
                printf("%s: Command not found.\n", argv[0]);
                exit(0);
            }
        } else {
            exit(0);
        }
    } else { /* parent process */
        if (!background) {
            if (waitpid(pid, &status, 0) < 0)
                printf("waitpid error\n");
        }
    }
    return;
}

/* parse the cmd_line , fulfill arguments, check for background pattern and find
 * the position of each unique functions */
int Parse(char* buf, char** argv, int* argc, int* func, int* pos, int* func_num)
/* buf: input string.
   argv: buf separate to each word.
   argc: the number of argv without &.
   func: store the index of functions. if func[0]=1, then
   functions[func[0]]="<". pos: the position of func[i]. func_num: the number of
   functions. */
{
    int i;
    int background = 0;
    char* delim = " ";
    argv[*argc] = strtok(buf, delim);
    while ((argv[++*argc] = strtok(NULL, delim))) {
        for (i = 0; i < 4; i++) {
            if (!strcmp(argv[*argc], functions[i])) {
                argv[*argc] = NULL;  /* if it is four of the special functions,
                                        then change it into NUNLL */
                func[*func_num] = i; /* record what the function is */
                pos[(*func_num)++] =
                    *argc; /* record where the functions is in argv */
                break;
            }
        }
    }
    /* if &exist, delete it and background = 1 */
    if (argv[*argc - 1] != NULL && !strcmp(argv[*argc - 1], "&")) {
        printf("background process\n");
        background = 1;
        argv[--*argc] = NULL;
    }

    if (argv[0] == NULL)
        return -1;

    if (!strcmp(argv[0], "exit")) /* check to exit */
        exit(0);

    if (!strcmp(argv[0], "cd")) { /* realize change directory*/
        if (chdir(argv[1]) < 0) {
            printf("wrong directory\n");
            exit(0);
        }
        return -1;
    }

    if (!strcmp(argv[0], "history")) { /* realize history n */
        if (argv[1]) {
            int n = atoi(argv[1]);
            for (i = 0; i < n; i++) {
                printf("the %d Command is : %s\n", i + 1,
                       historys[his_number - n + i]);
            }
        } else { /* realize history */
            for (i = 0; i < his_number; i++)
                printf("the %d Command is : %s\n", i + 1, historys[i]);
        }
        return -1;
    }

    return background;
}

/* "<": change stdin, ">" & ">>": change stdout */
void IO(int fd, int num) {
    int std_ = dup(num);
    close(num);
    dup(fd);
    close(fd);
}

/* four special functions 0:"<", 1:">", 2:">>", 3:"|" */
int Change_IO(char** argv,
              int argc,
              int* func,
              int* pos,
              int func_num,
              int* index) {
    int fd, i = 0, flag = 0; /* flag = 1 means there is a pipe */
    for (i = 0; i < func_num; i++) {
        /* func: 0:"<", 1:">", 2:">>", 3:"|" */
        if (((pos[i] + 1) < argc) && (argv[pos[i] + 1] != NULL)) {
            switch (func[i]) {
                case 0:
                    fd = open(argv[pos[i] + 1], O_RDONLY);
                    IO(fd, 0);
                    break;
                case 1:
                    fd = open(argv[pos[i] + 1], O_WRONLY | O_CREAT | O_TRUNC);
                    IO(fd, 1);
                    break;
                case 2:
                    fd = open(argv[pos[i] + 1], O_WRONLY | O_CREAT | O_APPEND);
                    IO(fd, 1);
                    break;
                case 3:
                    Pipe(argv, func, pos, i);
                    *index = i; /* if pipe is the last cmd, then need to tell
                                   where the cmd starts */
                    flag = 1;   /* there is a pipe */
                    break;
            }
        } else {
            printf("%s: there is no such a file. \n", functions[func[i]]);
            flag = -1; /* no need to execvp */
            break;
        }
    }
    return flag;
}

/* realize pipe function*/
void Pipe(char** argv, int* func, int* pos, int index) {
    int i, fore = 0;
    pid_t pid = 0;
    int status;
    int fd[2];
    pipe(fd); /* create pipe */

    if ((pid = fork()) == 0) { /* child pattern */
        close(fd[0]);          /* do not need read */
        close(1);              /* close stdout */
        dup(fd[1]);            /* set stdout to fd[1] */
        close(fd[1]);

        for (i = 0; i < index; i++) {
            if (func[i] == 3) /* check if is the first pipe */
                fore = pos[i] + 1;
        }

        if (fore) /* there is a pipe before this one, so we need to finish the
                     last one */
            execvp(argv[fore], argv + fore);
        else /* there is no pipe before */
            execvp(argv[0], argv);
    } else {          /* parent pattern */
        close(fd[1]); /* do not need write */
        close(0);     /* close stdin */
        dup(fd[0]);   /* set stdin to fd[0] */
        close(fd[0]);
        waitpid(pid, &status, 0);
    }
}
