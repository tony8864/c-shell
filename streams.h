#ifndef UTILS_H
#define UTILS_H

void declare_new_output_stream(int read_end, int write_end);
void declare_new_input_stream(int read_end, int write_end);
void change_standard_stream(void);
void reset_stream_change_variables(void);

void redirect_file_stream(void);
void set_fd_output_redirection(int fd);
void set_fd_input_redirection(int fd);
void reset_redirect_variables(void);

#endif