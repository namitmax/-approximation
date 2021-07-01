#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <pthread.h>
#include <utility>
#include "params.h"
#include "init_iter_alg.h"

#define MAX        50
#define MAX_THREAD 50
#define GLOBAL_MAX 20

void *
execute (void * arg);

#endif // EXECUTOR_H
