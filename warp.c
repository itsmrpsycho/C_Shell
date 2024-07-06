#include "headers.h"

extern char hd[MAX_PATH_SIZE];
extern char prev_dir[MAX_PATH_SIZE];

int warp(int forebackground , char ** tokenarray , int sizearray)
{
    // handle errors
    char current_wd[MAX_PATH_SIZE];
    getcwd(current_wd, MAX_PATH_SIZE);
 
    if(sizearray == 1)
    {
        strcpy(prev_dir, current_wd);
        chdir(hd);
        getcwd(current_wd, MAX_PATH_SIZE);
        // change the color of the prompt to blue
        printf("\033[1;34m");
        printf("%s\n", current_wd);
    }
    else
    {
        // "." , ".." , "~" , "-" , "path"
        for (int i = 1; i < sizearray; i++)
        {
            if(strcmp(tokenarray[i], "~") == 0)
            {
                strcpy(prev_dir, current_wd);
                chdir(hd);
            }
            else if(strcmp(tokenarray[i], "-") == 0)
            {
                char temp[MAX_PATH_SIZE];
                strcpy(temp, current_wd);
                strcpy(current_wd, prev_dir);
                strcpy(prev_dir, temp);
                chdir(current_wd);
            }
            else if(strcmp(tokenarray[i], ".") == 0)
            {
                // do nothing and stay in the current directory
            }
            else if(strcmp(tokenarray[i], "..") == 0)
            {
                char temp[MAX_PATH_SIZE];
                strcpy(temp, current_wd);
                int len = strlen(temp);
                int j = len - 1;
                while(temp[j] != '/')
                {
                    temp[j] = '\0';
                    j--;
                }
                temp[j] = '\0';
                strcpy(prev_dir, current_wd);
                strcpy(current_wd, temp);
                chdir(current_wd);
            }
            else
            {
                strcpy(prev_dir, current_wd);
                char temp[MAX_PATH_SIZE];
                // check if it begins with a '~'
                if(tokenarray[i][0] == '~')
                {
                    strcpy(temp, hd);
                    strcat(temp, tokenarray[i] + 1);
                    chdir(temp);
                }
                else if (tokenarray[i][0] == '/')
                {
                    chdir(tokenarray[i]);
                }
                else
                {
                    strcpy(temp, current_wd);
                    strcat(temp, "/");
                    strcat(temp, tokenarray[i]);
                    chdir(temp);
                }

            }
            getcwd(current_wd, MAX_PATH_SIZE); // update the current working directory
            // change the color of the prompt to blue
            printf("\033[1;34m");
            printf("%s\n", current_wd);
            printf("\033[0m");

        }
        
    }



    return 0;
}