/*
 * Developer Evgenii F.
 * Reviewd by Nitai I.
 * Date 02.03.2023
 * 
 */

#include <string.h>    /*strcpy*/
#include <ifaddrs.h>   /*getifaddrs*/
#include <arpa/inet.h> /*struct sockaddr_in*/
#include <unistd.h>    /*getpid*/
#include <stdio.h>    /*getpid*/

#include "uid.h"

static const ilrd_uid_t bad_uid = {0, 0, 0, {0}};

static size_t uid_counter = 0;

ilrd_uid_t UIDCreate(void) 	
{
	ilrd_uid_t new_uid = {0};
	struct ifaddrs *ifap = NULL;
	struct ifaddrs *ifa_runner = NULL;
	struct sockaddr_in *sock = NULL;
	
        time(&new_uid.time_stamp); 
	new_uid.counter = (++uid_counter);
	new_uid.pid = getpid();
	
	if (-1 == getifaddrs(&ifap))
	{
		perror("getifaddrs");
		--uid_counter;
		return UIDGetBad();
	}
	
	for (ifa_runner = ifap; ifa_runner != NULL; ifa_runner = ifa_runner->ifa_next) 
	{
		if (NULL != ifa_runner->ifa_addr && AF_INET == ifa_runner->ifa_addr->sa_family)
		{
			sock = (struct sockaddr_in *)ifa_runner->ifa_addr;
			memcpy(new_uid.ip, inet_ntoa(sock->sin_addr), LINUX_IP_SIZE);
		}
	}

	freeifaddrs(ifap);	
	
	return new_uid;
}


ilrd_uid_t UIDGetBad(void)
{
    return bad_uid;
}

int UIDIsSame(ilrd_uid_t lhs, ilrd_uid_t rhs)
{
    return (

        0 == difftime(lhs.time_stamp, rhs.time_stamp) &&
        0 == memcmp(lhs.ip, rhs.ip, LINUX_IP_SIZE) &&
        lhs.counter == rhs.counter &&
        lhs.pid == rhs.pid);
}
