#include "shell.h"
#include "parser.h"

static bool count_separated_segments_by_separator(char *command, char separator);

char** parse_text_by_separator(char *command, char separator) {

    char current_word[CMD_PARAM_SIZE];
    bool new_word_is_being_read = false;
    int word_index = 0;
    int array_index = 0;
    int total_words = count_separated_segments_by_separator(command, separator);
    char **args = (char **)malloc(sizeof(char *) * (total_words + 1));

    if(args == NULL) {
        printf("Memory allocation failed. Exiting with status 4 ...\n");
        exit(4);
    }

    for (int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {

        if (command[i] != separator) {
            
            if(command[i] == ' ' && !new_word_is_being_read) {
                continue;
            }

            if (!new_word_is_being_read) 
                new_word_is_being_read = true;
            
            current_word[word_index++] = command[i];
        }
        else if (new_word_is_being_read) {

            current_word[word_index] = '\0';
            size_t allocation_size = strlen(current_word);

            if(array_index == 0) 
                allocation_size += 10;
            else 
                allocation_size += 1;

            if((args[array_index] = (char *)malloc(sizeof(char) * allocation_size)) == NULL) {
                printf("Memory allocation failed. Exiting with status 5 ...\n");
                exit(5);
            }
            strcpy(args[array_index++], current_word);
            word_index = 0;
            new_word_is_being_read = false;
        }
    }

    if (new_word_is_being_read) {
        current_word[word_index] = '\0';
        args[array_index] = (char *)malloc(sizeof(char) * (strlen(current_word) + 1));
        strcpy(args[array_index++], current_word);
    }
    args[array_index] = NULL;
    return args;
}

void free_allocated_memory_for_parsed_text(char **ptr) {

    for (int i=0; ptr[i]!=NULL; i++) {
        free(ptr[i]);
    }
    free(ptr);
}

bool check_if_sequence_command(char *command) {
    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i] == ';') 
            return true;
    }
    return false;
}

bool check_if_pipeline_command(char *command) {
    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i] == '|')
            return true;
    }
    return false;
}

bool check_if_redirection_input_command(char *command) {
    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i] == '<')
            return true;
    }
    return false;
}

bool check_if_redirection_output_command(char *command) {
    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i] == '>')
            return true;
    }
    return false;
}

bool check_if_redirection_append_command(char *command) {
    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i] == '>' && command[i+1] == '>')
            return true;
    }
    return false;
}

static bool count_separated_segments_by_separator(char *command, char separator) {
    int total_segments = 0;
    bool reading_segment = false;

    for(int i=0; (command[i]!='\0' && command[i]!='\n'); i++) {
        if(command[i]!= separator) {
            if(command[i] == ' ' && !reading_segment) {
                continue;
            }
            if (!reading_segment) {
                reading_segment = true;
                total_segments++;
            }
        }
        else {
            reading_segment = false;
        }
    }   
    return total_segments;
}

void set_command_type(char *command, cmd *cmd_type) {
    if(check_if_sequence_command(command)) {
        *cmd_type = SEQUENCE_CMD;
    }
    else if(check_if_pipeline_command(command)){
        *cmd_type = PIPELINE_CMD;
    }
    else if(check_if_redirection_input_command(command)) {
        *cmd_type = REDIRECTION_INPUT_CMD;
    }   
    else if(check_if_redirection_append_command(command)) {
        *cmd_type = REDIRECTION_OUTPUT_APPEND_CMD;
    }
    else if(check_if_redirection_output_command(command)) {
        *cmd_type = REDIRECTION_OUTPUT_CMD;
    } 
}

void parse_command(char *command, cmd cmd_type, char ***parsed_command) {
    if(cmd_type == SEQUENCE_CMD) {
        *parsed_command = parse_text_by_separator(command, ';');
    }
    else if(cmd_type == PIPELINE_CMD) {
        *parsed_command = parse_text_by_separator(command, '|');
    }
    else if(cmd_type == REDIRECTION_INPUT_CMD) {
        *parsed_command = parse_text_by_separator(command, '<');
    }
    else if(cmd_type == REDIRECTION_OUTPUT_CMD) {
        *parsed_command = parse_text_by_separator(command, '>');
    }
    else if(cmd_type == REDIRECTION_OUTPUT_APPEND_CMD) {
        *parsed_command = parse_text_by_separator(command, '>');
    }
    else {
        *parsed_command = parse_text_by_separator(command, ' ');
    }
}