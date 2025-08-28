#include "shell.h"
#include "parser.h"
#include "streams.h"

static void execute_simple_command(char **args);
static void execute_sequence_command(char **args);
static void execute_pipeline_command(char **args);
static void execute_redirection_input_command(char **args);
static void execute_redirection_output_command(char **args);
static void execute_redirection_append_command(char **args);
static void run_command(char **command, cmd cmd_type);
static int** allocate_memory_for_ptr_to_pipes(int num_of_pipes);
static void free_memory_for_ptr_to_pipes(int num_of_pipes, int **fd_array);

void execute_command(char *command) {
    cmd cmd_type = SIMPLE_CMD;
    char **parsed_command = NULL;

    set_command_type(command, &cmd_type);
    parse_command(command, cmd_type, &parsed_command);
    run_command(parsed_command, cmd_type);
    free_allocated_memory_for_parsed_text(parsed_command);
}

static void run_command(char **command, cmd cmd_type) {
    if(cmd_type == SEQUENCE_CMD) {
        execute_sequence_command(command);
    }
    else if(cmd_type == SIMPLE_CMD) {
        execute_simple_command(command);
    }
    else if(cmd_type == PIPELINE_CMD) {
        execute_pipeline_command(command);
    }
    else if(cmd_type == REDIRECTION_INPUT_CMD) {
        execute_redirection_input_command(command);
    }
    else if(cmd_type == REDIRECTION_OUTPUT_CMD) {
        execute_redirection_output_command(command);
    }
    else if(cmd_type == REDIRECTION_OUTPUT_APPEND_CMD) {
        execute_redirection_append_command(command);
    }
}

static void execute_sequence_command(char **args) {
    for(int i=0; args[i]!=NULL; i++) {
        execute_command(args[i]);
    }
}

static void execute_simple_command(char **args) {
    if(strcmp("cd", args[0]) == 0) {
        chdir(args[1]);
        return;
    }
    else if(strcmp("exit", args[0]) == 0) {
        printf("Exiting shell ...\n");
        exit(0);
    }

    pid_t pid = fork();

    if (pid == 0) {
        char cmd_path[CMD_SIZE + 10] = "/usr/bin/";
        char main_command[CMD_SIZE] = {0};
        
        // these are executed only if parent sets certain flags
        change_standard_stream();
        redirect_file_stream();
        
        // exec first try
        strcpy(main_command, args[0]);
        strcat(cmd_path, main_command);
        args[0] = cmd_path;
        execv(cmd_path, args);

        // exec second try
        strcpy(cmd_path, "/bin/");
        strcat(cmd_path, main_command);
        args[0] = cmd_path;
        execv(cmd_path, args);
        perror("execv");
        exit(1);
    }
    else if (pid < 0) {
        perror("fork");
        exit(1);
    }
    wait(NULL);
}

static void execute_pipeline_command(char **args) {

    // initialize pointer to fd array
    int num_of_commands = 0;
    for(; args[num_of_commands]!=NULL; num_of_commands++);
    int **fd_array = allocate_memory_for_ptr_to_pipes(num_of_commands - 1);

    // first process gets input from stdin, so it only changes its output stream
    declare_new_output_stream(fd_array[0][0], fd_array[0][1]);
    execute_command(args[0]);
    reset_stream_change_variables();

    // intermediate processes changes both the stdin and the stdout streams
    for(int i=1; i<num_of_commands-1; i++) {
        declare_new_input_stream(fd_array[i-1][0], fd_array[i-1][1]);
        declare_new_output_stream(fd_array[i][0], fd_array[i][1]);
        close(fd_array[i-1][1]);
        execute_command(args[i]);
        reset_stream_change_variables();
        close(fd_array[i-1][0]);
    }

    // last process puts output in stdout, so it only changes its input stream
    declare_new_input_stream(fd_array[num_of_commands-2][0], fd_array[num_of_commands-2][1]);
    close(fd_array[num_of_commands-2][1]);
    execute_command(args[num_of_commands-1]);
    reset_stream_change_variables();
    close(fd_array[num_of_commands-2][0]);

    free_memory_for_ptr_to_pipes(num_of_commands-1, fd_array);    
}

static void execute_redirection_append_command(char **args) {
    FILE *fptr = fopen(args[1], "a");
    if(fptr == NULL) {
        exit_with_msg("fopen");
    }
    set_fd_output_redirection(fileno(fptr));
    execute_command(args[0]);
    reset_redirect_variables();
    fclose(fptr);
}

static void execute_redirection_output_command(char **args) {
    FILE *fptr = fopen(args[1], "w");
    if(fptr == NULL) {
        exit_with_msg("fopen");
    }
    set_fd_output_redirection(fileno(fptr));
    execute_command(args[0]);
    reset_redirect_variables();
    fclose(fptr);
}

static void execute_redirection_input_command(char **args) {
    FILE *fptr = fopen(args[1], "r");
    if(fptr == NULL) {
        exit_with_msg("fopen");
    }
    set_fd_input_redirection(fileno(fptr));
    execute_command(args[0]);
    reset_redirect_variables();
    fclose(fptr);
}

static int** allocate_memory_for_ptr_to_pipes(int num_of_pipes) {
    // allocate memory for pointer to fd array
    int **fd_array = (int **)malloc(sizeof(int *) * num_of_pipes);
    if(fd_array == NULL) {
        printf("Error allocating memory for pointer to pipes. Exiting ...\n");
        exit(1);
    }

    for(int i=0; i<num_of_pipes; i++) {
        
        // allocate memory for fd array
        fd_array[i] = (int *)malloc(sizeof(int) * 2);
        if(fd_array[i] == NULL) {
            printf("Error allocating memory for fd array. Exiting ...\n");
            exit(1);
        }

        // initialize pipe
        if(pipe(fd_array[i]) < 0) {
            exit_with_msg("pipe");
        }
    }
    return fd_array;
}

static void free_memory_for_ptr_to_pipes(int num_of_pipes, int **fd_array) {
    for(int i=0; i<num_of_pipes; i++) {
        free(fd_array[i]);
    }
    free(fd_array);
}