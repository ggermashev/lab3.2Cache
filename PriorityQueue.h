# pragma once

#include "Sequance.h"
#include "Dictionary.h"

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
	int BinSearch(K key_, T priority, int l = 0, int r = -11);
public:
	PQueue();
	void Add(K elem, T priority);
	void PopBack();
	K GetMinValue();
	void Set(K elem, T time, T priority);
	void Clear();
	~PQueue();
};

template <class K, class T>
class PQueueBin
{
private:
	DictionaryBin<K,T>* pair;
public:
	PQueueBin();
	void Add(T elem, K priority);
	void PopBack();
	K GetMinValue();
	void Set(T elem, K time, K priority);
	void Clear();
	~PQueueBin();
};

template <class K, class T>
bool cmp(PairQ<K, T> el1, PairQ<K, T> el2);