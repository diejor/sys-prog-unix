// file: TwoPipesThreeChildren.cpp
// date: 04/07/2024
// purpose: CS3377
// author: Diego Rodrigues Rodriguez
// professor: M. Amine Belkoura
//
// description:
// This program executes "ls -ltr | grep 3376 | wc -;", by using recursive calls.

#include <cstring>
#include <iostream>
#include <vector>
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
    cout << args[0] << " ";
    for (int i = 1; args[i] != NULL; i++) {
        cout << "\"" << args[i] << "\" ";
    }
}

/*
 * Debugging function to print the commands
 */
void print_commands(vector<char**>& cmds) {
    for (int i = 0; i < cmds.size(); i++) {
        cout << "Command " << i << ": ";
        print_args(cmds[i]);
        cout << endl;
    }
}

/*
 * Expand the command into an array of strings. 
 * @param cmd: the command to expand
 */
char** expand_command(char* cmd) {
    char** command = new char*[8];
    command[0] = strtok(cmd, " ");
    int i = 1;
    while (command[i-1] != NULL) {
        command[i] = strtok(NULL, " "); // get the next token
        i++;
    }
    return command;
}

/*
 * Parse the commands into an array of strings
 * @param raw_commands: the raw commands to parse_commands
 * @param size: the size of the raw commands array
 * @param cmds: the vector to store the parsed commands
 */
void parse_commands(char** raw_commands, int size, vector<char**>& cmds) {
    for (int i = 1; i < size; i++) {
        char** command = expand_command(raw_commands[i]);
        cmds.push_back(command);
    }
}

/*
 * Decides the commands to run based on the user input
 * @param args: the arguments passed to the program
 *
 */
void decide_commands(int size, char** args, vector<char**>& cmds) {
    if (size == 1) {
        cout << "** Using default arguments **" << endl;
        cout << "\t ls -ltr | grep 3376 | wc -l" << endl;
    } else if (size < 3) {
        cout << "** Usage:" << endl;
        cout << "\t \"command1\" \"command2\" \"command3\" ..." << endl;
        exit(1);
    } else if (size > 6) {
        cout << "** Too many arguments **" << endl;
        cout << "\t Only 5 commands are allowed" << endl;
        exit(1);
    } else {
        cout << "** Using custom commands **" << endl;
        cout << "\t";
        print_args(args);
        cout << endl;
        cmds.clear();

        parse_commands(args, size, cmds);
        // print_commands(cmds);
    }
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

    // cout << "Running command: ";
    // print_args(cmd);
    // cout << endl;

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
 * Recursive function to create pipes and run the commands. The first child process runs the last command and calls recursively to reach the last child that runs the first command. Then, the last child passses up the output through the pipes to the upper child processes until the first child process is reached.
 * @param cmds: the commands to run
 * @param index: the index of the command to run 
 * @param pipe_out_fd: file descriptor of the previous recursion.
 */ 
void recursive_pipe(vector<char**>& cmds, int index, int* pipe_out_fd) {
    if (index == 0) {
        run_command(cmds[index], NULL, pipe_out_fd);
    }
    int pipe_in_fd[2];
    pipe(pipe_in_fd);
    int child_pid = fork();

    if (child_pid == 0) {
        run_command(cmds[index], pipe_in_fd, pipe_out_fd);
    } else {
        recursive_pipe(cmds, index-1, pipe_in_fd);
    }
}

int main(int argc, char** argv) {

    // default commands
    char* first_cmd[8] = {"ls", "-ltr", NULL};
    char* second_cmd[8] = {"grep", "3376", NULL};
    char* third_cmd[8] = {"wc", "-l", NULL};
    int _size = 3;
    int* cmds_size = &_size;
    vector<char**> cmds = {first_cmd, second_cmd, third_cmd};
    decide_commands(argc, argv, cmds);


    // BODY OF ASSIGNMENT
    int last_pipe_fd[2];
    pipe(last_pipe_fd);
    int last_child_pid = fork();
    
    int index = cmds.size()-1;
    if (last_child_pid == 0) {
        run_command(cmds[index], last_pipe_fd, NULL);
    } else {
        recursive_pipe(cmds, index-1, last_pipe_fd);
    }

    return 0;
}


