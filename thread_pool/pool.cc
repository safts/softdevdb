#include "pool.h"
#include "globals.h"
#include <stdio.h>
#include <iostream>

using namespace std;

pool_t::pool_t() {

	this->start = 0;
	this->end = -1;
	this->count = 0;
}

pool_t::~pool_t() {

	int i;
//	cout << "Pool destroyed" << endl;
	//for (i = 0; i < count; i++) {
//		cout << "Pool destroyed" << endl;
//		printf(" queue %d : ", i);
//		data[i].print();
//		delete (data[i]);
	//}
}

void initialize(pool_t * pool) {

	pool->start = 0;
	pool->end = -1;
	pool->count = 0;
}

void place(pool_t * pool, Job* job) {

	pthread_mutex_lock(&mtx);
	Job* mess = new Job(*job);
	//*mess = job;
	while (pool->count >= POOL_SIZE) {
//		printf(">> Found Buffer Full \n");
		pthread_cond_wait(&cond_nonfull, &mtx);
//		exit(1);
	}
	pool->end = (pool->end + 1) % POOL_SIZE;
	pool->data[pool->end] = *mess;
	pool->count++;
//	std::cout << "Pool place: " ;
//	pool->data[pool->end].print();
	delete mess;
	pthread_mutex_unlock(&mtx);
}

Job* obtain(pool_t * pool) {

	Job* mess = new Job(N_DEF);
	pthread_mutex_lock(&mtx);
	while (pool->count <= 0) {
//		printf(">> Found Buffer Empty \n");
		pthread_cond_wait(&cond_nonempty, &mtx);
	}
	(*mess) = pool->data[pool->start];
	pool->start = (pool->start + 1) % POOL_SIZE;
	pool->count--;
	pthread_mutex_unlock(&mtx);
	return mess;
}

void producer(pool_t * ev_pool, Job* message) {

	place(ev_pool, message);
	pthread_cond_signal(&cond_nonempty);
}

Job * consumer(pool_t * ev_pool) {

	Job* message = obtain(ev_pool);
	pthread_cond_signal(&cond_nonfull);
	return message;
}

