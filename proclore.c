#include "headers.h"

int proclore(int forebackground, char **tokenarray, int sizearray)
{
    // get the pid of the process

    int pid_proc =0;

    if (sizearray != 1)
    {
        pid_proc = atoi(tokenarray[1]);
    }
    else
    {
        pid_proc = getpid();
    }

    char proc_path_stat[MAX_PATH_SIZE];
    sprintf(proc_path_stat, "/proc/%d/stat", pid_proc);

    char proc_path_exe[MAX_PATH_SIZE];
    sprintf(proc_path_exe, "/proc/%d/exe", pid_proc);

    char proc_status[STRING_SIZE];
    char proc_group[STRING_SIZE];
    char proc_memory[STRING_SIZE];
    char proc_exe[STRING_SIZE];
    int pgrp;
    int tpgid;

    FILE *stat_file = fopen(proc_path_stat, "r");
    if (stat_file == NULL)
    {
        // perror("Error in opening stat file");
        printf("Error: Process with pid %d does not exist\n", pid_proc);
        return 1;
    }

    char line[STRING_SIZE];
    // used chatgpt in knowing where the different variables were
    while (fgets(line, sizeof(line), stat_file))
    {
        char *token;
        token = strtok(line, " ");
        int i = 0;
        while (token != NULL)
        {
            if (i == 2)
            {
                strcpy(proc_status, token);
            }
            else if (i == 3)
            {
                strcpy(proc_group, token);
            }
            else if (i == 22)
            {
                strcpy(proc_memory, token);
            }
            else if (i == 4)
            {
                pgrp = atoi(token);
            }
            else if (i == 7)
            {
                tpgid = atoi(token);
            }
            token = strtok(NULL, " ");
            i++;
        }
    }

    fclose(stat_file);

    FILE *exe_file = fopen(proc_path_exe, "r");
    if (exe_file == NULL)
    {
        perror("Error in opening exe file");
        return 1;
    }

    char exe_path[STRING_SIZE];
    int len = readlink(proc_path_exe, exe_path, STRING_SIZE);
    exe_path[len] = '\0';

    fclose(exe_file);

    char *fore_back;
    if (pgrp == tpgid)
    {
        fore_back = "+";
    }
    else
    {
        fore_back = " ";
    }

    printf("pid -- %d\n", pid_proc);
    printf("Process Status -- %s%s\n", proc_status, fore_back);
    printf("Process Group -- %s\n", proc_group);
    printf("memory -- %s\n", proc_memory);
    printf("Executable Path -- %s\n", exe_path);

    return 0;
}