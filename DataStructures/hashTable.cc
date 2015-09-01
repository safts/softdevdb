
#include <iostream>
#include <ostream>
#include <sstream>
#include <cstdlib>

using namespace std;
template <class L>
hashTable<L>::hashTable(int bucketNum){

	this->bucketNum=bucketNum;
	buckets = new LinkedList<L>*[this->bucketNum];
	for(int i=0;i<this->bucketNum;i++)
		buckets[i]=NULL;
	recNum=0;
	collisions=0;

}
template <class L>
void hashTable<L>::insert(const L& key){

	unsigned long int hashRes=0;
	//string str=key.getStr();
	//cout << str << endl;
	string res;
	stringstream str;
	str << key;
	//cout << key << "blab" << key;
	res=str.str();
	//cout << "HAHAHA" << endl;
	hashRes=hash((unsigned char *)res.c_str());
	hashRes=hashRes % bucketNum;
	if(buckets[hashRes]==NULL)
		buckets[hashRes]=new LinkedList<L>();
	else
		collisions++;
	buckets[hashRes]->addNode(key);
	recNum++;
}

template <class L>
bool hashTable<L>::remove(const L& key){

	unsigned long int hashRes=0;
	string res;
	stringstream str;
	str << key;
	res=str.str();
	hashRes=hash((unsigned char *)res.c_str());
	hashRes=hashRes % bucketNum;
	L* data=find(key);

	if(data==NULL)
		return false;
	else{

		bool test = buckets[hashRes]->deleteNode(*data);
		if(!test)
			return false;
		else{
			recNum--;
			if(buckets[hashRes]->isEmpty()){
				delete buckets[hashRes];
				buckets[hashRes]=NULL;
			}
			return true;
		}
	}

}

template <class L>
void hashTable<L>::report(){

	cout << "-------------------AfmHash Report-------------------" << endl;
	cout << "Total Records:  " << recNum << endl;
	cout << "Buckets:        " << bucketNum << endl;
	cout << "Collisions:     " << collisions << endl;
	cout << "-------------------AfmHash Report-------------------" << endl;

}
template <class L>
void hashTable<L>::print(){

	cout <<"The hash table contains: " << endl;
	for(int i=0;i<this->bucketNum;i++){
		if(buckets[i]!=NULL){
			cout << "Bucket #" <<i << endl;
			buckets[i]->displayAllNodes();
		}
	}
}
template <class L>
hashTable<L>::~hashTable(){

	for(int i=0;i<bucketNum;i++)
		if(buckets[i]!=NULL)
			delete buckets[i];
	delete [] buckets;
}

template <class L>
L* hashTable<L>::find(const L& key){

	unsigned long int hashRes=0;
	string res;
	stringstream str;
	str << key;
	res=str.str();
	hashRes=hash((unsigned char *)res.c_str());
	hashRes=hashRes % bucketNum;
	if(buckets[hashRes]==NULL)
		return NULL;
	else{
		Node<L>* data;
		//L* data;
		data=buckets[hashRes]->findNode(key);
		if(data==NULL)
			return NULL;
		//n->insert(*data);
		return data->data;
	}
}

template <class L>
int hashTable<L>::modify(const L& key, const L& val){

	L* fd=find(key);
	if(fd==NULL)
		return -1;
	else{
		//L key1(key);
		remove(key);
		//key1=val;
		insert(val);
		return 0;
	}
}

template <class L>
unsigned long hashTable<L>::hash(unsigned char *str){

	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
