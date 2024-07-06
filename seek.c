#include "headers.h"

extern char hd[MAX_PATH_SIZE];
extern char prev_dir[MAX_PATH_SIZE];

// I have used the aid of ChatGPT. Needed it to get struct functionallity
void seek_recursive(const char *name, const char *search, int d_flag, int f_flag, int e_flag, int *count, char *exactMatch)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
    {
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            strcpy(path, name);
            strcat(path, "/");
            strcat(path, entry->d_name);

            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            if (strcmp(entry->d_name, search) == 0 && d_flag)
            {
                (*count)++;
                if (*count == 1 && e_flag)
                {
                    strcpy(exactMatch, path);
                    printf("\033[1;34m%s/\033[0m\n", path);
                }
                else
                {
                    printf("\033[1;34m%s/\033[0m\n", path);
                }
            }

            seek_recursive(path, search, d_flag, f_flag, e_flag, count, exactMatch);
        }
        else
        {
            char filePath[1024];
            strcpy(filePath, name);
            strcat(filePath, "/");
            strcat(filePath, entry->d_name);

            if (strcmp(entry->d_name, search) == 0 && f_flag)
            {
                (*count)++;
                if (*count == 1 && e_flag)
                {
                    strcpy(exactMatch, filePath);
                }
                else
                {
                    printf("\033[1;32m%s\033[0m\n", filePath);
                }
            }
        }
    }
    closedir(dir);
}

int seek(int forebackgrund, char **tokenarray, int sizearray)
{
    int d_flag = 1; // by default, they are switched on
    int f_flag = 1;
    int e_flag = 0;
    int count_flags = 0;

    char current_wd[MAX_PATH_SIZE];
    getcwd(current_wd, MAX_PATH_SIZE);

    if (sizearray > 2)
    {
        for (int i = 1; i < sizearray; i++)
        {
            if (strcmp(tokenarray[i], "-d") == 0)
            {
                f_flag = 0;
                count_flags++;
            }
            else if (strcmp(tokenarray[i], "-f") == 0)
            {
                d_flag = 0;
                count_flags++;
            }
            else if (strcmp(tokenarray[i], "-e") == 0)
            {
                e_flag = 1;
                count_flags++;
            }
        }
    }

    if (d_flag == 0 && f_flag == 0)
    {
        printf("Invalid flags\n");
        return 0;
    }

    char target_name[STRING_SIZE];
    char search_name[STRING_SIZE];

    if (sizearray > 2 + count_flags)
    {
        strcpy(target_name, tokenarray[sizearray - 1]);
        strcpy(search_name, tokenarray[sizearray - 2]);
    }
    else
    {
        target_name[0] = '.';
        strcpy(search_name, tokenarray[sizearray - 1]);
    }



    int count = 0;
    char exactMatch[STRING_SIZE];

    seek_recursive(target_name, search_name, d_flag, f_flag, e_flag, &count, exactMatch);

    // I have used the aid of ChatGPT 
    if (count == 1 && e_flag)
    {
        struct stat st;
        stat(exactMatch, &st);

        if (S_ISDIR(st.st_mode))
        {
            if (chdir(exactMatch) != 0)
            {
                printf("Missing permissions for task!\n");
                return 1;
            }
            strcpy(prev_dir, current_wd);
            getcwd(current_wd, MAX_PATH_SIZE);
            printf("Changing directory to: ");
            printf("\033[1;34m%s/\033[0m\n", exactMatch);
        }
        else
        {
            FILE *fp = fopen(exactMatch, "r");
            if (fp == NULL)
            {
                printf("Missing permissions for task!\n");
                return 1;
            }

            char line[256];
            printf("Printing contents of file: ");
            while (fgets(line, sizeof(line), fp))
            {
                printf("%s\n", line);
            }
            fclose(fp);
        }
    }
    else if (count == 0)
    {
        printf("No match found!\n");
    }

    return 0;
}
