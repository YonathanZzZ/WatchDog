#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */

#include "watchdog.h"

int main()
{
    char *str = "./watchdog_test.out";
    
    printf("----------------user program calls WDStart---------------\n");
    WDStart(&str);

    printf("-------------------user program keeps running-----------------\n");

    while(1)
    {
        sleep(2);
        printf("user process running...\n");
    }

    printf("----------------user program calls WDStop---------------\n");

    WDStop(0); 

    printf("----------------exited protected code section---------------\n");
    return (0);
}
