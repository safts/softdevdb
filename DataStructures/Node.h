#ifndef NODE_H_
#define NODE_H_

#include <cstdlib>
#include <iostream>



using namespace std;

template <class T>
class Node {
public:

	T* data;
	Node<T>* next;

	Node();

	bool operator==(const Node<T>& rhs) const;

	bool operator< (const Node<T>& rhs) const;

	Node<T>& operator= (const Node<T>& rhs);

	~Node();
	
	void insert(const T& dat);

	std::string getStr();

	bool operator!=(const Node<T>& rhs);
	
	bool operator> (const Node<T>& lhs);

	bool operator<=(const Node<T>& rhs);
	
	bool operator>=(const Node<T>& rhs);
	
//	ostream& operator<< (ostream& , Node<T>&);
	
//	friend ostream& operator<< (ostream& out, Node<T>& node){
//
//		if(node.data!=NULL)
//			out << *(node.data);
//		return out;
//	}
};
template <class T>
ostream& operator<< (ostream& , Node<T>&);

#include "Node.cc"
//template <class T>
//ostream& Node<T>::operator<< (ostream& out, Node& node){
//	
//	if(node.data!=NULL)
//		out << *(node.data) ;
//};	


#endif
