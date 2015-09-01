#ifndef _POOL_
#define _POOL_

#define POOL_SIZE 100

#include "job.h"

struct pool_t {
	Job data[POOL_SIZE];
	int start;
	int end;
	int count;
	
	pool_t();
	~pool_t();
};


void initialize(pool_t * );
void place(pool_t * , Job*);
Job * obtain(pool_t * );
void producer(pool_t * ,Job* );
Job * consumer(pool_t * );

#endif
