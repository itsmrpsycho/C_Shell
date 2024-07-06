#include "headers.h"

extern char hd[MAX_PATH_SIZE];
extern char prev_dir[MAX_PATH_SIZE];

void print_color(char *filename, char *dir_name)
{
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0)
        return;
    if (S_ISDIR(fileStat.st_mode))
    {
        printf("\033[1;34m");
        printf(" %s\n", dir_name);
        printf("\033[0m");
    }
    else if (fileStat.st_mode & S_IXUSR)
    {
        printf("\033[1;32m");
        printf(" %s\n", dir_name);
        printf("\033[0m");
    }
    else
    {
        printf("\033[0m");
        printf(" %s\n", dir_name);
        printf("\033[0m");
    }
}

int compare_strings(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}

// have used chatgpt for helping me finish this
void ls(const char *directory, int show_hidden, int long_format)
{
    struct dirent **entries;
    int num_entries;

    num_entries = scandir(directory, &entries, NULL, compare_strings); // replaced alphasort with this as was unidentified by system

    if (num_entries == -1)
    {
        perror("scandir");
    }

    for (int i = 0; i < num_entries; i++)
    {
        struct dirent *entry = entries[i];

        if (!show_hidden && entry->d_name[0] == '.')
        {
            continue; // Skip hidden entries
        }

        char path[MAX_PATH_SIZE];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        struct stat info;
        if (stat(path, &info) == -1)
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (long_format)
        {
            // used chatgpt for getting this
             printf((S_ISDIR(info.st_mode)) ? "d" : "-");
            printf((info.st_mode & S_IRUSR) ? "r" : "-");
            printf((info.st_mode & S_IWUSR) ? "w" : "-");
            printf((info.st_mode & S_IXUSR) ? "x" : "-");
            printf((info.st_mode & S_IRGRP) ? "r" : "-");
            printf((info.st_mode & S_IWGRP) ? "w" : "-");
            printf((info.st_mode & S_IXGRP) ? "x" : "-");
            printf((info.st_mode & S_IROTH) ? "r" : "-");
            printf((info.st_mode & S_IWOTH) ? "w" : "-");
            printf((info.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %ld", info.st_nlink);
            printf(" %s", getpwuid(info.st_uid)->pw_name);
            printf(" %s", getgrgid(info.st_gid)->gr_name);
            printf(" %ld", info.st_size);
            char *time_str = ctime(&info.st_mtime);
            time_str[strlen(time_str) - 1] = '\0';
            printf(" %s", time_str);
        }

        print_color(path, entry->d_name);

        free(entry);
    }

    free(entries);
}

int peek(int forebackground, char **tokenarray, int sizearray)
{
    char current_wd[MAX_PATH_SIZE];
    getcwd(current_wd, MAX_PATH_SIZE);

    int a_flag = 0;
    int l_flag = 0;
    int path_flag = 0;

    if (sizearray > 1)
    {
        for (int i = 1; i < sizearray; i++)
        {
            if (strcmp(tokenarray[i], "-a") == 0)
            {
                a_flag = 1;
            }
            else if (strcmp(tokenarray[i], "-l") == 0)
            {
                l_flag = 1;
            }
            else if (strcmp(tokenarray[i], "-la") == 0 || strcmp(tokenarray[i], "-al") == 0)
            {
                a_flag = 1;
                l_flag = 1;
            }
            else
            {
                path_flag = 1;
            }
        }
    }

    char path_to_be_opened[MAX_PATH_SIZE];
    if (path_flag == 1)
    {
        char temp[MAX_PATH_SIZE];
        // check if it begins with a '~'
        if (tokenarray[sizearray - 1][0] == '~')
        {
            strcpy(temp, hd);
            strcat(temp, tokenarray[sizearray - 1] + 1);
            strcpy(path_to_be_opened, temp);
        }
        else if (tokenarray[sizearray - 1][0] == '/')
        {
            strcpy(path_to_be_opened, tokenarray[sizearray - 1]);
        }
        else if (strcmp(tokenarray[sizearray - 1], ".") == 0)
        {
            strcpy(path_to_be_opened, current_wd);
        }
        else if (strcmp(tokenarray[sizearray - 1], "..") == 0)
        {
            strcpy(temp, current_wd);
            int len = strlen(temp);
            int j = len - 1;
            while (temp[j] != '/')
            {
                temp[j] = '\0';
                j--;
            }
            temp[j] = '\0';
            strcpy(path_to_be_opened, temp);
        }
        else if (strcmp(tokenarray[sizearray - 1], "-") == 0)
        {
            strcpy(path_to_be_opened, prev_dir);
        }
        else
        {
            strcpy(temp, current_wd);
            strcat(temp, "/");
            strcat(temp, tokenarray[sizearray - 1]);
            strcpy(path_to_be_opened, temp);
        }
    }
    else
    {

        strcpy(path_to_be_opened, current_wd);
    }

    DIR * d;
    

    char * path_ls = path_to_be_opened;

    ls(path_ls, a_flag,l_flag);

    return 0;
}