#define _POSIX_C_SOURCE 200112L /* setenv */
#include <stdlib.h>             /* setenv */
#include <sys/types.h>          /* getpid */
#include <unistd.h>             /* getpid */

#include "watchdog.h"

#define PID_SIZE (8)
#define SEM_NAME ("wd_sem")

int main(int argc, char *argv[])
{
    char pid_str[10] = {'\0'};

    (void)argc;

    /* save pid to string */
    sprintf(pid_str, "%d", getpid());

    setenv("WD_PID", pid_str, 1);
    WDStart(&argv[0]);

    return (0);
}