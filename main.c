#include "headers.h"

// defining global variables
char hd[MAX_PATH_SIZE];
char prev_dir[MAX_PATH_SIZE];
char global_input[INPUT_SIZE];
extern int background_proc[MAX_BACKGROUND_PROCESSES];
extern int background_proc_ptr;

// function to check if pointer pointing to string contains pastevents or pastevents execute
// used the aid of Copilot in aid for string parsing
int check_pastevents(char *str)
{
    char address_past_events[MAX_PATH_SIZE];
    strcpy(address_past_events, hd);
    strcat(address_past_events, "/.pastevents");
    FILE *file = fopen(address_past_events, "r");
    if (file == NULL)
    {
        // File doesn't exist, so create it
        file = fopen(address_past_events, "w");
        if (file == NULL)
        {
            perror("Error creating file");
            return 1;
        }
        // printf(".pastevents file created.\n");
    }

    char *pastevents_array[PASTEVENTS_NUMBER];
    char line[STRING_SIZE];
    int pastevents_array_ptr = 0;
    while (fgets(line, sizeof(line), file))
    {
        // printf("%s", line);
        pastevents_array[pastevents_array_ptr] = strdup(line);
        pastevents_array[pastevents_array_ptr][strcspn(pastevents_array[pastevents_array_ptr], "\n")] = '\0'; // remove the \n from the string
        pastevents_array_ptr++;
        if (pastevents_array_ptr == PASTEVENTS_NUMBER)
        {
            break;
        }
    }
    fclose(file);

    int can_be_stored = 1;
    char temp_str[STRING_SIZE];
    strcpy(temp_str, str);

    char *token = strtok(str, ";&");
    char *token_list[TOKEN_SIZE];
    int token_list_ptr = 0;
    while (token != NULL)
    {
        token_list[token_list_ptr] = token;
        token_list_ptr++;
        token = strtok(NULL, ";&");
    }
    token_list[token_list_ptr] = NULL;

    for (int i = 0; i < token_list_ptr; i++)
    {
        char temp_str2[STRING_SIZE];
        strcpy(temp_str2, token_list[i]);
        char *token2 = strtok(temp_str2, " \n\t\r");
        char *token2_list[TOKEN_SIZE];
        int token2_list_ptr = 0;
        while (token2 != NULL)
        {
            token2_list[token2_list_ptr] = token2;
            token2_list_ptr++;
            token2 = strtok(NULL, " \n\t\r");
        }
        token2_list[token2_list_ptr] = NULL;
        if (strcmp(token2_list[0], "pastevents") == 0)
        {
            if (token2_list_ptr == 1)
            {
                // pastevents
                can_be_stored = 0;
            }
            else if (strcmp(token2_list[1], "purge") == 0)
            {
                // pastevents purge
                can_be_stored = 0;
            }
            else if (strcmp(token2_list[1], "execute") == 0)
            {
                // pastevent
                int index = atoi(token2_list[2]);

                // replace token2_list[2] with the command at that index
                strcpy(token_list[i], pastevents_array[pastevents_array_ptr - index]);
            }
        }
    }

    // using modified token_list, create a new string with same delimiters as original string
    char new_str[STRING_SIZE];
    strcpy(new_str, "");
    for (int i = 0; i < token_list_ptr; i++)
    {
        strcat(new_str, token_list[i]);
        int stringlength = strlen(new_str);
        // now check what delimiter was in original input and add it to new_str
        if (temp_str[stringlength] == '&')
        {
            strcat(new_str, "&");
        }
        else if (temp_str[stringlength] == ';')
        {
            strcat(new_str, ";");
        }
        else
        {
            strcat(new_str, "");
        }
    }

    strcpy(global_input, new_str);
    global_input[strcspn(global_input, "\n")] = '\0'; // remove the \n from the string

    // printf("%s\n", global_input);

    return can_be_stored;
}

// used the aid of Copilot in aid for string parsing
int parse_string_exec(char *str, int exit_status)
{
    // check if the string contains pastevents or pastevents execute
    int can_be_stored = check_pastevents(str);
    strcpy(str, global_input);

    str[strcspn(str, "\n")] = '\0'; // remove the \n from the string

    char *token = strtok(str, ";");
    char *token_list[TOKEN_SIZE];
    int token_list_ptr = 0;

    while (token != NULL)
    {
        // adding tokens to a list
        token_list[token_list_ptr] = token;
        token_list_ptr++;
        token = strtok(NULL, ";");
    }
    token_list[token_list_ptr] = NULL;

    for (int i = 0; i < token_list_ptr; i++)
    {
        char temp[STRING_SIZE];
        strcpy(temp, token_list[i]);
        // check if the string contains &

        int and_flag = 0;
        for (int j = 0; j < strlen(temp); j++)
        {
            if (temp[j] == '&')
            {
                and_flag ++;
            }
        }
        
        char *token2 = strtok(temp, "&");
        char *token2_list[TOKEN_SIZE];
        int token2_list_ptr = 0;
        while (token2 != NULL)
        {
            // adding tokens to a list
            token2_list[token2_list_ptr] = token2;
            token2_list_ptr++;
            token2 = strtok(NULL, "&");
        }
        token2_list[token2_list_ptr] = NULL;

        int fore_background = 0; // 0 for foreground, 1 for background. Default is foreground
        for (int j = 0; j < token2_list_ptr; j++)
        {
            if (and_flag > 0)
            {
                fore_background = 1;
                and_flag--;
            }
            else
            {
                fore_background = 0;
            }
            char temp2[STRING_SIZE];
            strcpy(temp2, token2_list[j]);

            // find the commands and arguments
            char *token3 = strtok(temp2, " \n\t\r");
            char *token3_list[TOKEN_SIZE];
            int token3_list_ptr = 0;
            while (token3 != NULL)
            {
                // adding tokens to a list
                token3_list[token3_list_ptr] = token3;
                token3_list_ptr++;
                token3 = strtok(NULL, " \n\t\r");
            }
            token3_list[token3_list_ptr] = NULL;
            if (strcmp(token3_list[0], "exit") == 0)
            {
                if (can_be_stored == 1)
                {
                    pastevents(fore_background, token3_list, token3_list_ptr, can_be_stored);
                }

                if (token3_list_ptr == 1)
                {
                    exit_status = 1;
                    break;
                }
            }

            exec_command(fore_background, token3_list, token3_list_ptr, can_be_stored);
        }

        if (exit_status == 1)
        {
            break;
        }
    }

    return exit_status;
}

int main()
{
    // char hd[MAX_PATH_SIZE];    // Home directory
    getcwd(hd, MAX_PATH_SIZE);       // Get the home directory, it keeps track of where the shell is running from
    getcwd(prev_dir, MAX_PATH_SIZE); // Initialise the prev_dir as home directory

    int exit_status = 0; // Exit status of the last command

    memset(background_proc, 0, sizeof(background_proc));
    background_proc_ptr = 0;

    char temp_addr[STRING_SIZE];
    strcpy(temp_addr, hd);
    strcat(temp_addr, "/tmp.txt");
    
     FILE *file = fopen(temp_addr, "r+");

        if (file == NULL)
        {
            // File doesn't exist, so create it
            file = fopen(temp_addr, "w");
            if (file == NULL)
            {
                perror("Error creating file");
                return 1;
            }
        }

        fclose(file);

    // Keep accepting commands
    while (1)
    {

        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt();
        char input[INPUT_SIZE];
        fgets(input, INPUT_SIZE, stdin);

        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character

        strcpy(global_input, input);

        exit_status = parse_string_exec(input, exit_status);

        if (exit_status == 1)
        {
            break;
        }
    }
    return 0;
}