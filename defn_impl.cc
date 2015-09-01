#include "defn.h"
#include "defn_impl.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))


using namespace std;

//////////////////struct Word implementations/////////////////

Word::Word() {

	this->length=0;
//	this->word=NULL;
}

Word::Word(const Word& w) {
//	if (w.word != NULL) {
//		this->word = (char*) malloc((w.length + 1) * sizeof(char));
		strcpy(this->word, w.word);
		this->length = w.length;
//	}
}

bool Word::operator==(const Word& w) const{
	if (this->length == w.length) {
		if (strcmp(this->word, w.word) == 0)
			return true;
	} else
		return false;
	return false;

}

bool Word::operator!=(const Word& w) {
	return !this->operator==(w);
}

Word& Word::operator=(const Word& w) {
	if (this != &w) {
//		free(this->word);
//		if (w.word != NULL) {
//			this->word = (char*) malloc((w.length + 1) * sizeof(char));
		strcpy(this->word, w.word);
		this->length = w.length;
//		}
	}
	return *this;
}



Word::~Word() {

//	if (this->word != NULL)
//		free(this->word);
}

ostream& operator<< (ostream& out, const Word& w){

	if(w.word!=NULL)
		out << w.word;
	return out;
}

//////////////////struct Word implementations/////////////////

//////////////////struct Query implementations/////////////////



Query::~Query() {

	for (int i = 0; i < wordN; i++)
		if (content[i] != NULL)
			delete (content[i]);
}

//////////////////struct Query implementations/////////////////

//////////////////struct Document implementations/////////////////

//Document::Document(){
//
//	wordsN=0;
//}

Document::~Document(){

	content.deleteAllNodes();
}

//////////////////struct Document implementations/////////////////

void CreateWord(const char* word_str, Word* word) {

	int l;
	if (word_str != NULL) {
		l = strlen(word_str);
		if (l > 0) {
//			word->word = (char*) malloc((l + 1) * sizeof(char));
			strcpy(word->word, word_str);
		}
	}
	word->length = l;
}

const char * getWordContent(Word word){
  return NULL;
}

void CreateQuery(QueryID query_id, const char* query_str,
		enum MatchType match_type, unsigned int match_dist, Query* query) {
	//query = new Query

	for (int i = 0; i < MAX_QUERY_WORDS; i++)
		query->content[i] = NULL;
	stringstream ss(query_str);
	string str;
	query->wordN = 0;
	while (ss >> str && query->wordN < MAX_QUERY_WORDS) {
		query->content[query->wordN]=new Word();
		CreateWord(str.c_str(),query->content[query->wordN]);
		query->wordN++;
	}
	query->type = match_type;
	query->id = query_id;
	query->matchDist = match_dist;
}

int GetNumQueryWords(const struct Query* query) {
	return query->wordN;
}

const struct Word* getQueryWord(unsigned int word, const Query* query) {

	if(query==NULL)
		return NULL;
	if(word < query->wordN)
		return query->content[word];
	return NULL;
}

void RemoveDuplicates(struct Document* doc) {

	if(doc==NULL)
		return;
	hashTable<Word> hash;
	Node<Word> * node=doc->content.headPtr;
	Node<Word> * previous;
	//to_del=&doc->content.headPtr;
	for(int i=0;i<doc->wordN;i++){

		//to_del=node;
		Word* found=hash.find(*(node->data));
		if(found==NULL)
			hash.insert(*(node->data));
		else{
			//doc->content.deleteNode(*(node->data));
			//Mporei na nai sto head? (OXI)
			previous->next=node->next;
			delete node;
			node=NULL;
			doc->content.count--;
			doc->wordN--;
			node=previous->next;
			continue;
		}
		previous=node;
		node=node->next;
		if(node==NULL)
			break;
	}
}


void CreateDocument(DocID doc_id, const char* doc_str, Document* document) {

	// Elegxos gia max doc size

	hashTable<Word> hash;
	document->id=doc_id;
	//for (int i = 0; i < MAX_QUERY_WORDS; i++)
	//	query->content[i] = NULL;
	stringstream ss(doc_str);
	string str;
	//char* str,*str2;
	//str=(char*)doc_str;
	//str2=strtok(str," ");
	document->wordN = 0;
	while (ss >> str ) {
		Word* w=new Word();

		CreateWord(str.c_str(),w);

		Word* found=hash.find(*w);
		if(found==NULL){
			hash.insert(*w);
			document->content.addNode(*w);
			document->wordN++;
		}
		delete w;
	}
}

int GetNumDocumentWords(const struct Document* doc) {

	return doc->content.getNoOfNodes();
}

const struct Word* getDocumentWord(unsigned int word, const Document* doc) {
	if(doc==NULL)
		return NULL;
	if(word > (unsigned) doc->wordN)
		return NULL;
	Node<Word> * node=doc->content.headPtr;
	for(int i=0;i<doc->wordN;i++){
		if((unsigned) i== word)
			return node->data;
		else
			node=node->next;
	}
	return NULL;
}

int Equal(const Word* w1, const Word* w2) {
	if((*w1)==(*w2))
		return 0;
	else
		return 1;
}

int HammingDistance(const Word* w1, const Word* w2) {
	int i,hamdis=0;
	if(w1->length!=w2->length)
		return -1;
	for(i=0;i<w1->length;i++){
		if(w1->word[i]!=w2->word[i])
			hamdis++;
	}

	return hamdis;
}


int EditDistance(const Word* w1, const Word* w2) {
	int i,j;
	int **dd;
	char q1[w1->length];
	char q2[w2->length];
	strcpy(q1,w1->word);
	strcpy(q2,w2->word);
	dd=(int**)malloc(((w1->length)+1)*sizeof(int *));
	for(i=0;i<(w1->length)+1;i++){
		dd[i]=(int*)malloc(((w2->length)+1)*sizeof(int));
	}
	dd[0][0]=0;
	for(i=1;i<=(w1->length);i++){
		dd[i][0]=i;
	}
	for(j=1;j<=(w2->length);j++){
		dd[0][j]=j;
	}
	for(i=1;i<=(w1->length);i++){
		for(j=1;j<=(w2->length);j++){
				dd[i][j]=MIN3(dd[i-1][j]+1,dd[i][j-1]+1,dd[i-1][j-1]+(q1[i-1]==q2[j-1] ? 0:1));
		}
	}
	int toret=dd[(w1->length)][(w2->length)];
	for(i=0;i<(w1->length)+1;i++)
		free(dd[i]);
	free(dd);
	return toret;
}

int Minimum(int a,int b,int c){
	int m;
	m=a;
	if(b<m){
		m=b;
	}
	if(c<m){
		m=c;
	}
	return m;
}
