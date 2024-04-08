// file: TwoPipesThreeChildren.cpp
// date: 04/07/2024
// purpose: CS3377
// professor: M. Amine Belkoura
//
// description:
// This program executes "ls -ltr | grep 3376 | wc -;", by dividing the two command
// among three children processes. The first child process executes the last command 
// "wc -l", the second child process executes the second command "grep 3376", and then
// the third child process executes the first command "ls -ltr". 

#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

/*
 * Debugging function to print the arguments
 */ 
void print_args(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }
}

/*
 * Expand the command into an array of strings
 * @param cmd: the command to expand
 */
void expand_command(char** cmd) {
    int size = sizeof(cmd);
    char* token = strtok(cmd[0], " ");
    token = strtok(NULL, " ");
    int i = 1;
    while (token != NULL) {
        cmd[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    cmd[i] = NULL;
    cmd[0] = strtok(cmd[0], " "); // wipe first command
}

/*
 * Parse the commands from the user input
 * @param raw_commands: a single c-string containing the commands separated by "|"
 */
void parse_commands(char* raw_commands, char** first_cmd, char** second_cmd, char** third_cmd) {
    char* token = strtok(raw_commands, "|");
    int i = 0;
    while (token != NULL) {
        if (i == 0) {
            first_cmd[0] = token;
        } else if (i == 1) {
            second_cmd[0] = token;
        } else if (i == 2) {
            third_cmd[0] = token;
        }
        token = strtok(NULL, "|");
        i++;
    }

    expand_command(first_cmd);
    expand_command(second_cmd);
    expand_command(third_cmd);
}

/*
 * Runs the given command replacing stdin and stdout with the given file descriptors
 * if they are not NULL. Note that intermediate pipes both file descriptors are not NUlL. For parent the stdin file descriptior is NULL and for last pipe stdout is NULL.
 * @param cmd: the command to run_command
 * @param pipe_in_fd: the file descriptor to read from replacing stdin
 * @param pipe_out_fd: the file descriptor to write to replacing stdout
 */
int run_command(char** cmd, int* pipe_in_fd, int* pipe_out_fd) {
    // if there is a pipe_in_fd, then we need to read from the 
    // pipe instead of stdin
    if (pipe_in_fd != NULL) {
        dup2(pipe_in_fd[0], 0);

        close(pipe_in_fd[0]);
        close(pipe_in_fd[1]);
    }
    // if there is a pipe_out_fd, then we need to write to the 
    // pipe instead of stdout
    if (pipe_out_fd != NULL) {
        dup2(pipe_out_fd[1], 1);

        close(pipe_out_fd[0]);
        close(pipe_out_fd[1]);
    }

    execvp(*cmd, cmd);
    exit(1);
}

/*
 * Decides the commands to run based on the user input
 * @param args: the arguments passed to the program
 * @param first_cmd: the first command to run
 * @param second_cmd: the second command to run
 * @param third_cmd: the third command to run
 */
void decide_commands(int size, char** args, char** first_cmd, char** second_cmd, char** third_cmd) {
    if (size == 1) {
        cout << "** Using default arguments **" << endl;
        cout << "\t ls -ltr | grep 3376 | wc -l" << endl;
    } else if (size > 2) {
        cout << "** second argument should be in the form of:" << endl;
        cout << "\t \"command1 | command2\"" << endl;
    } else {
        cout << "** Using custom commands **" << endl;
        cout << "\t" << args[1] << endl;
        parse_commands(args[1], first_cmd, second_cmd, third_cmd);
    }
}

int main(int argc, char** argv) {

    // default commands
    char* first_cmd[8] = {"ls", "-ltr", NULL};
    char* second_cmd[8] = {"grep", "3376", NULL};
    char* third_cmd[8] = {"wc", "-l", NULL};

    decide_commands(argc, argv, first_cmd, second_cmd, third_cmd);

    char** cmds[] = {first_cmd, second_cmd, third_cmd};

    // BODY OF ASSIGNMENT
    int first_pipefd[2];
    pipe(first_pipefd);
    int first_child = fork();

    /*
     * Strcuture of the program:
     *                  P
     *                  |
     *              first_pipe
     *                fork()
     *                  |
     *            +-----+-----+
     *            |           |
     *            P           |
     *        second pipe     |
     *          fork()        |
     *            |           |
     *        +---+---+       |
     *        |       |       |
     *        P       |       |
     *    third pipe  |       |
     *      fork()    |       |
     *        |       |       |
     *     ---+---    |       |
     *     |     |    |       |
     *     P     C3   C2      C1
     */


    if (first_child == 0) {
        run_command(third_cmd, first_pipefd, NULL);
    } else {
        int second_pipefd[2];
        pipe(second_pipefd);
        int second_child = fork();
        if (second_child == 0) {
            run_command(second_cmd, second_pipefd, first_pipefd);
        } else { 
            int third_child = fork();
            if (third_child == 0) {
                run_command(first_cmd, NULL, second_pipefd);
            } 
        }
    }

    return 0;
}

