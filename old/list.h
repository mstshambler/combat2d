#ifndef _list_h
#define _list_h

template <class T> 
class ListElement {
protected:
	T *data;
	ListElement *next, *prev;

public:
	ListElement() {
		data = NULL;
		next = NULL;
		prev = NULL;
	}

	ListElement(const T *d) {
		SetRef(d);
		next = NULL;
		prev = NULL;
	}

	ListElement(const T *d, const ListElement *n, const ListElement *p) {
		SetRef(d);
		SetNext(n);
		SetPrev(p);
	}

	~ListElement() {
	}

	void SetRef(const T *d) {
		data = (T *)d;
	}

	void SetNext(const ListElement *n) {
		next = (ListElement *)n;
	}
	
	void SetPrev(const ListElement *p) {
		prev = (ListElement *)p;
	}

	T *GetRef() const {
		return data;
	}
	
	ListElement *GetNext() const {
		return next;
	}
	
	ListElement *GetPrev() const {
		return prev;
	}
};

template <class T> class ListReader;

template <class T> 
class List {
protected:
	ListElement<T> *firstElement, *lastElement;
	int count;
	int modified; // 0 - no changes, 1 - added element, 2 - removed element, 3 - cleared
	ListElement<ListReader<T>> *firstReader, *lastReader, *curReader;
	ListReader<T> *curReaderRef;

	void ClearList(const int &mode) {
		while(firstElement) {
			lastElement = firstElement->GetNext();
			if (mode)
				delete firstElement->GetRef();
			delete firstElement;
			firstElement = lastElement;
		}
		firstElement = lastElement = NULL;
		count = 0;
		modified = 3;

		curReader = firstReader;
		while(curReader) {
			curReaderRef = curReader->GetRef();
			curReaderRef->SetCurrentElement(NULL);
			curReaderRef->SetNextElement(NULL);
			curReaderRef->SetPrevElement(NULL);
			curReader = curReader->GetNext();
		}
	}

public:
	List() {
		firstElement = lastElement = NULL; 
		firstReader = lastReader = NULL;
		count = 0;
		modified = 0;
	}

	~List() {
		Clear();		
	}

	ListElement<T> *GetFirstElement() const {
		return firstElement;
	}

	ListElement<T> *GetLastElement() const {
		return lastElement;
	}

	void Clear() {
		ClearList(0);
	}

	void ClearAndDelete() {
		ClearList(1);
	}

	void AddElement(const T *element) {
		ListElement<T> *newElement = new ListElement<T>();

		newElement->SetRef(element);
		newElement->SetNext(NULL);
		if (firstElement == NULL) {
			newElement->SetPrev(NULL);
			firstElement = newElement;
			lastElement = newElement;
		} else {
			lastElement->SetNext(newElement);
			newElement->SetPrev(lastElement);

			curReader = firstReader;
			while(curReader) {
				curReaderRef = curReader->GetRef();
				if (curReaderRef->GetCurrentElement() == lastElement)
					curReaderRef->SetNextElement(lastElement->GetNext());
				curReader = curReader->GetNext();
			}

			lastElement = newElement;
		}
		count++;
		if (modified == 0)
			modified = 1;
	}

	void RemoveElement(const T *element) {
		ListElement<T> *findElement, *prevElement;

		findElement = firstElement;
		prevElement = NULL;
		while(findElement && findElement->GetRef() != element) {
			prevElement = findElement;
			findElement = findElement->GetNext();
		}
		if (findElement) {
			if (findElement == firstElement) {
				firstElement = firstElement->GetNext();
				if (firstElement)
					firstElement->SetPrev(NULL);
				else
					lastElement = NULL;
			}
			if (findElement == lastElement) {
				lastElement = lastElement->GetPrev();
				if (lastElement)
					lastElement->SetNext(NULL);
			}
			if (prevElement) {
				prevElement->SetNext(findElement->GetNext());
				if (findElement->GetNext())
					findElement->GetNext()->SetPrev(prevElement);
			}

			curReader = firstReader;
			while(curReader) {
				curReaderRef = curReader->GetRef();
				if (curReaderRef->GetCurrentElement() == findElement) {
					curReaderRef->SetNextElement(findElement->GetNext());
					curReaderRef->SetPrevElement(findElement->GetPrev());
				}
				if (curReaderRef->GetNextElement() == findElement)
					curReaderRef->SetNextElement(findElement->GetNext());
				if (curReaderRef->GetPrevElement() == findElement)
					curReaderRef->SetPrevElement(findElement->GetPrev());
				curReader = curReader->GetNext();
			}

			delete findElement;
			count--;
			if (modified < 2)
				modified = 2;
		}
	}

	int GetCount() const {
		return count;
	}

	int GetModified() const {
		return modified;
	}
	
	void SetModified(const int &i) {
		modified = i;
	}

	void AttachReader(const ListReader<T> *r) {
		ListElement<ListReader<T>> *newReader = new ListElement<ListReader<T>>();

		newReader->SetRef(r);
		newReader->SetNext(NULL);
		if (firstReader == NULL) {
			newReader->SetPrev(NULL);
			firstReader = newReader;
			lastReader = newReader;
		} else {
			lastReader->SetNext(newReader);
			newReader->SetPrev(lastReader);

			lastReader = newReader;
		}
	}

	void DetachReader(const ListReader<T> *r) {
		ListElement<T> *findReader, *prevReader;

		findReader = firstReader;
		prevReader = NULL;
		while(findReader && findReader->GetRef() != r) {
			prevReader = findReader;
			findReader = findReader->GetNext();
		}
		if (findReader) {
			if (findReader == firstReader) {
				firstReader = firstReader->GetNext();
				if (firstReader)
					firstReader->SetPrev(NULL);
				else
					lastReader = NULL;
			}
			if (findReader == lastReader) {
				lastReader = lastReader->GetPrev();
				if (lastReader)
					lastReader->SetNext(NULL);
			}
			if (prevReader) {
				prevReader->SetNext(findReader->GetNext());
				if (findReader->GetNext())
					findReader->GetNext()->SetPrev(prevReader);
			}

			delete findReader;
		}
	}
};

template <class T>
class ListReader {
protected:
	ListElement<T> *curElement;
	ListElement<T> *nextElement;
	ListElement<T> *prevElement;
	List<T> *baseList;

	void UpdateElements() {
		if (curElement) {
			nextElement = curElement->GetNext();
			prevElement = curElement->GetPrev();
		} else {
			nextElement = NULL;
			prevElement = NULL;
		}
	}

public:
	ListReader(List<T> *l) {
		ListReader();
		Attach(l);
	}

	ListReader() {
		curElement = NULL;
		baseList = NULL;
	}

	~ListReader() {
	}

	void Attach(List<T> *l) {
		baseList = l;
		baseList->AttachReader(this);
		curElement = l->GetFirstElement();
		if (curElement)
			nextElement = curElement->GetNext();
		prevElement = NULL;
	}

	void Detach() {
		if (baseList)
			baseList->DetachReader(this);
		curElement = NULL;
		UpdateElements();
	}

	List<T> *GetList() const {
		return baseList;
	}

	void SetCurrentElement(const ListElement<T> *elem) {
		curElement = (ListElement<T> *)elem;
	}

	ListElement<T> *GetCurrentElement() const {
		return curElement;
	}
	
	void SetNextElement(const ListElement<T> *elem) {
		nextElement = (ListElement<T> *)elem;
	}

	ListElement<T> *GetNextElement() const {
		return nextElement;
	}

	ListElement<T> *GoNextElement() {
		curElement = nextElement;
		UpdateElements();

		return curElement;
	}
	
	void SetPrevElement(const ListElement<T> *elem) {
		prevElement = (ListElement<T> *)elem;
	}

	ListElement<T> *GetPrevElement() const {
		return prevElement;
	}

	ListElement<T> *GoPrevElement() {
		curElement = prevElement;
		UpdateElements();

		return curElement;
	}

	T *GetCurrentRef() const {
		if (curElement)
			return curElement->GetRef();
		return NULL;
	}
	
	T *GoNextRef() {
		GoNextElement();

		return GetCurrentRef();
	}
	
	T *GoPrevRef() {
		GoPrevElement();

		return GetCurrentRef();
	}
	
	T *GoFirstRef() {
		if (baseList)
			curElement = baseList->GetFirstElement();
		UpdateElements();

		return GetCurrentRef();
	}
	
	T *GoLastRef() {
		if (baseList)
			curElement = baseList->GetLastElement();

		UpdateElements();
		return GetCurrentRef();
	}

	T *GoRef(int i) {
		int k = 0;

		GetFirstRef();
		while(k < i && GoNextRef()) k++;
		UpdateElements();

		return GetCurrentRef();
	}
};

#endif
