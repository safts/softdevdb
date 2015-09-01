

template <class T>
LinkedList<T>::LinkedList() {

	count = 0;
	headPtr = NULL;
	return;
}

template <class T>
LinkedList<T>::~LinkedList() {

	deleteAllNodes();
	return;
}


template <class T>
void LinkedList<T>::report(){

	cout << "-------------------List Report-------------------" << endl;
	cout << "Total Records:  " << getNoOfNodes() << endl;
	cout << "-------------------List Report-------------------" << endl;
}

template <class T>
void LinkedList<T>::addNode(const T& newData) {

	Node<T> *pNew = new Node<T>;
	Node<T> *pPre = NULL;
	Node<T> *pCur = headPtr;
	T* nD=new T(newData);
	pNew->data = nD;
//	while (pCur && *pNew < *pCur) {
//		pPre = pCur;
//		pCur = pCur->next;
//	}
//	if (pPre) {
//		pNew->next = pPre->next;
//		pPre->next = pNew;
//		count++;
//	} else {
	//if(!pCur){
		headPtr=pNew;
		pNew->next=pCur;//NULL;
		count++;
		return;
	//}
		
//	pCur=headPtr;
	while(pCur){
		pPre=pCur;
		pCur=pCur->next;
	}
	//	pNew->next = headPtr;
	//	headPtr = pNew;
	pPre->next=pNew;
	pNew->next=NULL;
	count++;
//	}

}

template <class T>
bool LinkedList<T>::deleteNode(const T& data) {

	bool exit;
	Node<T> *pPre = NULL, *pCur = headPtr;
	while (pCur && *(pCur->data) != data ) {
		pPre = pCur;
		pCur = pCur->next;
	}
	if (pCur && *(pCur->data) == data) {
		if (pPre) {
			pPre->next = pCur->next;
			delete pCur;
			count--;
			exit = true; // return true if successful
		} else {
			headPtr = pCur->next;
			delete pCur;
			count--;
			exit = true; // return true if successful
		}
	} else
		exit = false; // return false if unsuccessful
	return exit;
}

template <class T>
void LinkedList<T>::deleteAllNodes() {
	Node<T> *temp;
	while (headPtr) {
		temp = headPtr;
		headPtr = headPtr->next;
		delete temp;
		count--;
	}
	return;
}

template <class T>
bool LinkedList<T>::isEmpty() {
	return count == 0;
}

template <class T>
int LinkedList<T>::getNoOfNodes() const{
	return count;
}

template <class T>
void LinkedList<T>::displayAllNodes() {

	Node<T> *pCur = headPtr;
	int nodeCount = 1;
	while (pCur) {
		cout << "Node " << nodeCount << ": ";
		cout << *pCur;
		cout << endl;
		nodeCount++;
		pCur = pCur->next;
	}
	return;
}

template <class T>
Node<T>* LinkedList<T>::findNode(const T& key1){

	Node<T> *pCur = headPtr;
//	while ( pCur && (*(pCur->data) > key1 )){
//		pCur=pCur->next;
//	}
//	if( !pCur || (*(pCur->data) < key1 ))
//		return NULL;
//	else
//		return pCur;
	while( pCur ){
		if (*(pCur->data) == key1)
			return pCur;
		else
			pCur=pCur->next;
	}
	return NULL;
	
}

