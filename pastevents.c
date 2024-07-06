#include "headers.h"

extern char global_input[INPUT_SIZE];
extern char hd[MAX_PATH_SIZE];

int pastevents(int fore_background, char **token_array, int size_array, int can_be_stored)
{
    // if can be stored = 1, otherwise 0


    char address_past_events[MAX_PATH_SIZE];
    strcpy(address_past_events, hd);
    strcat(address_past_events, "/.pastevents");

    if (can_be_stored == 1)
    {
        FILE *file = fopen(address_past_events, "r+");

        if (file == NULL)
        {
            // File doesn't exist, so create it
            file = fopen(address_past_events, "w");
            if (file == NULL)
            {
                perror("Error creating file");
                return 1;
            }
        }
        // store the command

        // read each line of file into a string from top to bottom and store in array of size 15
        char *pastevents_array[PASTEVENTS_NUMBER];
        char line[STRING_SIZE];
        int pastevents_array_ptr = 0;
        fseek(file, 0, SEEK_SET);
        while (fgets(line, sizeof(line), file))
        {
            if (strcmp(line, "\n") == 0)
            {
                continue;
            }
            else
            {

                pastevents_array[pastevents_array_ptr] = strdup(line);
                pastevents_array[pastevents_array_ptr][strcspn(pastevents_array[pastevents_array_ptr], "\n")] = '\0'; // remove the \n from the string
                pastevents_array_ptr++;
            }

        }


        if (pastevents_array_ptr > 0 && (strcmp(pastevents_array[pastevents_array_ptr - 1], global_input) == 0))
        {
            // already exists so do nothing
            // printf("Command already exists in pastevents.\n");
            fclose(file);
        }
        else
        {
            if (pastevents_array_ptr >= PASTEVENTS_NUMBER  )
            {
                // remove the oldest and shift the array
                for (int i = 0; i < PASTEVENTS_NUMBER-1; i++)
                {
                    pastevents_array[i] = pastevents_array[i + 1];
                }
                pastevents_array_ptr = PASTEVENTS_NUMBER-1;
            }
            // add the new command to the array
            

            pastevents_array[pastevents_array_ptr] = global_input;
            pastevents_array_ptr++;

            // write the array to the file and each string in new line
            fseek(file, 0, SEEK_SET);
            for (int i = 0; i < pastevents_array_ptr; i++)
            {
                pastevents_array[i][strcspn(pastevents_array[i], "\n")] = '\0'; // remove the \n from the string
                fprintf(file, "%s", pastevents_array[i]);
                fprintf(file, "\n");
            }

            fclose(file);
        }
    }
    else
    {
        if (size_array == 1) // means only pastevents
        {
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
            }
            // read the contents of the the file and print to terminal
            char line[STRING_SIZE];
            fseek(file, 0, SEEK_SET);
            int count = 0;
            while (fgets(line, sizeof(line), file))
            {
                line[strcspn(line, "\n")] = '\0'; // remove the \n from the string
                printf("%s\n", line);
                count++;
                if (count >= PASTEVENTS_NUMBER)
                {
                    break;
                }
            }
            fclose(file);
        }
        else if (strcmp(token_array[1], "purge") == 0)
        {
            fclose(fopen(address_past_events, "w"));
        }
    }

    return 0;
}