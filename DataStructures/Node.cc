#include <cstdlib>
#include "Node.h"
#include <sstream>
#include <ostream>
#include <iostream>

using namespace std;

template <class T>
Node<T>::Node(){

	data=NULL;
	next=NULL;
};


template <class T>
bool Node<T>::operator==(const Node<T>& rhs) const{ 

	if(*(this->data) == *(rhs.data))
		return true;
	else
		return false;
};

template <class T>
bool Node<T>::operator< (const Node<T>& rhs) const{

	if( *(this->data) < *(rhs.data))
		return true;
	else
		return false;
};

template <class T>
Node<T>& Node<T>::operator= (const Node<T>& rhs){

	if(*this != rhs){
		delete this->data;
		this->data=new T;
		*(this->data)=*(rhs.data);
		*(this->next)=*(rhs.next);
	}
	return *this;
};

template <class T>
Node<T>::~Node(){

	if(data!=NULL)
		delete data;
};

template <class T>
void Node<T>::insert(const T& dat){

	if(data==NULL)	
		data = new T;
	*(this->data)=dat;
};

template <class T>
bool Node<T>::operator!=(const Node<T>& rhs){

	return !this->operator==(rhs);
};

template <class T>
bool Node<T>::operator> (const Node<T>& lhs){

	return  lhs.operator< (*this);
};

template <class T>
bool Node<T>::operator<=(const Node<T>& rhs){

	return !this->operator> (rhs);
};

template <class T>
string Node<T>::getStr(){
	
	if(data==NULL)
		return NULL;
	else{
		string res;
		stringstream str;
		str << *data;
		res=str.str();
		cout << res << endl;
		return res;
	}
	return NULL;
}

template <class T>
ostream& operator<< (ostream& out, Node<T>& node){

	if(node.data!=NULL)
		out << *(node.data);
	return out;
}

template <class T>
bool Node<T>::operator>=(const Node<T>& rhs){

	return !this->operator<(rhs);
};
