#include "core.h"
//#include "defn_impl.h"
#include "index_impl.h"
#include "index.h"
#include "defn_impl.h"
#include "DataStructures/hashTable.h"
//#include "thread_pool/globals.h"
#include "thread_pool/pool.h"
#include "thread_pool/job.h"
#include "scheduler.h"
#include "cache.h"



struct Qinfo{

	QueryID id;
	int size;
};

struct Result{

	DocID dID;
	int ** map;// [QueryID][theshpinaka][megethos]
	bool ** matches;
	int queriesN;

	Result(int queriesN,DocID d){

		this->dID=d;
		this->queriesN=queriesN;
		map=new int*[queriesN];
		for(int i=0;i<queriesN;i++){
			map[i]=new int[2];
			map[i][0]=-1;
			map[i][1]=-1;
		}
		matches=new bool*[queriesN];
		for(int i=0;i<queriesN;i++){
			matches[i]=new bool[5];
			for(int j=0;j<5;j++)
				matches[i][j]=false;
		}
	}

	Result(const Result& old){

		this->dID=old.dID;
		this->queriesN=old.queriesN;
		//this->map=new int*[queriesN];
		map=new int*[queriesN];
		for(int i=0;i<queriesN;i++){
			map[i]=new int[2];
			map[i][0]=old.map[i][0];
			map[i][1]=old.map[i][1];
		}
		matches=new bool*[queriesN];
		for(int i=0;i<queriesN;i++){
			matches[i]=new bool[6];
			for(int j=0;j<5;j++)
				matches[i][j]=old.matches[i][j];
		}
	}

	~Result(){
		if(map!=NULL){
			for(int i=0;i<queriesN;i++)
				delete [] map[i];
			delete []map;
		}
			if(matches!=NULL){
				for(int i=0;i<queriesN;i++)
					delete []matches[i];
				delete[] matches;
			}
	}

	bool operator==(const Result& r){
		if(this->dID == r.dID)
			return true;
		else
			return false;
	}

	bool operator!=(const Result& r){
			return !this->operator ==(r);
		}

	Result& operator=(const Result& r){
		if (this != &r) {
			if(this->map!=NULL){
				for(int i=0;i<this->queriesN;i++)
					delete map[i];
				delete map;
				this->map=new int*[r.queriesN];
				for(int i=0;i<r.queriesN;i++){
					this->map[i]=new int[2];
				}
			}
			if(this->matches!=NULL){
				for(int i=0;i<this->queriesN;i++)
					delete this->matches[i];
				delete this->matches;
				this->matches=new bool*[r.queriesN];
				for(int i=0;i<r.queriesN;i++){
					this->matches[i]=new bool[5];
					//this->matches[i]=new bool[6];
				}
			}
			for(int i=0;i<r.queriesN;i++){
				this->map[i][0]=r.map[i][0];
				this->map[i][1]=r.map[i][1];
			}
			for(int i=0;i<r.queriesN;i++){
				for(int j=0;j<5;j++)
					this->matches[i][j]=r.matches[i][j];
			}
		}
		return *this;
	}

	bool assign(QueryID id,int size){

		map[id-1][0]=size;
		return true;
	}

	bool match(QueryID id,int pos){

		matches[id-1][pos]=true;
		return true;
	}

	bool isMatch(QueryID id,int size){

		for(int i=0;i<size;i++)
			if(matches[id-1][i]==false)
				return false;
		return true;

	}

	bool getMatch(QueryID q){

		if(isMatch(q-1,map[q-1][0]))
			return true;
		return false;
	}
	void printMatches(){

		cout << "PRINTING MATCHES FOR DOCID: " <<dID << endl;
		for(int i=0;i<queriesN;i++){
			if(isMatch(i,map[i][1]))
				cout << "Query: " << map[i][0] << " matches the document" << endl;
		}
	}
};




struct indexStructures {

	int querN;
	hashTable<Entry>* exactMatch;
	Index* editMatch;
	Index* hamMatch[MAX_WORD_LENGTH-MIN_WORD_LENGTH];
	hashTable<Entry>* editHash;
	hashTable<Entry>* hamHash[MAX_WORD_LENGTH-MIN_WORD_LENGTH];
	LinkedList<Result>* results;
	LinkedList<Qinfo>* info;

	pool_t jobPool;

	scheduler sch;

	Cache* ind_cache;
};


pthread_mutex_t mtx;
pthread_mutex_t mtx1;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;
//pool_t ev_pool;


indexStructures iS;

enum ErrorCode InitializeIndex() {

	iS.exactMatch=new hashTable<Entry>;
	iS.editMatch=new Index;
	iS.editHash=new hashTable<Entry>;
	for(int i=0;i<MAX_WORD_LENGTH-MIN_WORD_LENGTH;i++){
		iS.hamMatch[i]=NULL;
		iS.hamHash[i]=NULL;
	}
	iS.querN=0;
	iS.results=new LinkedList<Result>;
	iS.info=new LinkedList<Qinfo>;
//	iS.results=NULL;


	initialize(&iS.jobPool);
	iS.sch.initialiseScheduler(&iS.jobPool);

	iS.ind_cache=(Cache*)malloc(sizeof(Cache));

	Listhptr l=NULL;
	Hashptr h=NULL;

	iS.ind_cache->nof=0;
	iS.ind_cache->list=l;
	iS.ind_cache->hasht=h;

	return EC_SUCCESS;
}




void InsertToHash(Entry* e,hashTable<Entry>* hash,QueryID& id,int pos,int dist){

	if(hash==NULL)
		return;
	if(e==NULL)
		return;
	Entry* found;
//	if(strcmp(e->word->word,"pgdma")==0)
//		cout << "EDW";
	if((found=hash->find(*e))!=NULL)
		found->payload->addToPl(id,pos,dist);
	else{
		e->payload->addToPl(id,pos,dist);
		hash->insert(*e);
	}
}




enum ErrorCode DOStartQuery(QUERY* q_info) {

//	QUERY* info;
//	Job* start=consumer(&iS.jobPool);
//	start->print();


//	QUERY* q_info=(QUERY*)start->info;
//	cout << "Called!" << endl;

	QueryID query_id=q_info->ID;
//	const char* query_str=q_info->QSTR;
	MatchType match_type=q_info->MT;
	unsigned int match_dist=q_info->MDIST;

//	q_info->print();
//	cout << "Query text: " << q_info->MT << endl;
	Query q;
	for(unsigned int i=0;i<q_info->q.wordN;i++){
		q.content[i]=new Word;
		*q.content[i]=*q_info->q.content[i];
	}
	q.matchDist=q_info->q.matchDist;
	q.id=q_info->q.id;
	q.type=q_info->q.type;
	q.wordN=q_info->q.wordN;
	//CreateQuery(query_id,query_str,match_type,match_dist,&q);
	if(match_type==MT_EXACT_MATCH){
		//cout << "Match me exact" << endl;

		int words=q.wordN;
		for(int i=0;i<words;i++){
			Entry e;
			CreateEntry(q.content[i],&e);
			InsertToHash(&e,iS.exactMatch,q.id,i,match_dist);
		}
	}
	else if(match_type==MT_HAMMING_DIST){
		//cout << "Match me hamming" << endl;

		int words=q.wordN;
		for(int i=0;i<words;i++){
			Entry e;
			CreateEntry(q.content[i],&e);
			int len=q.content[i]->length;
//			cout << len << endl;
			if(iS.hamHash[len-MIN_WORD_LENGTH]==NULL)
				iS.hamHash[len-MIN_WORD_LENGTH]=new hashTable<Entry>;
//			cout << "Vazw sto Hash to " <<e.word->word << endl;
			InsertToHash(&e,iS.hamHash[len-MIN_WORD_LENGTH],q.id,i,match_dist);
			DestroyEntry(&e);
		}

	}
	else if(match_type==MT_EDIT_DIST){
		//cout << "Match me edit" << endl;
		int words=q.wordN;
		for(int i=0;i<words;i++){
			Entry e;
			CreateEntry(q.content[i],&e);
			InsertToHash(&e,iS.editHash,q.id,i,match_dist);
		}
	}
	iS.querN++;

	Qinfo info;
	info.id=query_id;
	info.size=q.wordN;

	iS.info->addNode(info);

//	DestroyQuery(q);
//	delete start;
	return EC_SUCCESS;

}


enum ErrorCode StartQuery(QueryID query_id, const char* query_str,
		enum MatchType match_type, unsigned int match_dist) {

	Job start(ST_QUERY);
	QUERY* info = (QUERY*) start.info;

	info->ID=query_id;
	info->MDIST=match_dist;
	info->MT=match_type;
	CreateQuery(query_id,query_str,match_type,match_dist,&info->q);
	//info->QSTR=(const char*)query_str;

//	start.print();
	iS.sch.addJob(&start);

//	return DOStartQuery();
	//cout << "Vazw to query " << query_id << endl;
	return EC_SUCCESS;

}

int ExportfromHash(hashTable<Entry>& hash,EntryList* entryList){

	CreateEntryList(entryList);
	if(hash.recNum>0){

		for(int i=0;i<hash.bucketNum;i++){
			if(hash.buckets[i]==NULL)
				continue;
			Node<Entry>* it=hash.buckets[i]->headPtr;
			for(int k=0;k<hash.buckets[i]->count;k++){
				Entry e;
				//e=it->data;
				CreateEntry(it->data->word,&e);
				AddEntry(entryList,it->data);
				DestroyEntry(&e);
				it=it->next;
			}
		}
	}
	else
		return 0;
	return hash.recNum;
}

void CreateDocResults(DocID id,EntryList* list,Result* result){

	//result(iS.info->count,id);
	Node <Qinfo>* it=iS.info->headPtr;
	for(int i=0;i<iS.info->count;i++){
		if(result->assign(it->data->id,it->data->size)==false)
			cout << "Problem!" << endl;
		it=it->next;
	}
	Node<Entry>* it1;
	it1=list->list->headPtr;
	for(int i=0;i<list->list->count;i++){
		int plcount=it1->data->payload->queries->count;
		Node<pInfo>* pit=it1->data->payload->queries->headPtr;
		for(int j=0;j<plcount;j++){
			if(result->match(pit->data->id,pit->data->pos)==false)
				cout << "Problem(2)! " << endl;
			pit=pit->next;
		}
		it1=it1->next;
	}

}

void UpdateDocResults(DocID id,EntryList* list,Result* result){

	Node<Entry>* it1;
	it1=list->list->headPtr;
	for(int i=0;i<list->list->count;i++){
		int plcount=it1->data->payload->queries->count;
		Node<pInfo>* pit=it1->data->payload->queries->headPtr;
		for(int j=0;j<plcount;j++){
			if(result->match(pit->data->id,pit->data->pos)==false)
				cout << "Problem(2)! " << endl;
			pit=pit->next;
		}
		it1=it1->next;
	}
}

void BuildIndexes(){

	EntryList editEntries;
	EntryList hamEntries;
	EntryList exactEntries;

	if(ExportfromHash(*iS.editHash,&editEntries)>0){
		BuildEntryIndex(&editEntries,MT_EDIT_DIST,iS.editMatch);
	}
	else{
		delete iS.editMatch;
		iS.editMatch=NULL;
	}
	for(int i=0;i<MAX_WORD_LENGTH-MIN_WORD_LENGTH;i++){
		if(iS.hamHash[i]==NULL)
			continue;
		if(ExportfromHash(*iS.hamHash[i],&hamEntries)>0){

			//cout << "Ftiaxnw hamindex gia length " <<i+MIN_WORD_LENGTH << endl;
			iS.hamMatch[i]=new Index;
			BuildEntryIndex(&hamEntries,MT_HAMMING_DIST,iS.hamMatch[i]);
			DestroyEntryList(&hamEntries);
		}

		else
			iS.hamMatch[i]=NULL;
		delete iS.hamHash[i];
		iS.hamHash[i]=NULL;
	}

	if(ExportfromHash(*iS.exactMatch,&exactEntries)==0){
		delete iS.exactMatch;
		iS.exactMatch=NULL;
	}

	DestroyEntryList(&editEntries);
	DestroyEntryList(&exactEntries);


}


enum ErrorCode DOMatchDocument(DOC* start){

	DOC* document=(DOC*)start;

	DocID doc_id = document->ID;
	if(doc_id%100 == 0)
			cout << "Kanw match to document " << doc_id << endl;
//	Document document;
//	CreateDocument(doc_id,doc_str,&document);
	EntryList hamresult;
	CreateEntryList(&hamresult);
	EntryList editresult;
	CreateEntryList(&editresult);
	EntryList exactresult;
	CreateEntryList(&exactresult);

	Node<Word>* it=document->doc->content.headPtr;




	for(int k=0;k<document->doc->wordN;k++){

//		int pos;
//		if(iS.ind_cache->nof==0 || (pos=foundinHash(iS.ind_cache->hasht,it->data))==0){
				//iS.ind_cache=insertcache(iS.ind_cache,&word,&entry_list);
			int wordL=it->data->length;
			if(iS.hamMatch[wordL-MIN_WORD_LENGTH]!=NULL)
				LookupEntryIndex(it->data,3,iS.hamMatch[wordL-MIN_WORD_LENGTH],&hamresult);
			if(iS.editMatch!=NULL)
				LookupEntryIndex(it->data,3,iS.editMatch,&hamresult);
			Entry* found;
			Entry e;
			CreateEntry(it->data,&e);
			if((found=iS.exactMatch->find(e))!=NULL){
				AddEntry(&hamresult,found);
			}
//		}
//		else{
//			if(pos>0){
//				cac=updatecache(iS.ind_cache,&words[i],pos);
//			}
//		}


		it=it->next;
	}

	it=document->doc->content.headPtr;

//	for(int k=0;k<document->doc->wordN;k++){

//		it=it->next;
//	}
//	Node<Entry>* temp=editresult.list->headPtr;
//	for(int k=0;k<editresult.list->count;k++){
//		AddEntry(&hamresult,temp->data);
//		temp=temp->next;
//	}

	Result r(iS.info->count,doc_id);
	CreateDocResults(doc_id,&hamresult,&r);
	pthread_mutex_lock(&mtx);
	iS.results->addNode(r);
	pthread_mutex_unlock(&mtx);

	DestroyEntryList(&hamresult);
	DestroyEntryList(&editresult);
	DestroyEntryList(&exactresult);

	delete start;

	return EC_SUCCESS;
}

enum ErrorCode MatchDocument(DocID doc_id, const char* doc_str) {

	static int i=0;
	if(i==0){

		iS.sch.addBarrierJobs();

		BuildIndexes();
	}
	i++;

//	iS.sch.addBarrierJobs();

	Job start(MATCH_DOC);
	start.info=new DOC;
	DOC* info = (DOC*) start.info;

	info->ID=doc_id;
	Document* document=new Document;
	CreateDocument(doc_id,doc_str,document);
	info->doc=document;
	iS.sch.addJob(&start);

	return EC_SUCCESS;
			//DOMatchDocument(&start);
}

enum ErrorCode GetNextAvailRes(DocID* p_doc_id, unsigned int* p_num_res,
		QueryID** p_query_ids) {
	if(iS.results==NULL)
		return EC_NO_AVAIL_RES;
	if(iS.results->count==0)
		return EC_NO_AVAIL_RES;


	iS.sch.addBarrierJobs();

	Node<Result>* temp=iS.results->headPtr;
	LinkedList<QueryID> lresults;


	//CreateEntryList(&lresults);

	*p_doc_id=temp->data->dID;

	for(int i=temp->data->queriesN;i>=1;i--){
		if(temp->data->isMatch(i,temp->data->map[i-1][0]))
			lresults.addNode(i);
	}

	*p_num_res=(unsigned)lresults.count;

	Node <QueryID>* it=lresults.headPtr;
	*p_query_ids=(QueryID*)malloc(lresults.count*sizeof(QueryID)); //QueryID[lresults.count]; //(QueryID*)malloc(lresults.count*sizeof(QueryID));
	for(int i=0;i<lresults.count;i++){
		//cout << "To data einai " << *(it->data) << endl;
		(*p_query_ids)[i]=*(it->data);
		it=it->next;
	}



//	pthread_mutex_lock(&mtx1);
	iS.results->deleteNode(*(temp->data));
//	pthread_mutex_lock(&mtx1);

	lresults.deleteAllNodes();

	return EC_SUCCESS;

}

enum ErrorCode DestroyIndex() {

	if(iS.editHash!=NULL){

	}
	if(iS.exactMatch!=NULL){

	}
	for(int i=0;i<MAX_WORD_LENGTH-MIN_WORD_LENGTH;i++){
		if(iS.hamMatch[i]==NULL)
			delete iS.hamMatch[i];
		if(iS.hamHash[i]==NULL)
			delete iS.hamHash[i];
	}
	if(iS.results!=NULL){
		iS.results->deleteAllNodes();
		delete iS.results;
	}
	if(iS.info!=NULL){
		delete iS.info;
	}
	free(iS.ind_cache);
	return EC_SUCCESS;
}


void* threadFunc(void *){

	void * p=NULL;

	while(true){
		Job* start=consumer(&iS.jobPool);

		if(start->type==ST_QUERY){
			QUERY* q_info=(QUERY*)start->info;
			pthread_mutex_lock(&mtx);
			DOStartQuery(q_info);
			pthread_mutex_unlock(&mtx);
		}
		if(start->type==MATCH_DOC){
//			cout << "Tha kanw match doc" << endl;
			DOC* d=(DOC*)start->info;
//			pthread_mutex_lock(&mtx);
			DOMatchDocument(d);
//			pthread_mutex_unlock(&mtx);
		}
		if(start->type==BARRIER){

//			cout << "Kanw wait se barrieraaaa " << endl;
			pthread_barrier_wait(&iS.sch.barrier);
		}

		delete start;
	}

	return p;

}

