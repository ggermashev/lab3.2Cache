#include "PriorityQueue.h"
#include "Sequance.h"

template <class K,class T>
PQueue<K,T>::PQueue()
{
	pair = new ArrayVector<PairQ<K,T>*>();
}

template <class K,class T>
bool cmp(PairQ<K,T>* el1, PairQ<K,T>* el2)
{
	return el1->priority > el2->priority;
}

template <class K,class T>
void PQueue<K,T>::swap(PairQ<K,T>* el1, PairQ<K,T>* el2)
{
	PairQ<K,T>* save = el1;
	el1 = el2;
	el2 = save;
}

template <class K,class T>
void PQueue<K,T>::Add(K elem, T priority)
{
	PairQ<K, T>* node = new PairQ<K, T>(elem, priority);
	pair->Append(node);
	pair->HeapSort(false, 0, pair->GetLength() - 1, &cmp);
}

template <class K,class T>
void PQueue<K,T>::PopBack(T value)
{
	/*
	int i;
	for (i = 0; i < pair->GetLength(); i++)
	{
		if (value == pair->Get(i)->elem)
			break;
	}
	swap((pair->Get(i)), pair->Get(pair->GetLength() - 1));
	*/
	pair->PopBack();

	//pair->HeapSort(false, 0, pair->GetLength() - 1, &cmp);
}

template <class K,class T>
T PQueue<K,T>::GetMinValue()
{
	return pair->GetLast()->elem;
}

template <class K, class T>
void PQueue<K, T>::Set(K elem,T priority)
{
	int i;
	for (i = 0; i < pair->GetLength(); i++)
	{
		if (pair->Get(i)->elem == elem)
			break;
	}
	PairQ<K, T>* node = new PairQ<K, T>(pair->Get(i)->elem, priority);
	pair->Set(node, i);
}