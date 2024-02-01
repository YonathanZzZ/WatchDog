#include <sys/types.h> /* getpid */
#include <unistd.h>	   /* getpid */
#include <time.h>	   /* time_t */
#include <string.h>	   /* strcpy */
#include <stdlib.h>	   /* malloc, free */
#include <ifaddrs.h>   /* struct ifaddrs, struct sockaddr */
#include <arpa/inet.h> /* AF_INET */
#include <pthread.h>   /* mutex */

#include "uid.h"

enum
{
	TRUE = 1,
	FALSE = 0,
	FAILURE = 0,
	SUCCESS = 1
};

const uid_t UIDBadUID = {0};

pthread_mutex_t lock;

static int GetIP(char *ip)
{
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	char *addr;

	if (0 != getifaddrs(&ifap))
	{
		return (FAILURE);
	}
	for (ifa = ifap; ifa; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			addr = inet_ntoa(sa->sin_addr);
		}
	}

	strcpy(ip, addr);

	freeifaddrs(ifap);

	return (SUCCESS);
}

uid_t UIDCreate(void)
{
	static size_t counter = 0; 
	uid_t UID = {0};
	char *ip = NULL;

	UID.pid = getpid();

	pthread_mutex_lock(&lock);
	++counter;
	UID.counter = counter;
	pthread_mutex_unlock(&lock);

	UID.time = time(0);

	if (FAILURE == GetIP(UID.ip))
	{
		return (UIDBadUID);
	}

	free(ip);

	return (UID);
}

int UIDIsSame(uid_t id1, uid_t id2)
{
	if (id1.pid == id2.pid && id1.counter == id2.counter && id1.time == id2.time && 0 == strcmp(id1.ip, id2.ip))
	{
		return TRUE;
	}

	return FALSE;
}
