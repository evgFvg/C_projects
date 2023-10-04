#define _POSIX_SOURCE
#include <stdio.h>     /*printf*/
#include <pthread.h>   /*pthread_create*/
#include <stdlib.h>    /*malloc*/
#include <string.h>    /*strcpy*/
#include <time.h>      /*time*/
#include <sys/types.h> /*ssize_t*/
#include <unistd.h>    /*struct timespec*/
#include <semaphore.h> /*semaphore*/
#include <fcntl.h>     /*O_CREAT sem*/
#include <signal.h>    /*SIGUSR1*/
#include <stdatomic.h>

#include "wd.h"

#define ERROR (-1)
#define SUCCESS (0)

int main(int argc, char *argv[])
{
    int i = 0;
    printf("User called WD_START function\n");

    if (SUCCESS != WD_Start(argc, argv, 5, 3))
    {
        printf("Unable to run the WD\n");
        return ERROR;
    }

    for (i = 50; i >= 0; --i)
    {
        printf("User proc i value is %d\n", i);
        sleep(1);
    }
    printf("User proc is ended, calling StopWD\n");
    StopWD();
    printf("StopWd worked fine, end of user.proc\n");

    return 0;
}
