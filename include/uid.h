#ifndef _UID_H__
#define _UID_H__

#include <time.h> /* time_t */
#include <sys/types.h> /* pid_t */

typedef struct UID
{
	pid_t pid;
	time_t time;
	size_t counter;
	char ip[16];
}uid_t;

extern const uid_t UIDBadUID;

/***********************************************************************/
/*
Description: Create a UID
Arguments: none
Return: UID struct

Time complexity: O(1).
Space complexity: O(1).
*/

uid_t UIDCreate(void);

/***********************************************************************/
/*
Description: 
Arguments:
Return: 

Time complexity: O().
Space complexity: O().
*/

int UIDIsSame(uid_t id1, uid_t id2);

#endif /* _UID_H__ */
