#include <stdlib.h>
#include <cstring>
#include <cstring>
#include "core.h"
#include "index.h"
#include "defn_impl.h"
#include "defn.h"
#include "index_impl.h"
#include "indexNode.h"
#include <cmath>
#include <iomanip>

using namespace std;

bool Entry::operator==(const Entry& e) const{
	if(this->word==NULL || e.word==NULL)
		return true;
	else if(*(this->word)==*(e.word)){
		//if(this->payload==NULL && e.payload==NULL)
		//	return true;
		//if(*(this->payload)==*(e.payload))
		//	return true;
		return true;
	}
	return false;

}


Entry::~Entry(){
	if(this->word!=NULL){
		delete this->word;
		//entry->word=NULL;
	}
	if(this->payload!=NULL){
		delete this->payload;
		this->payload=NULL;
		//delete entry->payload;
		//entry->payload=NULL;
	}
}

Entry::Entry(){
//	payload=new Payload;
//	word=new Word;
	payload=NULL;
	word=NULL;
}

Entry::Entry(const Entry& e){

	word=new Word;
	*word=*e.word;
	payload=new Payload;
	*payload=*e.payload;
}

Entry& Entry::operator=(const Entry& e){
	if (this != &e) {
		if(this->word!=NULL){
			delete this->word;
//			this->word=new Word();
//			this->word=e.word;
		}
			this->word=new Word();
			this->word=e.word;
		if(this->payload!=NULL){
			delete this->payload;
//			this->payload=new Payload();
//			this->payload=e.payload;
		}
			this->payload=new Payload();
			this->payload=e.payload;
	}
	return *this;
}

Payload& Payload::operator=(const Payload& p){
	if (this != &p) {
		if(this->queries!=NULL)
			delete this->queries;
		this->queries=new LinkedList<pInfo>;
		if(p.queries!=NULL){
			int lnum=p.queries->count;
			Node<pInfo>* nd;
			nd=p.queries->headPtr;
			for(int i=0;i<lnum;i++){
				this->queries->addNode(*nd->data);
				nd=nd->next;
			}
		}
	}
	return *this;
}


void Payload::addToPl(QueryID& id,int pos,int dist){

	pInfo inf;
	inf.id=id;
	inf.pos=pos;
	inf.dist=dist;
	if(queries==NULL)
		queries=new LinkedList<pInfo>;
	queries->addNode(inf);
	return;
}

void Payload::printPayloads(){

}


stringstream& operator<<(stringstream& out,const Entry& e){
	if(e.word!=NULL)
		out << (e.word->word);
	return out;
}


enum ErrorCode CreateEntry(Word* word, Entry* entry) {
	if(word==NULL)
		return EC_FAIL;
	if(entry->word!=NULL){
		DestroyEntry(entry);
	}
//	if(entry->word==NULL)
	entry->word=new Word;//(*word);
	CreateWord(word->word,entry->word);
	entry->payload=new Payload();
	return EC_SUCCESS;
}

enum ErrorCode DestroyEntry(Entry *entry) {

	if(entry!=NULL){
		if(entry->word!=NULL){
			delete entry->word;
			entry->word=NULL;
		}
		if(entry->payload!=NULL){
			delete entry->payload;
			entry->payload=NULL;
			//delete entry->payload;
			//entry->payload=NULL;
		}
	}
	return EC_SUCCESS;
}

enum ErrorCode AddEntry(EntryList* entryList, const Entry* entry) {


//	Entry* newE=new Entry();
//	newE->word=new Word();
//	newE->payload=new Payload();
//	*newE->word=*entry->word;
//	*newE->payload=*entry->payload;
//	entryList->list->addNode(*newE);

	entryList->list->addNode(*entry);


	if(entryList->current==NULL)
		entryList->current=entryList->list->headPtr;
	return EC_SUCCESS;
}



enum ErrorCode CreateEntryList(EntryList* entryList) {

	entryList->list=new LinkedList<Entry>;
	entryList->current=entryList->list->headPtr;
	return EC_SUCCESS;
}

Entry* GetFirst(const EntryList* entryList) {
	if(entryList==NULL)
		return NULL;
	Node<Entry>* nd;
	nd=entryList->list->headPtr;
	if(nd!=NULL)
		return nd->data;
	return NULL;
}

Entry* GetNext(EntryList* entryList) {
	if(entryList->current==NULL)
		return NULL;
	if(entryList->current->next == NULL){
		entryList->current=entryList->list->headPtr;
		return entryList->current->data;
	}
	entryList->current=entryList->current->next;
	return entryList->current->data;
}

enum ErrorCode DestroyEntryList(EntryList* entryList) {
	if(entryList==NULL)
		return EC_FAIL;
	else if(entryList->list!=NULL)
		delete entryList->list;
	return EC_SUCCESS;
}

unsigned int GetNumberOfEntries(const EntryList* entryList) {
	return entryList->list->getNoOfNodes();
}

Entry* listSelect(const EntryList* entries,int num){

	Node<Entry>* it=entries->list->headPtr;
	if(entries->list->getNoOfNodes() < num)
		return NULL;
	while(num>1){
		it=it->next;
		num--;
	}
	return it->data;
}

bool exists(Entry** array,int size, Entry* check){

	for(int i=0;i<size;i++)
		if(array[i]!=NULL)
			if(*array[i]==*check)
				return true;
	return false;
}

void quick(int array[], int start, int end){
    if(start < end){
        int l=start+1, r=end, p = array[start];
        while(l<r){
            if(array[l] <= p)
                l++;
            else if(array[r] >= p)
                r--;
            else
                swap(array[l],array[r]);
        }
        if(array[l] < p){
            swap(array[l],array[start]);
            l--;
        }
        else{
            l--;
            swap(array[l],array[start]);
        }
        quick(array, start, l);
        quick(array, r, end);
    }
}

int medOfMed(int* dists,int size){

	int partSize=10;
	int* array=(int*) malloc(size*sizeof(int));
	for(int i=0;i<size;i++){
		array[i]=dists[i];
	}
	int partNum=0;
	if(size < partSize){
		partNum=size;
		partSize=1;
	}
	else
		partNum=size/partSize;
	int ** splitArray;
	splitArray= (int**) malloc(partNum*sizeof(int*));
	for(int i=0;i<partNum;i++)
		splitArray[i]=(int*) malloc(partSize*sizeof(int));
	int j=0;
	for(int i=0;i<size;i++){
		if(i%(partSize) == 0 && i!=0)
			j++;
		if(j == partNum)
			break;
		splitArray[j][i%partSize]=array[i];

	}
	for(int i=0;i<partNum;i++){
		quick(splitArray[i],0,partSize-1);
	}
	int* medians=(int*) malloc(partNum*sizeof(int));
	for(int i=0;i<partNum;i++){
		medians[i]=splitArray[i][partSize/2];
	}
	int medOfMed;
	medOfMed=medians[partNum/2];
	free(array);
	for(int i=0;i<partNum;i++)
		free(splitArray[i]);
	free(splitArray);
	free(medians);
	return medOfMed;
}


Entry* getVp(const EntryList* entries,int (*cmpF)(const Word*,const Word*),int ** dist1,int *dSize){

	int entriesN=entries->list->getNoOfNodes();
	if(entriesN <= 2)
		return NULL;
	srand ((unsigned)time(NULL));
	int vPn=5;
	if(entriesN < 10)
		vPn=2;
	Entry** vPs=NULL;
	vPs=(Entry**)malloc(vPn*sizeof(Entry*));
	for(int i=0;i<vPn;i++)
		vPs[i]=NULL;
	int vPsfound=0;
	while(vPsfound < vPn){
		Entry* vPcheck;
		int randvP=rand() % (entriesN+1);
		vPcheck=listSelect(entries,randvP);
		if(exists(vPs,vPn,vPcheck))
			continue;
		vPs[vPsfound]=vPcheck;
		vPsfound++;
	}
	int sampleN=(entriesN * 0.1);
	if(entriesN < 10){
		sampleN=entriesN-vPn;
		if(sampleN > 7)
			sampleN=4;
	}
	Entry** samples=NULL;
	samples=(Entry**)malloc(sampleN*sizeof(Entry*));
	for(int i=0;i<sampleN;i++)
		samples[i]=NULL;
	int sfound=0;
	while(sfound < sampleN){
		Entry* vPcheck;
		int randSmpl=rand() % (entriesN+1);
		vPcheck=listSelect(entries,randSmpl);
		samples[sfound]=vPcheck;
		sfound++;
	}
	double *devAr=(double*)malloc(vPn*(sizeof(double)));
	int *dist=(int*)malloc(sampleN*sizeof(int));
	*dSize=sampleN;
	*dist1=dist;
	for(int i=0;i<vPn;i++){
		int average=0;
 		for(int j=0;j<sampleN;j++){
			dist[j]=(*cmpF)(vPs[i]->word,samples[j]->word);
			average+=dist[j];
		}
		average/=sampleN;
		double dev=0;
		for(int j=0;j<sampleN;j++){
			dev+=pow(dist[j]-average,2);
		}
		dev/=sampleN;
		dev=sqrt(dev);
		devAr[i]=dev;
	}
	int sel=0;
	for(int i=0;i<vPn;i++){
		if(devAr[i]>=devAr[sel])
			sel=i;
	}
	for(int j=0;j<sampleN;j++){
		dist[j]=(*cmpF)(vPs[sel]->word,samples[j]->word);
	}
	free(samples);
	free(devAr);
	free(dist);
	Entry* vP=new Entry;
	CreateEntry(vPs[sel]->word,vP);
	free(vPs);
	return vP;

}

void createNode(const EntryList* entries,int (*cmpF)(const Word*,const Word*),indexNode** node, int depth){

	if(entries==NULL)
		return;
	int * dist=NULL;
	int dSize;
	Entry* vantP=NULL;
	if(entries->list->getNoOfNodes()==1){
		*node=new indexNode();
		(*node)->vP=new Entry();
		CreateEntry(entries->list->headPtr->data->word,(*node)->vP);
		*(*node)->vP->payload=*(entries->list->headPtr->data->payload);
		return;
	}
	if(entries->list->getNoOfNodes()==2){
		*node=new indexNode;
		(*node)->vP=new Entry();
		CreateEntry(entries->list->headPtr->data->word,(*node)->vP);
		*(*node)->vP->payload=*(entries->list->headPtr->data->payload);

		(*node)->rad=50;
		(*node)->inRad=new indexNode();
		(*node)->inRad->vP=new Entry();
		CreateEntry(entries->list->headPtr->next->data->word,(*node)->inRad->vP);
		*(*node)->inRad->vP->payload=*(entries->list->headPtr->next->data->payload);
		return;
	}
	if(entries->list->getNoOfNodes() > 2){
		int size=entries->list->getNoOfNodes();
		vantP=getVp(entries,cmpF,&dist,&dSize);
		Node<Entry>* it=entries->list->headPtr;
		int entriesS=entries->list->getNoOfNodes();

		for(int i=0;i<entriesS;i++){
			if(Equal(vantP->word,it->data->word)==0)
				*(vantP->payload)=*it->data->payload;
			it=it->next;
		}
		if(vantP==NULL){
		}
		int* dists=(int*)malloc(size*sizeof(int));
		Node<Entry>* it1;
		it1=entries->list->headPtr;
		for(int i=0;i<size;i++){

			dists[i]=cmpF(vantP->word,it1->data->word);
			if(dists[i]==0){
				i--;
			}

			it1=it1->next;
			if(it1==NULL)
				break;
		}
		(*node)=new indexNode();
		(*node)->rad=medOfMed(dists,size-1);
		EntryList* inRad=new EntryList;
		EntryList* offRad=new EntryList;
		CreateEntryList(inRad);
		CreateEntryList(offRad);
		it=entries->list->headPtr;
		int edD=0;
		for(int i=0;i<entriesS;i++){
			Entry entry;
			CreateEntry(it->data->word,&entry);
			*entry.payload=*it->data->payload;
			if((edD=(*cmpF)(it->data->word,vantP->word)) <= (*node)->rad && edD>0)
				AddEntry(inRad,&entry);
			else if(edD>0)
				AddEntry(offRad,&entry);
			it=it->next;
		}
		(*node)->vP=new Entry;
		CreateEntry(vantP->word,(*node)->vP);
		*(*node)->vP->payload=*(vantP->payload);

		if(inRad->list->getNoOfNodes()>0) {
			createNode(inRad,cmpF,&(*node)->inRad,depth+1);
		}
		if(offRad->list->getNoOfNodes()>0){
			createNode(offRad,cmpF,&(*node)->offRad,depth+1);
		}
		DestroyEntryList(inRad);
		delete inRad;
		DestroyEntryList(offRad);
		delete offRad;
		DestroyEntry(vantP);
		delete vantP;
		free(dists);
	}
	return;
}

enum ErrorCode BuildEntryIndex(const EntryList* entries, enum MatchType type,
		Index* index) {
	int (*cmpF)(const Word*,const Word*);
	if(type==MT_EXACT_MATCH)
		cmpF=&Equal;
	if(type==MT_HAMMING_DIST){
//		cout << "Hamming!" << endl;
		cmpF=&HammingDistance;
	}
	if(type==MT_EDIT_DIST)
		cmpF=&EditDistance;
	index->type=type;
	createNode(entries,cmpF,&index->headNode,0);
	return EC_SUCCESS;
}

void destroyIndex(indexNode* ind){
	if (ind==NULL)
		return;
	if(ind->inRad!=NULL){
		destroyIndex(ind->inRad);
		ind->inRad=NULL;
	}
	if(ind->offRad!=NULL){
		destroyIndex(ind->offRad);
		ind->offRad=NULL;
	}
	if(ind->vP!=NULL){
		DestroyEntry(ind->vP);
		delete ind->vP;
		ind->vP=NULL;
		//delete ind;
	}
	delete ind;
}



void printIndex(indexNode* ind){
	if(ind==NULL)
		return;
	printIndex(ind->inRad);
	printIndex(ind->offRad);
	if(ind->vP!=NULL){
		cout << "ind->vp: " << *ind->vP->word << endl;
		if(ind->vP->payload!=NULL){
			cout << "Exei kai payload.." << endl;
			ind->vP->payload->printPayloads();
		}
	}
}

enum ErrorCode DestroyEntryIndex(Index* index) {
	destroyIndex(index->headNode);
	//delete index;
	return EC_SUCCESS;
}

void searchNode(const Word* word, int threshold,
		const indexNode* node, EntryList* result,enum MatchType type){
	int dist;
	if(type==MT_HAMMING_DIST)
		dist=HammingDistance(word,node->vP->word);
	else
		dist=EditDistance(word,node->vP->word);
	if(dist <= threshold){
		Entry newE;
		CreateEntry(node->vP->word,&newE);
		///*newE.word=*(node->vP->word);
		//newE.payload=new Payload;
//		cout << "I match " << word->word << " with " << node->vP->word->word
//				<<" me threshold " << threshold << endl;
		Node<pInfo>* itP=node->vP->payload->queries->headPtr;
		int pNum=node->vP->payload->queries->count;
		for(int i=0;i<pNum;i++){
			//if(itP->data->dist >= threshold){
				pInfo p=*itP->data;
				if(dist <= p.dist)
					newE.payload->queries->addNode(p);
			//}
			itP=itP->next;
		}
		//if(itP->data->dist >= threshold)
		AddEntry(result,&newE);//node->vP);
	}
	if( (node->inRad!=NULL) && dist <= node->rad+threshold){
		searchNode(word,threshold,node->inRad,result,type);
	}
	if( (node->offRad!=NULL) && dist > node->rad - threshold ){
		searchNode(word,threshold,node->offRad,result,type);
	}
}

enum ErrorCode LookupEntryIndex(const Word* word, int threshold,
		const Index* index, EntryList* result) {

	if(word==NULL)
		return EC_FAIL;
	searchNode(word,threshold,index->headNode,result,index->type);
	if(result->list->getNoOfNodes()>0)
		return EC_SUCCESS;
	return EC_FAIL;
}

