#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include<limits.h>
#include <time.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_HISTORY_SIZE 100
#define MAX_COMMANDS 30
#define MAX_ARGS 64
#define MAX_SUBSTR 100
#define max_length1 1024

// Function to execute a single command
void execute_command(char *command, char *history[], int *history_count) {
    char *args[MAX_ARGS];
    int num_args = 0;

    // Tokenize the command into arguments
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[num_args++] = token;
        token = strtok(NULL, " ");
    }
    args[num_args] = NULL; // Null-terminate the argument list

    // Create a child process to execute the command
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("Execution error");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        // Display command execution details
        time_t current_time;
        time(&current_time);
        printf("Process ID: %d, Time: %s", pid, ctime(&current_time));

        // Store command in history
        if (*history_count < MAX_HISTORY_SIZE) {
            char history_entry[MAX_INPUT_LENGTH + 30]; // Adjust the size accordingly
            snprintf(history_entry, sizeof(history_entry), "PID: %d, Time: %s - %s", pid, ctime(&current_time), command);
            history[*history_count] = strdup(history_entry);
            if (history[*history_count] == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            (*history_count)++;
        } else {
            fprintf(stderr, "History is full. Ignoring new command.\n");
        }
    }

}

// Function to execute a single command and not adding anything in histroy and for pipelines only
void execute_command1(char *command) {
    char *args[MAX_ARGS];
    int num_args = 0;

    // Tokenize the command into arguments
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[num_args++] = token;
        token = strtok(NULL, " ");
    }
    args[num_args] = NULL; // Null-terminate the argument list

    // Create a child process to execute the command
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("Execution error");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }

}
// displaying comands
void display_history(char *history[], int history_count) {
    printf("Command History:\n");
    // printing history
    for (int i = 0; i < history_count; i++) {
        printf("%s\n", history[i]);
    }
}


int main(){
    char inputs[MAX_INPUT_LENGTH];
    char *history[MAX_HISTORY_SIZE];
    int history_count = 0;
    int history_capacity = 0;
    

    while(1){
        int flag =0;
        printf("SHELL> ");
        fflush(stdout);
        // Read user input
        if (fgets(inputs, sizeof(inputs), stdin) == NULL) {
            perror("Input error");
            exit(EXIT_FAILURE);
        }
        int abc = sizeof(inputs);
        // taking actual size of inputs for iterating.
        size_t input_len = strlen(inputs);
        inputs[input_len -1] = '\0';
        // tremoving new line character 
        if(history_count>=history_capacity){
            history_capacity++;
        }
        if(strncmp(inputs,"cd ",3)==0){
            flag =1;
            const char *path  = inputs +3;
            if(chdir(path)!=0){
                perror("chdir");
            }else{
                char *current = NULL;
                size_t max_path_length = 100;
                current = (char *)malloc(max_path_length);
                if(current == NULL){
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                if(getcwd(current,max_path_length)!= NULL){
                    printf("current directory : %s\n",current);
                }else{
                    perror("getcwd");
                }
                free(current);
                current = NULL;

            }
            pid_t pid = getpid();
            // Display command execution details
            time_t current_time;
            time(&current_time);
            printf("Process ID: %d, Time: %s", pid, ctime(&current_time));
            // Store command in history
            if (history_count < MAX_HISTORY_SIZE) {
                char history_entry[MAX_INPUT_LENGTH + 30]; // Adjust the size accordingly
                snprintf(history_entry, sizeof(history_entry), "PID: %d, Time: %s - %s", pid, ctime(&current_time), inputs);
                history[history_count] = strdup(history_entry);
                if (history[history_count] == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                (history_count)++;
                } else {
                   fprintf(stderr, "History is full. Ignoring new command.\n");
            }
        }
        if(strlen(inputs) >=4 && strncmp(inputs,"exit",4)==0){
            flag =1;
            display_history(history, history_count);
            exit(EXIT_SUCCESS);
        }
         if(strlen(inputs)>=7 && strncmp(inputs,"history",7)==0){
            flag =1;
            display_history(history, history_count);
            pid_t pid = getpid();
            // Display command execution details
            time_t current_time;
            time(&current_time);
            printf("Process ID: %d, Time: %s", pid, ctime(&current_time));
            // Store command in history
            if (history_count < MAX_HISTORY_SIZE) {
                char history_entry[MAX_INPUT_LENGTH + 30]; // Adjust the size accordingly
                snprintf(history_entry, sizeof(history_entry), "PID: %d, Time: %s - %s", pid, ctime(&current_time), inputs);
                history[history_count] = strdup(history_entry);
                if (history[history_count] == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                (history_count)++;
                } else {
                   fprintf(stderr, "History is full. Ignoring new command.\n");
            }
        }
        // Check for pipe command
        char *pipe_command = strstr(inputs, "|");
        if (pipe_command != NULL) {
            // Split the input into multiple commands
            char *commands[MAX_COMMANDS];
            int num_commands = 0;
            char *token = strtok(inputs, "|");
            char *a =" ";
            char *substr[MAX_SUBSTR];
            int z =0;
            while (token != NULL && num_commands < MAX_COMMANDS) {
                substr[z++] =strdup(token);
                commands[num_commands++] = token;
                token = strtok(NULL, "|");
            }
            char concatenated[max_length1] = "";
            for(int i= 0 ;i<z;i++){
                strcat(concatenated,substr[i]);
                if(i<z-1){
                    strcat(concatenated,"|");
                }
                free(substr[i]);
            }
            pid_t pid = getpid();
            // Display command execution details
            time_t current_time;
            time(&current_time);
            printf("Process ID: %d, Time: %s", pid, ctime(&current_time));
            // Store command in history
            if (history_count < MAX_HISTORY_SIZE) {
                char history_entry[MAX_INPUT_LENGTH + 30]; // Adjust the size accordingly
                snprintf(history_entry, sizeof(history_entry), "PID: %d, Time: %s - %s", pid, ctime(&current_time), concatenated);
                history[history_count] = strdup(history_entry);
                if (history[history_count] == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                (history_count)++;
                } else {
                   fprintf(stderr, "History is full. Ignoring new command.\n");
            }
            // Execute the pipeline of commands
            int input_fd = STDIN_FILENO; // Initialize input file descriptor to stdin

            for (int i = 0; i < num_commands; i++) {
                char *command = commands[i];

                // Create a pipe to pass data between commands
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1) {
                    perror("Pipe error");
                    exit(EXIT_FAILURE);
                }

                // Create a child process to execute the current command
                pid_t pid = fork();

                if (pid == -1) {
                    perror("Fork error");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // Child process
                    if (i != 0) {
                        // Redirect input from the previous command's pipe
                        dup2(input_fd, STDIN_FILENO);
                        close(input_fd);
                    }

                    if (i != num_commands - 1) {
                        // Redirect output to the current command's pipe
                        dup2(pipe_fd[1], STDOUT_FILENO);
                        close(pipe_fd[0]);
                    }

                    close(pipe_fd[1]); // Close the write end of the pipe

                    execute_command1(command); // Execute the current command
                    exit(EXIT_SUCCESS);
                } else {
                    // Parent process
                    waitpid(pid, NULL, 0);
                    close(pipe_fd[1]); // Close the write end of the pipe
                    input_fd = pipe_fd[0]; // Set the input file descriptor for the next command
                }
                flag =1;
            }
            
        } else {
            if(flag ==0){
                // Execute a single command
                execute_command(inputs, history, &history_count);
            }
            }
        // Display command execution details
    }
}