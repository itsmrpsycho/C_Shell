#include "headers.h"

char proc_name_g[STRING_SIZE];
int time_for_proc;
int background_proc[MAX_BACKGROUND_PROCESSES];
int background_proc_ptr = 0;
extern char hd[MAX_PATH_SIZE];

int system_commands(int fore_background, char **token_array, int size_array)
{
    int status;
    pid_t pid;
    pid = fork();
    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (pid == 0)
    {
        // child process

        if(fore_background == 1)
        {
            char temp_addr[STRING_SIZE];
            strcpy(temp_addr, hd);
            strcat(temp_addr, "/tmp.txt");

            // redirecting the output of the process to a file if in background
            int devnull = open(temp_addr, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, S_IRUSR|S_IRGRP|S_IWGRP|S_IWUSR);
            dup2(devnull, STDOUT_FILENO);
            dup2(devnull, STDERR_FILENO);
            close(devnull);
        }

        if (execvp(token_array[0], token_array) < 0)
        {
            printf("Error in executing command. %s is either not a command or the arguments passed are incorrect. Please check .\n" , token_array[0]);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        // redirecting the output of the process to a file if in background

        // parent process
        if (fore_background == 0)
        {
            // foreground process
            waitpid(pid, NULL, WUNTRACED);
        }
        else
        {
            // background process
            printf("Process created with pid: %d\n", pid);
            background_proc[background_proc_ptr] = pid;
            background_proc_ptr++;
        }
        gettimeofday(&end, NULL);
    }
    else
    {
        // fork failed
        printf("Fork failed\n");
        exit(1);
    }
    int time_to_run;
    time_to_run = (int)(end.tv_usec - start.tv_usec) / 1000000 + (int)(end.tv_sec - start.tv_sec); // line used from Copilot
    char temp[STRING_SIZE];

    strcpy(temp, token_array[0]);

    if (fore_background == 0 && time_to_run > 2)
    {
        time_for_proc = time_to_run;
        strcpy(proc_name_g, temp);
    }

    return 0;
}