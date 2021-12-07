#pragma once
#include "PriorityQueue.h"
#include "Sequance.h"
#include "Dictionary.h"

#include <iostream>

template <class K,class T>
PQueue<K,T>::PQueue()
{
	pair = new ArrayVector<PairQ<K,T>*>();
}

template <class K,class T>
bool cmp(PairQ<K,T>* el1, PairQ<K,T>* el2)
{
	return el1->priority < el2->priority;
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
	pair->Prepend(node);
	//pair->FastSort(false, 0, pair->GetLength() - 1, &cmp);
	
}

template <class K,class T>
void PQueue<K,T>::PopBack()
{
	pair->PopBack();
}


template <class K,class T>
K PQueue<K,T>::GetMinValue()
{
	return pair->GetLast()->elem;
}

template <class K, class T>
int PQueue<K, T>::BinSearch(K elem_, T priority_, int l, int r)
{
	if (r == -11) r = pair->GetLength() - 1;
	int mid = (r + l) / 2;
	if (priority_ > pair->Get(mid)->priority)
	{
		if (l <= mid - 1) BinSearch(elem_, priority_, l, mid - 1);
	}
	else if (priority_ < pair->Get(mid)->priority)
	{
		if (mid + 1 <= r) BinSearch(elem_, priority_, mid + 1, r);
	}
	else
	 {
		 if (elem_ == pair->Get(mid)->elem)
		 {
			 return mid;
		 }
		 
		 else
		 {
			 int ll = mid;
			 int rr = mid;
			 while ((pair->Get(ll)->elem != elem_) && (pair->Get(rr)->elem != elem_))
			 {
				 if (ll > 0)ll--;
				 if (rr < pair->GetLength() - 1) rr++;
				 if (pair->Get(ll)->elem == elem_) return ll;
				 if (pair->Get(rr)->elem == elem_) return rr;
			 }
		 }
		 
	 }
}

template <class K, class T>
void PQueue<K, T>::Set(K elem,T ltime,T priority)
{
	int i = 0;
	i = BinSearch(elem, ltime);
	if (i < 0) return;
	PairQ<K, T>* node = new PairQ<K, T>(elem, priority);//
	delete pair->Get(i);
	pair->Set(node, i);
	//pair->FastSort(true, 0, pair->GetLength() - 1, &cmp);
}

template <class K, class T>
void PQueue<K, T>::Clear()
{
	for (int i = 0; i < pair->GetLength(); i++)
	{
		delete pair->Get(i);
	}
	delete[] pair;
}

template <class K, class T>
PQueue<K, T>::~PQueue()
{
	Clear();
}

//////////////////////////////Bin////////////////////////////////

template <class K, class T>
PQueueBin<K, T>::PQueueBin()
{
	pair = new DictionaryBin<K, T>();
}


template <class K, class T>
void PQueueBin<K, T>::Add(T elem, K priority)
{
	pair->Add(elem, priority);
}


template <class K, class T>
void PQueueBin<K, T>::PopBack()
{
	K key = pair->GetKeyMin();
	pair->Remove(key);
}


template <class K, class T>
K PQueueBin<K, T>::GetMinValue()
{
	K key = pair->GetKeyMin();
	return pair->Get(key);
}


template <class K, class T>
void PQueueBin<K, T>::Set(T elem, K time, K priority)
{
	pair->Remove(time);
	pair->Add(elem, priority);
}


template <class K, class T>
void PQueueBin<K, T>::Clear()
{
	pair->Clear();
}


template <class K, class T>
PQueueBin<K, T>::~PQueueBin()
{
	Clear();
}