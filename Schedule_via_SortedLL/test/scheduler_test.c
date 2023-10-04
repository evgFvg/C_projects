#include "scheduler.h"

#include <stddef.h>    /* size_t */
#include <assert.h>
#include <stdio.h> /*printf*/
#include <stdlib.h> /*system*/

static int PrintNumberX2(void *num);
static int OpenYouTube(void *num);
static int UserStopFunc(void *param);
void TestCreateDestroy(void);
void TestAddRemoveClear(void);
void TestSTartStop(void);
void TestInterval(void);

int main()
{
    TestCreateDestroy();
    TestAddRemoveClear();
    TestSTartStop();
    TestInterval();

    printf("All tests have been passed\n");
    
    return 0;
}

/*********************Test functions**************/

void TestCreateDestroy(void)
{
    sch_t *new_sch = SchCreate();

    assert(1 == SchIsEmpty(new_sch));
    assert(0 == SchSize(new_sch));

    SchDestroy(new_sch);
}

void TestAddRemoveClear(void)
{
    sch_t *new_sch = SchCreate();
    int test_num = 7;
    ilrd_uid_t res_uid = {0};

    res_uid = SchAddTask(new_sch, PrintNumberX2, &test_num, time(NULL), 0);

    assert(0 == SchIsEmpty(new_sch));
    assert(1 == SchSize(new_sch));

    SchAddTask(new_sch, PrintNumberX2, &test_num, time(NULL), 0);
    res_uid = SchAddTask(new_sch, PrintNumberX2, &test_num, time(NULL), 0);

   assert(3 == SchSize(new_sch));

    SchRemoveTask(new_sch, res_uid);

    assert(2 == SchSize(new_sch));

    SchRemoveTask(new_sch, UIDGetBad());

    assert(2 == SchSize(new_sch));

    SchClear(new_sch);

    assert(0 == SchSize(new_sch));
    assert(1 == SchIsEmpty(new_sch));

    SchDestroy(new_sch);
}

void TestSTartStop(void)
{
    sch_t *new_sch = SchCreate();
    int test_num = 7;
    int test_num2 = 9;

    SchAddTask(new_sch, OpenYouTube, &test_num, time(NULL) + 5, 0);
    SchAddTask(new_sch, PrintNumberX2, &test_num2, time(NULL) + 10, 0);
    SchAddTask(new_sch, UserStopFunc, new_sch,time(NULL) + 15, 0);

    SchAddTask(new_sch, PrintNumberX2, &test_num, time(NULL) + 20, 0);
    SchAddTask(new_sch, PrintNumberX2, &test_num2, time(NULL) + 25, 0);

    assert(5 == SchSize(new_sch));

    SchRun(new_sch);

    assert(2 == SchSize(new_sch)); /*2 tasks left after stop*/

    SchDestroy(new_sch);
}

void TestInterval(void)
{
    sch_t *new_sch = SchCreate();
    int test_num = 7;
    int test_num2 = 9;

    SchAddTask(new_sch, PrintNumberX2, &test_num, time(NULL) + 5, 0);
    SchAddTask(new_sch, PrintNumberX2, &test_num2, time(NULL) + 10, 3);
    SchAddTask(new_sch, UserStopFunc, new_sch, time(NULL) + 20, 0);

    SchRun(new_sch);
    SchDestroy(new_sch);
}

/********************************Static functions********************************/

static int PrintNumberX2(void *num)
{
    int res = *(int *)num;
    printf("%d x 2 equals to %d\n", res, res * 2);
    
    return 0;
}

static int OpenYouTube(void *num)
{
    char* url = "https://www.youtube.com/@RottenTomatoesTRAILERS";
    char command[1024];
    int res = 0;
    (void)num;
    
    printf("Opening youtube\n");
    
    sprintf(command, "xdg-open %s > /dev/null 2>&1 &", url);
    res = system(command);
    (void)res;
    
    return 0;
}

static int UserStopFunc(void *param)
{
    sch_t *sch = (sch_t *)param;
    SchStop(sch);
    return 0;
}
