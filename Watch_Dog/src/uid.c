/****************************
* developer: Neviu Mamu
* reviewer: Yossi Matzliah
* date: 01.03.2023
*****************************/

#include <time.h>		/* time_t, time libary functions */	
#include <string.h>		/* strcmp, memcpy */
#include <arpa/inet.h>		/* sockadrr_in, AF  */
#include <ifaddrs.h>		/* struct ifaddrs, getifaddrs func */
#include <unistd.h>		/* getpid */

#include "uid.h"		/* my library */

/*********************** MACROS *****************/

#define LINUX_IP_SIZE (14)
#define ERROR (1)
#define ERROR_CODE (-1)
#define SUCCESS (0)
#define LOCAL_HOST_IP_NAME ("lo")


/*********************** static and global *****************/

static int  GetIp(unsigned char *ip);

static size_t global_count = 1;

/*********************** functions *****************/

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t uid = {0};
	
	uid.counter = global_count;
	uid.pid = getpid();
	++global_count;
	
	uid.time_stamp = time(NULL);
	
	if(GetIp(uid.ip) == ERROR || uid.time_stamp == ((time_t)ERROR_CODE))
	{
		return 	UIDGetBad();
	}
	
	return uid;
	
}


int UIDIsSame(ilrd_uid_t lhs, ilrd_uid_t rhs)
{
	int check = 1;
	
	if(difftime(lhs.time_stamp, rhs.time_stamp) != 0)
	{
		check = 0;
	}
	
	if(check)
	{
		check = (lhs.counter == rhs.counter);
	}
	
	if(check)
	{
		check = (lhs.pid == rhs.pid);
	}
	
	if(check)
	{
		check = (memcmp(lhs.ip, rhs.ip, LINUX_IP_SIZE) == 0);
	}
	
	return check;
}


ilrd_uid_t UIDGetBad()
{
	const ilrd_uid_t bad_uid = {0};
	
	return bad_uid;
}

/*********************** static  functions *****************/

static int  GetIp(unsigned char *ip)
{
	char *unwanted_interface = LOCAL_HOST_IP_NAME;
	
	struct ifaddrs *ls_ad =  NULL;
	struct ifaddrs *hold_to_free =  NULL;
	struct sockaddr_in *ip_location = NULL;
	
	if (getifaddrs(&ls_ad) == ERROR_CODE)
	{
		return ERROR;	
	}
	
	hold_to_free = ls_ad;
	
	while (NULL != ls_ad)
	{
		if (ls_ad->ifa_addr != NULL && ls_ad->ifa_addr->sa_family == AF_INET && (0 != strcmp(unwanted_interface, ls_ad->ifa_name)))
		{
			ip_location = (struct sockaddr_in *)ls_ad->ifa_addr;
			
			memcpy(ip, inet_ntoa(ip_location->sin_addr), LINUX_IP_SIZE);
			
			break;
		}
		
		ls_ad = ls_ad->ifa_next;
	}
	
	freeifaddrs(hold_to_free);
	
	return SUCCESS;
}



