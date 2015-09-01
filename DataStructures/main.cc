#include <iostream>
#include "linkedList.h"
#include "hashTable.h"

using namespace std;



int main(void){

	hashTable<int> hash;
	for(int i=0;i<15;i++){
		hash.insert(3*i);
		cout << "Evala to " << 3*i << endl;
	}
	for(int i=0;i<15;i++){
		int* found=hash.find(3*i);
		if(!found)
			cout << "Den to vrhka.." << endl;
		else
			cout << "H find epistrefei " << *found << endl;
	}
	hash.report();
	hash.print();

	for(int i=0;i<15;i++){
		int* found=hash.find(3*i);
		if(!found)
			cout << "Den to vrhka.." << endl;
		else
			hash.modify(3*i,i);
	}
	hash.report();
	hash.print();
}
