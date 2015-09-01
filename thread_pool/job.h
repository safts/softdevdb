#ifndef _JOB_
#define _JOB_

#include "../core.h"
#include "../defn_impl.h"

enum j_type {ST_QUERY,MATCH_DOC,GET_RES,BARRIER,N_DEF};

struct QUERY{

	QueryID ID;
	MatchType MT;
	unsigned int MDIST;
	Query q;

	void print();
};

struct DOC{

	DocID ID;
	Document * doc;

	void print();
};


struct Job{
	
	public:
	
		j_type type;
		void* info;
//		char message[256];
//		int fd;
		
		Job(j_type=N_DEF);
		Job(Job& );
		Job& operator=(const Job& );
		~Job();
	
		void print();
};

#endif


