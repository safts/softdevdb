#ifndef INDEX_IMPL_H
#define	INDEX_IMPL_H


#include "DataStructures/linkedList.h"
#include "index.h"

struct indexNode;
struct Entry;
struct EntryList;




#ifdef	__cplusplus
extern "C" {
#endif


struct EntryList {

	LinkedList<Entry>* list;
	Node<Entry> * current;

};

typedef struct EntryList EntryList;

struct Index {

	indexNode* headNode;
	MatchType type;
};

typedef struct Index Index;


#ifdef	__cplusplus
}
#endif

struct indexNode{

	Entry* vP;
	EntryList* list;
	indexNode* inRad;
	indexNode* offRad;
	int rad;

	indexNode(){

		vP=NULL;
		list=NULL;
		inRad=NULL;
		offRad=NULL;
		rad=-1;
	}



	~indexNode(){

		if(vP!=NULL)
			DestroyEntry(vP);
		if(list!=NULL)
			delete list;
		if(inRad!=NULL)
			delete inRad;
		if(offRad!=NULL)
			delete offRad;
	}
};

#endif	/* INDEX_IMPL_H */

