
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

#define ENV_VAR_THRESH_KEEPER ("__WD_ENV_VAR_")
#define ERROR (-1)
#define SUCCESS (0)

static int GetEnvArguments(size_t *thresh, size_t *interval);

int main(int argc, char *argv[])
{
    size_t thresh = 0;
    size_t interval = 0;

    GetEnvArguments(&thresh, &interval);

    if (argc > 1)
    {
        printf("wd value of argv[1] is %s\n", argv[1]);
    }

    printf("WD called WD_START function\n");

    WD_Start(argc, argv, thresh, interval);

    return 0;
}

static int GetEnvArguments(size_t *thresh, size_t *interval)
{
    char *both_values = NULL;
    char *token = NULL;

    both_values = getenv(ENV_VAR_THRESH_KEEPER);

    if (NULL == both_values)
    {
        perror("Reading env var error\n");
        exit(1);
    }

    token = strtok(both_values, " ");

    if (NULL != token)
    {
        *thresh = (size_t)atoi(token);
    }

    token = strtok(NULL, " ");

    if (NULL != token)
    {
        *interval = (size_t)atoi(token);
    }

    return SUCCESS;
}
