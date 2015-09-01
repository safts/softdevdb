#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <iostream>
#include <cstdlib>

#include "Node.h"

using namespace std;


template <class T>
class LinkedList {

	public:


		Node<T>* headPtr;
		int count;
	
		//int dataCmp(T& , T& );
		//void displayNode(Node<T>*);

		LinkedList();

		void addNode(const T& );
		bool deleteNode(const T &);
		void deleteAllNodes();

		Node<T>* findNode(const T& );

		bool isEmpty();
		int getNoOfNodes() const;

		void displayAllNodes();
		void report();

		~LinkedList();

};

#include "linkedList.cc"


//template <class T>
//int LinkedList<T>::dataCmp(T& value0, T& value1) {
//
//	return value0.compare(value0,value1);
//}

//template <class T>
//void LinkedList<T>::displayNode(Node<T> *node) {
//
//	node->data->print();
//	return;
//}

#endif
