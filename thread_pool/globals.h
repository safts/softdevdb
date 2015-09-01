#ifndef _GLOBALS_
#define _GLOBALS_

#include <pthread.h>
#include "pool.h"

extern pthread_mutex_t mtx;
extern pthread_mutex_t mtx1;
extern pthread_cond_t cond_nonempty;
extern pthread_cond_t cond_nonfull;
extern pool_t ev_pool;

extern void* threadFunc(void* );

#endif
