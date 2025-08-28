#include "streams.h"
#include "shell.h"

// variables for pipelining
static int input_stream = -1;
static int output_stream = -1;
static int fd_read_end_stream_output = -1;
static int fd_write_end_stream_output = -1;
static int fd_read_end_stream_input = -1;
static int fd_write_end_stream_input = -1;

// variables for redirection
static int input_redirection = -1;
static int output_redirection = -1;
static int append_redirection = -1;

void reset_stream_change_variables(void) {
    input_stream = -1;
    output_stream = -1;
    fd_read_end_stream_output = -1;
    fd_write_end_stream_output = -1;
    fd_read_end_stream_input = -1;
    fd_write_end_stream_input = -1;
}

void declare_new_output_stream(int read_end, int write_end) {
    output_stream = STDOUT_FILENO;
    fd_write_end_stream_output = write_end;
    fd_read_end_stream_output = read_end;
}

void declare_new_input_stream(int read_end, int write_end) {
    input_stream = STDIN_FILENO;
    fd_read_end_stream_input = read_end;
    fd_write_end_stream_input = write_end;
}

void change_standard_stream(void) {
    if(output_stream == STDOUT_FILENO) {
        dup2(fd_write_end_stream_output, STDOUT_FILENO);
        close(fd_write_end_stream_output);
        close(fd_read_end_stream_output);
    }
    if(input_stream == STDIN_FILENO) {
        dup2(fd_read_end_stream_input, STDIN_FILENO);
        close(fd_read_end_stream_input);
        close(fd_write_end_stream_input);
    }
}

void set_fd_input_redirection(int fd) {
    input_redirection = fd;
}

void set_fd_output_redirection(int fd) {
    output_redirection = fd;
}

void redirect_file_stream(void) {
    if(input_redirection >= 0) {
        dup2(input_redirection, STDIN_FILENO);
        close(input_redirection);
    }
    if(output_redirection >= 0) {
        dup2(output_redirection, STDOUT_FILENO);
        close(output_redirection);
    }
    if(append_redirection >= 0) {
        dup2(append_redirection, STDIN_FILENO);
        close(append_redirection);
    }
}

void reset_redirect_variables(void) {
    input_redirection = -1;
    output_redirection = -1;
    append_redirection = -1;
}