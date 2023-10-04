#ifndef __ILRD_OL139_40_WD_H__
#define __ILRD_OL139_40_WD_H__

#include <stddef.h> /* size_t */

int WD_Start(int argc, char *argv[], size_t thresh_hold, size_t interval);
void StopWD();
#endif

