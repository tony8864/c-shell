#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define false 0
#define true 1
#define bool int

// definitions for pipe
#define READ_END 0
#define WRITE_END 1

// size for a temporary buffer
#define BUFF_SIZE 20

// size of the command
#define CMD_SIZE 256

// size of command parameter
#define CMD_PARAM_SIZE 50

// total number of parameters permitted in a command
#define CMD_TOTAL_PARAMS 20

// maximum path size
#define PATH_SIZE 128

void print_shell_message(void);

void print_prompt(void);

void run_shell(void);

void execute_command(char *command);

void exit_with_msg(char *msg);

#endif