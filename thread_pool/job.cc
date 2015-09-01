#include "job.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

void QUERY::print(){

	cout << "QueryID   : " << ID <<endl;
	cout << "MatchType : ";
	if(MT==MT_EXACT_MATCH)
		cout << "MT_EXACT_MATCH" << endl;
	else if(MT==MT_HAMMING_DIST)
		cout << "MT_HAMMING_DIST" << endl;
	else if(MT==MT_EDIT_DIST)
		cout << "MT_EDIT_MATCH" << endl;
	else
		cout << "ERROR!" << endl;
	cout << "Dist      : " << MDIST << endl;
}

void DOC::print(){

	cout << "DocID : " << ID << endl;
}


Job::Job(j_type type){

	this->type=type;
	if(type==ST_QUERY){
		info = new QUERY;
	}
	else if(type==MATCH_DOC){

	}
	else if(type==GET_RES){

	}
	else
		info=NULL;
};

Job& Job::operator=(const Job& old){
	
	if(&old!=this){
		this->type=old.type;
		if(type==ST_QUERY && old.info!=NULL){
			if(this->info!=NULL)
				delete (QUERY*)this->info;
			this->info=(QUERY*)old.info;
		}
		else if(type==MATCH_DOC && old.info!=NULL){
			this->info=(DOC*)old.info;
		}
		else if(type==GET_RES){

		}

	}
	return *this;
}

Job::~Job(){

	if(type==ST_QUERY && info!=NULL){
//		QUERY* p=(QUERY*) info;
//		delete p;
	}
	else if(type==MATCH_DOC){

		//delete (DOC*) info;
	}
	else if(type==GET_RES){

	}
//	else
//		info=NULL;

};


Job::Job(Job& old){

//	memset(this->message,0,sizeof(this->message));
//	strcpy(this->message,old.message);
	this->type=old.type;
	if(type==ST_QUERY && old.info!=NULL){
//		QUERY* p=(QUERY*) info;
//		delete p;
		this->info=(QUERY*)old.info;
	}
	else if(type==MATCH_DOC){

		this->info=(DOC*) old.info;
	}
	else if(type==GET_RES){

	}

//	cout << "Copy constr: " ;
//	this->print();
//	cout << "Copy constr: " ;
}


void Job::print(){

	if(type==ST_QUERY){
		QUERY* p=(QUERY*)info;
		p->print();
	}
	else if(type==MATCH_DOC){
		DOC* p=(DOC*)info;
		p->print();
	}
	else if(type==GET_RES){

	}
	
}
