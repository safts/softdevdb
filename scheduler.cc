#include "scheduler.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

#include "thread_pool/globals.h"

using namespace std;

void scheduler::initialiseScheduler(pool_t* pool) {

	this->pool = pool;

	threadsN = 10;

	threads = (pthread_t*) malloc(threadsN * sizeof(pthread_t));

	pthread_mutex_init(&mtx, 0);
	pthread_mutex_init(&mtx1, 0);
	pthread_cond_init(&cond_nonempty, 0);
	pthread_cond_init(&cond_nonfull, 0);

	int err;
	for (int i = 0; i < threadsN; i++)
		if ((err = pthread_create(&threads[i], NULL, threadFunc, &pool)) != 0) {
//			perror("pthread_create", err);
			exit(1);
		}
}

void scheduler::addJob(Job* job) {

	producer(pool, job);
}

void scheduler::addBarrierJobs() {

	pthread_barrier_init(&barrier,NULL,threadsN+1);

	Job barrierJ;

	barrierJ.type=BARRIER;
	for(int i=0;i<threadsN;i++)
		producer(pool,&barrierJ);

	//consumer()

	pthread_barrier_wait(&barrier);

//	cout << "Eimai o scheduler kai perasa to barrier "<< endl;
	pthread_barrier_destroy(&barrier);
}

void scheduler::destroyScheduler() {

	pthread_cond_destroy(&cond_nonempty);
	pthread_cond_destroy(&cond_nonfull);
	pthread_mutex_destroy(&mtx);
	pthread_mutex_destroy(&mtx1);

	void* p;
//	for(int i=0;i<threadsN;i++)
//		pthread_join(threads[i],&p);

	free(threads);
}
