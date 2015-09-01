#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "Node.h"
#include "linkedList.h"

/*
#include <string>


class LinkedList;
class FullData;
*/
template <class L>
class hashTable{

	public:
		LinkedList<L> ** buckets;
		int recNum;
		int bucketNum;
		int collisions;

		hashTable(int =3000);

		void insert(const L& );
		bool remove(const L& );
		L* find(const L& );
		int modify(const L&, const L&);
		void print();
		void report();

		// This hashing algorithm (djb2)
		// was found in http://www.cse.yorku.ca/~oz/hash.html
		unsigned long hash(unsigned char *);

		~hashTable();
};

#include "hashTable.cc"

#endif /* HASHTABLE_H_ */
