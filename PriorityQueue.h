# pragma once

#include "Sequance.h"


template <class K, class T>
class PairQ
{
public:
	K elem;
	T priority;
	PairQ() { elem = 0; priority = 0; }
	PairQ(K elem_, T priority_) { elem = elem_, priority = priority_; }
};

template <class K,class T>
class PQueue
{
private:
	ArrayVector<PairQ<K,T>*>* pair;
	//bool cmp(PairQ<K,T> el1,PairQ<K,T> el2);
	void swap(PairQ<K, T>* el1, PairQ<K, T>* el2);
public:
	PQueue();
	void Add(K elem, T priority);
	void PopBack(T elem);
	T GetMinValue();
	void Set(K elem, T priority);
	
};

template <class K, class T>
bool cmp(PairQ<K, T> el1, PairQ<K, T> el2);