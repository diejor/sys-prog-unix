// file: onepipe.cpp
// author: M. Amine Belkoura
// date: 03/04/2020
// purpose: CS3377
// description:
// this program executes "ls -ltr | grep 3376", by dividing the two command
// among the child and parent process

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void print_args(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }
}

int main(int argc, char **argv) {
    int status;
    int childpid;
    char *cat_args[] = {"ls", "-ltr", NULL};
    char *grep_args[] = {"grep", "3376", NULL};
    print_args(argv);
    return 0;
}

int create_child(char **args) {
    int childpid;
    int pipefd[2];
    pipe(pipefd);
    if ((childpid = fork()) == -1) {
        perror("Error creating a child process");
        exit(1);
    }
    if (childpid == 0) {
        // replace cat's stdout with write part of 1st pipe
        dup2(pipefd[1], 1);
        // close all piped(very important!); end we're using was safely copied
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(*args, args);
        exit(0);
    } else {
        // replace grep'stdin with read end of 1st pipe
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        execvp(*args, args);
    }
    return childpid;
}

