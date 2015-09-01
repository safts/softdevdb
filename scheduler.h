#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "thread_pool/pool.h"
#include "thread_pool/globals.h"

struct scheduler{

	pool_t* pool;

	int threadsN;

	pthread_t* threads;

	pthread_barrier_t barrier;

	void initialiseScheduler(pool_t*);

	void addJob(Job* );
	void addBarrierJobs();

	Job* getJob(j_type ); //must be deleted afterwards

	void destroyScheduler();

};



#endif /* SCHEDULER_H_ */
