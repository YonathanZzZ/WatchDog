#ifndef __WATCHDOG__
#define __WATCHDOG__

#include <stddef.h>

enum
{
	WD_FAILED_TO_CREATE_CHILD_PROCESS,
	WD_SUCCESS = 0, 
	WD_FAILED_TO_CREATE_WATCHDOG
};

/*
Name: WDStart
Description: 
Start a watchdog to protect a section of code.
Arguments:
exe_path - path to an executable file
Return: status
Time complexity: O(1)
Space complexity: O(1)  
*/ 

int WDStart(char **path);

/*
Name: WDStop
Description: 
Stop the watchdog
Arguments:
timeout - max time to wait for graceful exit 
Return: none
Time complexity: O(1)
Space complexity: O(1)  
*/

void WDStop(size_t timeout);

#endif /* __WATCHDOG__ */
