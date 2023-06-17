#ifndef _TASKS_H
#define _TASKS_H

#include "ActiveObject.h"


int AOTask1(void *task);

int AOTask2(void *task);

int AOTask3(void *task);

int AOTask4(void *task);

void sigint_handler(int signum);

#endif // _TASKS_H
