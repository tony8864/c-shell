#ifndef PARSER_H
#define PARSER_H

#include <string.h>

typedef enum {
    SIMPLE_CMD,
    SEQUENCE_CMD,
    PIPELINE_CMD,
    REDIRECTION_INPUT_CMD,
    REDIRECTION_OUTPUT_CMD,
    REDIRECTION_OUTPUT_APPEND_CMD
} cmd;

char** parse_text_by_separator(char *command, char separator);

bool check_if_sequence_command(char *command);
bool check_if_pipeline_command(char *command);
bool check_if_redirection_input_command(char *command); 
bool check_if_redirection_output_command(char *command);
bool check_if_redirection_append_command(char *command); 
void set_command_type(char *command, cmd *cmd_type);
void parse_command(char *command, cmd cmd_type, char ***parse_command);
void free_allocated_memory_for_parsed_text(char **ptr);

#endif