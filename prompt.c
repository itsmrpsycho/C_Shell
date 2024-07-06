#include "headers.h"

extern char hd[MAX_PATH_SIZE];
extern int time_for_proc;
extern char proc_name_g[STRING_SIZE];
extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;

int getProcessNameFromPID(int pid, char *name, int len)
{
    char proc_path_stat[MAX_PATH_SIZE];
    sprintf(proc_path_stat, "/proc/%d/stat", pid);
    // printf("%s\n", proc_path_stat);

    FILE *stat_file = fopen(proc_path_stat, "r");
    if (stat_file == NULL)
    {
        perror("Error in opening stat file");
        return 1;
    }

    char line[STRING_SIZE];

    fscanf(stat_file, "%*d %s", name);
    fclose(stat_file);

    return 0;
}

void prompt()
{
    // Do not hardcode the prmopt

    // Get the username
    char username[MAX_USERNAME_SIZE];
    struct passwd *pw = getpwuid(getuid());
    strcpy(username, pw->pw_name);

    // Get the hostname
    char hostname[MAX_SYSTEM_NAME_SIZE];
    gethostname(hostname, MAX_SYSTEM_NAME_SIZE);

    // Get the current working directory
    char cwd[MAX_PATH_SIZE];
    getcwd(cwd, MAX_PATH_SIZE);

    if (strcmp(cwd, hd) == 0)
    {
        strcpy(cwd, "~");
    }
    else if (strstr(cwd, hd) != NULL)
    {
        char temp[MAX_PATH_SIZE];
        strcpy(temp, cwd + strlen(hd)); // Adding strlen(hd) effectively moves pointer to place where part not identical to hd. This is the relative path
        strcpy(cwd, "~");
        strcat(cwd, temp);
    }

    for (int i = 0; i < background_proc_ptr; i++)
    {
        if (background_proc[i] != -1)
        {

            int status;
            char *proc_name = (char *)calloc(STRING_SIZE, sizeof(char));
            getProcessNameFromPID(background_proc[i], proc_name, STRING_SIZE);
            int result = waitpid(background_proc[i], &status, WNOHANG);
            // used the aid of Copilot in mentioning the cases
            if (result == -1)
            {
                // perror("waitpid");
            }
            else if (result == 0)
            {
            }
            else if (WIFEXITED(status))
            {
                // printf("Process with PID %d has completed. Exit status: %d\n", , WEXITSTATUS(status));
                printf("%s with PID %d has completed with normal exit. Exit status: %d\n", proc_name, background_proc[i], WEXITSTATUS(status));
                background_proc[i] = -1;
            }
            else if (WIFSIGNALED(status))
            {
                printf("%s with PID %d has been terminated by a signal: %d\n", proc_name, background_proc[i], WTERMSIG(status));
                background_proc[i] = -1;
            }
            else
            {
                printf("%s with PID %d has completed but not with a normal exit.\n", proc_name, background_proc[i]);
                background_proc[i] = -1;
            }
        }
    }

    // open tmp.txt file
    char temp_addr[STRING_SIZE];
    strcpy(temp_addr, hd);
    strcat(temp_addr, "/tmp.txt");
    FILE *temp_file = fopen(temp_addr, "r");
    if (temp_file != NULL)
    {
        char line[STRING_SIZE];
        while (fgets(line, sizeof(line), temp_file))
        {
            printf("%s", line);
        }
    }
    fclose(temp_file);

    FILE *delete_temp_file = fopen(temp_addr, "w");
    fclose(delete_temp_file);

    // // change the color of the prompt
    printf("\033[1;32m");

    // checking for back and foreground processes
    if (time_for_proc > 2)
    {
        printf("<%s@%s: %s:%d %s> ", username, hostname, proc_name_g, time_for_proc, cwd);
        time_for_proc = 0;
    }
    else
    {
        printf("<%s@%s:%s> ", username, hostname, cwd);
    }

    // change the color of the prompt back to white

    printf("\033[0m");
}
