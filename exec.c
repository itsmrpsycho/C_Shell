#include "headers.h"


int exec_command(int fore_background, char ** token_array , int size_array, int can_be_stored)
{

    if (strcmp(token_array[0], "warp") == 0)
    {
        warp(fore_background, token_array, size_array);
        if(can_be_stored == 1)
        {
            pastevents(fore_background, token_array, size_array, can_be_stored);
        }
    }
    else if (strcmp(token_array[0], "peek") == 0)
    {
        peek(fore_background, token_array, size_array);
        if(can_be_stored == 1)
        {
            pastevents(fore_background, token_array, size_array, can_be_stored);
        }
    }
    else if (strcmp(token_array[0], "pastevents")==0)
    {
        pastevents(fore_background, token_array, size_array, can_be_stored);
    }
    else if (strcmp(token_array[0], "proclore") == 0)
    {
        proclore(fore_background, token_array, size_array);
        if(can_be_stored == 1)
        {
            pastevents(fore_background, token_array, size_array, can_be_stored);
        }
    }
    else if (strcmp(token_array[0], "seek") == 0)
    {
        seek(fore_background, token_array, size_array);
        if(can_be_stored == 1)
        {
            pastevents(fore_background, token_array, size_array, can_be_stored);
        }
    }
    else
    {
        system_commands(fore_background, token_array, size_array);
        if(can_be_stored == 1)
        {
            pastevents(fore_background, token_array, size_array, can_be_stored);
        }
    }
  


    return 0;

}