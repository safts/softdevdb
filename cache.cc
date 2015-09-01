#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include "index_impl.h"
#include "cache.h"
#define CMAX 10
#define BNUM 5
#define MAX_WORD_LENGTH 31
#define NUMBER_OF_WORDS 102

using namespace std;



//char *contents[]={"schofield", "diamondback", "udar", "taurus", "help", "file", "pepperbox", "gnom", "udar","information", "bodeo", "charter", "contents",
//"terminator", "organized", "autorevolver", "rossi", "content", "bulldog", "http", "trooper", "defender", "ruger", "silent", "qspr", "redhawk", "king",
//"windicator", "mateba", "nagant", "judge", "lawman", "magnum", "wiki", "wesson", "post", "smith", "buntline", "bull", "revolver", "arminius", "raging",
//"striker", "wikipedia", "johnson", "mauser", "public", "detective", "astra", "fosbery", "single", "colt", "centennial", "python", "korth", "webley",
//"action", "combat", "iver", "zulaica", "letter", "turner", "automatic", "double", "ontology", "ladysmith", "cobra", "reichsrevolver", "famae", "maximum",
//"manufacturer", "anaconda", "vaquero", "this", "weihrauch", "model", "security", "izmech", "army", "super", "blackhawk", "dbpedia", "arms", "introducing",
//"manurhin", "peacemaker", "abstract", "rast", "gasser", "speed", "bearcat", "resource", "edit", "list", "paterson", "armsel", "section", "enfield", "service",
//"special", "revolvers", "navy"};



//int main(int argc,char *argv[]){
//	int pos,i;
//	Cacheptr cac;
//	Listhptr l;
//	Hashptr h;
//	  Word words[NUMBER_OF_WORDS];
//	  Entry entry;
//	  EntryList entry_list;
//	  CreateEntryList(&entry_list);
//	  for (i=0; i<NUMBER_OF_WORDS; ++i) {
//	    CreateWord(contents[i], &words[i]);
//	    CreateEntry(&words[i], &entry);
//	    AddEntry(&entry_list, &entry);
//	  }
//	h=NULL;
//	l=NULL;
//	cac=(Cache*)malloc(sizeof(Cache));
//	cac->nof=0;
//	cac->list=l;
//	cac->hasht=h;
//	for(i=2;i<15;i++){
//		if(cac->nof==0){
//			cac=insertcache(cac,&words[i],&entry_list);
//		}else{
//			pos=foundinHash(cac->hasht,&words[i]);
//			//cout << "word value " << (unsigned char*)&words[i].word << endl;
//			if(pos>0){
//				cac=updatecache(cac,&words[i],pos);
//			}else{
//				cac=insertcache(cac,&words[i],&entry_list);
//			}
//		}
//	}
//	cout << "Last in cache " << cac->list->tail->elem->elem->word->word << endl;
//	deletecache(cac);
//	cout << "Bghke" << endl;
//return 0;
//}
Cacheptr insertcache(Cacheptr cac,Word *w,EntryList *el){
	Word nw;
	//CacheNode de;
	//Prepei na a) antigrapsw to word kai na valw to antigrafo tou sto node
	//			b) antigrapsw kathe node ths entrylist kai na ta perasw se kathe bucket
	if(cac->nof==0){
		cac->hasht=inserthash(cac->hasht,w,el);
		cac->list=insertlist(cac->list,cac->hasht->buckets[(hashf((unsigned char*)w->word))%BNUM]->head);
		cac->nof++;
	}else{
		if(cac->nof<CMAX){
			cac->hasht=inserthash(cac->hasht,w,el);
			cac->list=insertlist(cac->list,cac->hasht->buckets[(hashf((unsigned char*)w->word))%BNUM]->head);
			cac->nof++;
		}else if(cac->nof>=CMAX){
			cout << cac->list->tail->elem->elem->word->word << endl;
			CreateWord(cac->list->tail->elem->elem->word->word,&nw);
			cac->hasht=inserthash(cac->hasht,w,el);
			cac->list=insertlist(cac->list,cac->hasht->buckets[(hashf((unsigned char*)w->word))%BNUM]->head);
			cac->list=deleteflist(cac->list);
			cac->hasht->buckets[(hashf((unsigned char*)nw.word))%BNUM]=deletefbucket(cac->hasht->buckets[(hashf((unsigned char*)nw.word))%BNUM],&nw);
		}
	}
return cac;
}

Cacheptr updatecache(Cacheptr cac,Word *w,int pos){
	cac->list=updatelist(cac->list,cac->hasht->buckets[(hashf((unsigned char*)w->word))%BNUM],pos);
	return cac;
}

Listhptr deleteflist(Listhptr lh){
	Listptr temp;
	lh->max--;
	lh->tail->elem->match=NULL;
	lh->tail->prev->next=NULL;
	temp=lh->tail;
	lh->tail=temp->prev;
	free(temp);
return lh;
}

Blisthptr deletefbucket(Blisthptr bh,Word *w){
	int i;
	Blistptr temp;
	bh->it=bh->head;
	if(bh->numof==1){
		deleteCnode(bh->it->elem);
		temp=bh->head;
		bh->head=NULL;
		bh->tail=NULL;
		bh->it=NULL;
		free(temp);
		return NULL;
	}
	for(i=0;i<(bh->numof);i++){
		if(strcmp(bh->it->elem->word->word,w->word)==0){
			if(strcmp(bh->it->elem->word->word,bh->tail->elem->word->word)==0){
				deleteCnode(bh->it->elem);
				bh->it->prevb->nextb=NULL;
				bh->tail=bh->tail->prevb;
				bh->tail->prevb=NULL;
				temp=bh->tail;
				temp=bh->it;
				bh->it=bh->head;
				bh->numof--;
				free(temp);
			}else{
				deleteCnode(bh->it->elem);
				bh->it->prevb->nextb=bh->it->nextb;
				bh->it->nextb->prevb=bh->it->prevb;
				bh->it->nextb=NULL;
				bh->it->prevb=NULL;
				temp=bh->it;
				bh->numof--;
				free(temp);
				break;
			}
		}else{
			bh->it=bh->it->nextb;
		}
	}
return bh;
}

Listhptr updatelist(Listhptr lh,Blisthptr bh,int pos){
	int i;
	bh->it=bh->head;
	for(i=1;i<pos;i++){
		bh->it=(bh->it)->nextb;
	}
	if(bh->it->match->prev==NULL){
		bh->it=bh->head;
	}else if(bh->it->match->next==NULL){
		bh->it->match->prev->next=NULL;
		bh->it->match->next=lh->head;
		lh->tail=bh->it->match->prev;
		lh->head->prev=bh->it->match;
		bh->it->match->prev=NULL;
		bh->it=bh->head;
	}else {
		bh->it->match->prev->next=bh->it->match->next;
		bh->it->match->next->prev=bh->it->match->prev;
		lh->head->prev=bh->it->match;
		bh->it->match->next=lh->head;
		lh->head=bh->it->match;
		bh->it=bh->head;
	}
return lh;
}

int foundinBucket(Blisthptr bh,Word *w){
	int i;
	bh->it=bh->head;
	if(bh->numof==1){
		if(strcmp(bh->head->elem->word->word,w->word)==0){
			return 1;
		}
	}else{
		for(i=0;i<bh->numof;i++){
			if(strcmp(bh->it->elem->word->word,w->word)==0){
				return i+1;
			}else{
				bh->it=(bh->it)->nextb;
			}
		}
	}	
return 0;
}

int foundinHash(Hashptr h,Word *w){
	int r;
	if(h->buckets[(hashf((unsigned char*)w->word))%BNUM]==NULL){
		return 0;
	}
	r=foundinBucket(h->buckets[(hashf((unsigned char*)w->word))%BNUM],w);
	return r;
}

Listhptr insertlist(Listhptr l,Blistptr b){
	Listptr temp;
	temp=(ListNode*)malloc(sizeof(ListNode));
	temp->next=NULL;
	temp->prev=NULL;
	temp->elem=b;
	if(l==NULL){
		l=(ListHead*)malloc(sizeof(ListHead));
		l->max=1;
		l->head=temp;
		l->tail=temp;
		b->match=temp;
	}else{
		b->match=temp;
		temp->next=l->head;
		l->head->prev=temp;
		l->head=temp;
		l->max=l->max+1;
	}
return l;
}

unsigned long hashf(unsigned char *str){
	unsigned long h = 5381;
	int c;
	while ((c = *str++))
		h= ((h << 5) + h) + c; /* hash * 33 + c */

	return h;
}

Hashptr inserthash(Hashptr has,Word *w,EntryList *el){
	int i;
	if(has==NULL){
		Blisthptr *bar;
		bar=(Blisthptr*)malloc(BNUM*sizeof(Blisthptr));
		has=(Hashtable*)malloc(sizeof(Hashtable));
		has->hn=BNUM;
		has->buckets=bar;
		for(i=0;i<BNUM;i++){
			has->buckets[i]=NULL;
		}
	}
	has->buckets[(hashf((unsigned char*)w->word))%BNUM]=insertbucket(has->buckets[(hashf((unsigned char*)w->word))%BNUM],w,el);
return has;
}

Blisthptr insertbucket(Blisthptr bh,Word *w,EntryList *el){
	CacheNode* cn = (CacheNode*)malloc(sizeof(CacheNode));

	cn->word=new Word;
	CreateWord(w->word,cn->word);

	cn->entrylist=new EntryList;
	CreateEntryList(cn->entrylist);

	Node <Entry> * it=el->list->headPtr;

	for(int i=0;i<el->list->count;i++){
		AddEntry(cn->entrylist,it->data);
		it=it->next;
	}
	Blistptr f;
	f=(Blistnode*)malloc(sizeof(Blistnode));
	f->prevb=NULL;
	f->nextb=NULL;
	f->elem=cn;
	if(bh==NULL){
		bh=(Blisthead*)malloc(sizeof(Blisthead));
		bh->head=f;
		bh->tail=f;
		bh->it=bh->head;
		bh->numof=1;
	}else{
		f->nextb=bh->head;
		bh->head->prevb=f;
		bh->head=f;
		bh->it=bh->head;
		bh->numof=bh->numof+1;
	}
return bh;
}
void deleteCnode(CacheNode *cn){
	delete cn->word;
	DestroyEntryList(cn->entrylist);
}
void deleteChash(Hashptr h){
	for(int i=0;i<h->hn;i++){
		if(h->buckets[i]!=NULL){
			//cout << "kalhspera " << h->buckets[i]->numof << endl;
			for(int k=0;k<h->buckets[i]->numof;k++){
				if(h->buckets[i]->head!=NULL){
					h->buckets[i]->it=h->buckets[i]->head;
					h->buckets[i]->head=h->buckets[i]->head->nextb;
					deleteCnode(h->buckets[i]->it->elem);
					free(h->buckets[i]->it);
				}else{
					free(h->buckets[i]->tail);
					deleteCnode(h->buckets[i]->tail->elem);
				}
			}
			free(h->buckets[i]);
		}
	}
}

void deleteClist(Listhptr lh){
	Listptr it;
	for(int i=0;i<lh->max-1;i++){
			it=lh->head;
			lh->head=lh->head->next;
			free(it);
		}
	free(lh->head);
}
void deletecache(Cacheptr c){
	deleteChash(c->hasht);
	deleteClist(c->list);
	free(c);
}
