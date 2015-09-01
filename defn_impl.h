#ifndef DEFN_IMPL_H
#define	DEFN_IMPL_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "./DataStructures/linkedList.h"
#include "./DataStructures/hashTable.h"

#ifdef	__cplusplus
extern "C" {
#endif

struct Word {

	char word[MAX_WORD_LENGTH];
	int length;

	Word();

	Word(const Word& );
	bool operator==(const Word& ) const;
	bool operator!=(const Word& );
	Word& operator=(const Word& );

	~Word();

};



struct Query {

	Word* content[MAX_QUERY_WORDS];
	unsigned int wordN;
	MatchType type;
	QueryID id;
	int matchDist;

//	Query();

	~Query();
};



struct Document {

	LinkedList<Word> content;
	int wordN;
	DocID id;

//	Document();

	~Document();
};

#ifdef	__cplusplus
}
#endif

ostream& operator<< (ostream& , const Word&);


#endif	/* DEFN_IMPL_H */
