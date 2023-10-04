
#include <stdio.h>     /*printf*/
#include <pthread.h>   /*pthread_create*/
#include <stdlib.h>    /*malloc*/
#include <string.h>    /*strcpy*/
#include <time.h>      /*time*/
#include <unistd.h>    /*struct timespec*/
#include <semaphore.h> /*semaphore*/
#include <fcntl.h>     /*O_CREAT sem*/
#include <signal.h>    /*SIGUSR1*/
#include <stdatomic.h>
#include <sys/wait.h> /*wait*/

/*#include "heap.h"
#include "pq.h"
#include "task.h"
#include "uid.h"
#include "dynamic_vector.h"*/
#include "scheduler.h"


#include "wd.h"

#define ERROR (-1)
#define SUCCESS (0)
#define STOP_SCH (3)
#define DEFAULT_SEM_NAME ("/sem_wd")
#define WD_ELF_PATH ("./wd.out")
#define BUFF_SIZE (10)
#define PROC_NAME (argv[0])
#define TRUE (1)
#define ENV_VAR_THRESH_KEEPER ("__WD_ENV_VAR_")
#define ENV_VAR_USER_FILE_NAME ("__USER_PROCESS_OWN_NAME_")
#define DEFAULT_START_TASK_TIME ((time(NULL)) + (3))

#ifndef NDEBUG
#define PRINT_DEBUG(...) (printf(__VA_ARGS__))
#else
#define PRINT_DEBUG(...)
#endif


pthread_mutex_t mut_counter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut_stop_sch = PTHREAD_MUTEX_INITIALIZER;

atomic_int counter = ATOMIC_VAR_INIT(0);
int is_stopped_sch = 0;
int is_dead_wd = 0;
pid_t pid_addr = {0};
sch_t *sch_g = NULL;
sem_t *sem_g = NULL;
char **argv_g = NULL;
pthread_t user_thr = {0};
size_t thresh_hold_g = 0;

static int SemAndSchInit();
static int FillTasks(size_t interval);
static int SetEnv(size_t thresh_hold, size_t interval, char *own_name);
static int InitSignalHandlers(void);
static int CreateWD(void);

static int SendSigUsr1Task(void *task_arg);
static int CheckThreshHoldTask(void *task_arg);
static int ContUserTask(void *arg);

void HandlerSIGUSR1(int signum);
void HandlerSIGUSR2(int signum);

static void *UserRunSch(void *arg);
static void WD_Run(void);

static void DestroyAll(char *error_msg);
static int IsWDProc(const char *proc_name);

/******************************User Function**************************************/

int WD_Start(int argc, char *argv[], size_t thresh_hold, size_t interval)
{
    (void)argc;
    #ifndef NDEBUG
    printf("Hello\n");
    #endif
    

    PRINT_DEBUG("My PID is %d\n", getpid());

    if (SUCCESS != InitSignalHandlers())
    {
        perror("Signal handlers init error\n");

        return ERROR;
    }

    PRINT_DEBUG("Signal handlers initiated\n");

    if (ERROR == SemAndSchInit())
    {
        return ERROR;
    }

    PRINT_DEBUG("Semaphore and Scheduler initiated\n");

    if (ERROR == FillTasks(interval))
    {
        DestroyAll("Sched task's filling error\n");
        return ERROR;
    }

    PRINT_DEBUG("2 tasks added successfully to sch\n");

    argv_g = argv;
    thresh_hold_g = thresh_hold;

    if (TRUE != IsWDProc(PROC_NAME))
    {
        /*user proc*/
        PRINT_DEBUG("User process after inspection IsWdProc\n");

        if (SUCCESS != SetEnv(thresh_hold, interval, argv[0]))
        {
            DestroyAll("Setting ENV variables error\n");
            return ERROR;
        }

        if (ERROR == CreateWD())
        {
            DestroyAll("Forking WD error\n");
            return ERROR;
        }
        else /*WD created*/
        {
            PRINT_DEBUG("WD created successfully...\n");

            if (SUCCESS != pthread_create(&user_thr, NULL, UserRunSch, NULL))
            {
                StopWD();
                return ERROR;
            }
            else
            {
                PRINT_DEBUG("User's thread susccessfully created\n");
            }
        }
    }
    else /*wd proc*/
    {
        PRINT_DEBUG("Entered wd part of start func\n");
        pid_addr = getppid();
        WD_Run();

        return SUCCESS;
    }

    return SUCCESS;
}

void StopWD()
{
    kill(pid_addr, SIGUSR2);
    raise(SIGUSR2);
    wait(NULL);
    pthread_join(user_thr, NULL);

    unsetenv(ENV_VAR_THRESH_KEEPER);
    unsetenv(ENV_VAR_USER_FILE_NAME);

    sem_unlink(DEFAULT_SEM_NAME);
    SchDestroy(sch_g);
}

/******************************Init Functions******************************/

static int InitSignalHandlers()
{
    struct sigaction sa_1;
    struct sigaction sa_2;

    sa_1.sa_handler = HandlerSIGUSR1;
    sigemptyset(&sa_1.sa_mask);
    sa_1.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa_1, NULL) == ERROR)
    {
        return ERROR;
    }

    sa_2.sa_handler = HandlerSIGUSR2;
    sigemptyset(&sa_2.sa_mask);
    sa_2.sa_flags = 0;

    if (sigaction(SIGUSR2, &sa_2, NULL) == ERROR)
    {
        return ERROR;
    }

    return SUCCESS;
}

static int SemAndSchInit()
{
    sem_g = sem_open(DEFAULT_SEM_NAME, O_CREAT, 0644, 0);

    if (sem_g == SEM_FAILED)
    {
        perror("Semaphor open error\n");
        return ERROR;
    }

    sch_g = SchCreate();

    if (NULL == sch_g)
    {
        perror("Scheduler creating error\n");

        if (ERROR == sem_close(sem_g))
        {
            perror("Closing semaphore error\n");
        }

        return ERROR;
    }

    return SUCCESS;
}

static int SetEnv(size_t thresh_hold, size_t interval, char *own_name)
{
    char buff1[BUFF_SIZE] = {0};
    char buff2[BUFF_SIZE] = {0};
    char res_buf[2 * BUFF_SIZE] = {0};

    sprintf(buff1, "%d", (int)thresh_hold);
    sprintf(buff2, "%d", (int)interval);
    snprintf(res_buf, sizeof(res_buf), "%s %s", buff1, buff2);

    if (SUCCESS != setenv(ENV_VAR_USER_FILE_NAME, own_name, 1) ||
        SUCCESS != setenv(ENV_VAR_THRESH_KEEPER, res_buf, 1))
    {
        return ERROR;
    }

    return SUCCESS;
}

static int FillTasks(size_t interval)
{
    ilrd_uid_t res_uid = {0};

    res_uid = SchAddTask(sch_g, SendSigUsr1Task, NULL, DEFAULT_START_TASK_TIME, interval);

    if (TRUE == UIDIsSame(res_uid, UIDGetBad()))
    {
        return ERROR;
    }

    res_uid = SchAddTask(sch_g, CheckThreshHoldTask, NULL, DEFAULT_START_TASK_TIME, interval);

    if (TRUE == UIDIsSame(res_uid, UIDGetBad()))
    {
        return ERROR;
    }

    return SUCCESS;
}

static int CreateWD()
{
    pid_t wd_pid = {0};
    PRINT_DEBUG("Entered CreateWD() func...\n");

    argv_g[0] = WD_ELF_PATH;

    wd_pid = fork();

    if (ERROR == wd_pid)
    {
        return ERROR;
    }
    else if (0 == wd_pid) /*wd child proc*/
    {
        if (ERROR == execv(WD_ELF_PATH, argv_g))
        {
            return ERROR;
        }
    }
    else /*user parent proc*/
    {
        if (0 < is_dead_wd)
        {
            PRINT_DEBUG("WD is down, creating new WD proces, please wait...\n");
            raise(SIGSTOP); /*letting WD to up*/
        }

        pid_addr = wd_pid; /*reseting new wd_pid*/
        is_dead_wd = TRUE; /*entering the function >1 times means WD is down*/
        sem_wait(sem_g);
    }

    return SUCCESS;
}

/**************************Signal Handlers******************************/

void HandlerSIGUSR1(int signum)
{
    (void)signum;
    pthread_mutex_lock(&mut_counter);
    atomic_store(&counter, 0);
    pthread_mutex_unlock(&mut_counter);
    PRINT_DEBUG("SIG1 received by proc %d\n", (int)getpid());
}

void HandlerSIGUSR2(int signum)
{
    (void)signum;

    pthread_mutex_lock(&mut_stop_sch);
    is_stopped_sch = TRUE;
    pthread_mutex_unlock(&mut_stop_sch);
    PRINT_DEBUG("Sig2 to Stop the Sch received\n");
    sem_close(sem_g);
}

/**************************TASK functions****************************/

static int SendSigUsr1Task(void *task_arg)
{
    (void)task_arg;

    pthread_mutex_lock(&mut_stop_sch);
    if (is_stopped_sch == TRUE)
    {
        pthread_mutex_unlock(&mut_stop_sch);
        return STOP_SCH;
    }
    pthread_mutex_unlock(&mut_stop_sch);

    PRINT_DEBUG("Sending signal SIGUSR1 from %d to %d\n", getpid(), pid_addr);
    kill(pid_addr, SIGUSR1);

    pthread_mutex_lock(&mut_counter);
    atomic_fetch_add(&counter, 1);
    pthread_mutex_unlock(&mut_counter);

    return SUCCESS;
}

static int CheckThreshHoldTask(void *task_arg)
{
    (void)task_arg;
    int atomic_val = 0;

    pthread_mutex_lock(&mut_counter);
    atomic_val = atomic_load(&counter);
    PRINT_DEBUG("Counter atomic value in checker: %d\n", atomic_val);
    
    if (atomic_val >= (int)thresh_hold_g)
    {
        PRINT_DEBUG("Counter atomic value is %d\n", atomic_val);
        PRINT_DEBUG("Thresh hold task failure. Forking a new process...\n");
        pthread_mutex_unlock(&mut_counter);
        return ERROR;
    }

    pthread_mutex_unlock(&mut_counter);

    return SUCCESS;
}

static int ContUserTask(void *arg)
{
    (void)arg;
    kill(pid_addr, SIGCONT);
    
    return SUCCESS;
}

/************************************RUN functions******************************/

static void WD_Run()
{
    int res = 0;
    ilrd_uid_t res_uid = {0};
    PRINT_DEBUG("Wd entered WD_run func...\n");

    argv_g[0] = getenv(ENV_VAR_USER_FILE_NAME);
    res_uid = SchAddTask(sch_g, ContUserTask, NULL, time(NULL), 3);

    if (TRUE == UIDIsSame(res_uid, UIDGetBad()))
    {
        DestroyAll("Task adding error\n");
        return;
    }

    sem_post(sem_g);

    PRINT_DEBUG("running WD sched...\n");
    res = SchRun(sch_g);
    PRINT_DEBUG("WD sched is down, res = %d\n", res);

    if (ERROR == res)
    {
        perror("Error while running the sch, destroying the Sch and execv new User proc\n");
        SchDestroy(sch_g);

        if (ERROR == execv(argv_g[0], argv_g))
        {
            DestroyAll("ExecV error in WD_run\n");
            return;
        }
    }
    
    PRINT_DEBUG("WD_Destroy was called\n");

    SchDestroy(sch_g);
}

static void *UserRunSch(void *arg)
{
    (void)arg;
    int res = 0;
    PRINT_DEBUG("Entered user thread function..\n");

    while (TRUE)
    {

        res = SchRun(sch_g);
        PRINT_DEBUG("Users scheduler is down, res = %d\n", res);

        if (ERROR == res) /*WD is down*/
        {
            wait(NULL); /*picking up orphan WD process*/
            res = CreateWD();

            if (SUCCESS != res)
            {
                perror("Forking WD error\n");
                StopWD();
                break;
            }

            atomic_store(&counter, 0);
        }
        else
        {
            break;
        }
    }

    pthread_exit(NULL);
}

/******************************Common static functions*********************/
static int IsWDProc(const char *proc_name)
{
    PRINT_DEBUG("proc name is - %s\n", proc_name);

    return strcmp(proc_name, WD_ELF_PATH) == 0;
}

static void DestroyAll(char *error_msg)
{
    perror(error_msg);
    SchDestroy(sch_g);
    sem_close(sem_g);
}

