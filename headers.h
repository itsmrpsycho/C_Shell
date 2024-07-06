#ifndef HEADERS_H_
#define HEADERS_H_

# define MAX_USERNAME_SIZE 1024
# define MAX_SYSTEM_NAME_SIZE 1024
# define MAX_PATH_SIZE 1024
# define INPUT_SIZE 4096
# define TOKEN_SIZE 1024
# define STRING_SIZE 1024
# define MAX_FILE_NUMBER 1024
# define PASTEVENTS_NUMBER 15
# define MAX_BACKGROUND_PROCESSES 1024


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>


#include "prompt.h"
#include "exec.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "proclore.h"
#include "seek.h"
#include "system_commands.h"
#endif