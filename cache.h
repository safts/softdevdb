/*
 * cache.h
 *
 *  Created on: Mar 25, 2014
 *      Author: sergios
 */

#ifndef CACHE_H_
#define CACHE_H_

typedef struct lnode *Listptr; //node listas
typedef struct lhead *Listhptr;
typedef struct blnode *Blistptr;
typedef struct hash *Hashptr;
typedef struct blhead *Blisthptr;
typedef struct cache *Cacheptr;

typedef struct cnode{
Word *word;
EntryList *entrylist;
}CacheNode;

typedef struct lnode{
	Blistptr elem;
	Listptr prev;
	Listptr next;
}ListNode;

typedef struct lhead{
	int max;
	Listptr head;
	Listptr tail;
}ListHead;

typedef struct blnode{
	CacheNode *elem;
	Listptr match;
	Blistptr nextb;
	Blistptr prevb;
}Blistnode;

typedef struct blhead{
	int numof;
	Blistptr it;
	Blistptr head;
	Blistptr tail;
}Blisthead;

typedef struct hash{
	int hn;
	Blisthptr *buckets;
}Hashtable;

typedef struct cache{
	int nof;
	Hashptr hasht;
	Listhptr list;
}Cache;

void deleteCnode(CacheNode *);
Cacheptr updatecache(Cacheptr,Word *,int);
Cacheptr insertcache(Cacheptr,Word *,EntryList *);
void deleteChash(Hashptr);
void deleteClist(Listhptr);
void deletecache(Cacheptr);
Hashptr inserthash(Hashptr,Word *,EntryList *);
Listhptr insertlist(Listhptr ,Blistptr);
Listhptr updatelist(Listhptr,Blisthptr,int);
Blisthptr insertbucket(Blisthptr,Word *,EntryList *);
Blisthptr deletefbucket(Blisthptr,Word *);
Listhptr deleteflist(Listhptr);
int foundinHash(Hashptr,Word *);
int foundinBucket(Blisthptr,Word *);
unsigned long hashf(unsigned char *str);


#endif /* CACHE_H_ */
