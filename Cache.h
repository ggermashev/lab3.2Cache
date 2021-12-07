#pragma once

#include "Dictionary.h"
#include "Sequance.h"
#include "PriorityQueue.h"


template <class K,class T>
class CacheNode
{
public:
	T value;
	K key;
	int lastTime = 0;
	bool deleted;
	int count;
	CacheNode(K key_, T value_) { value = value_;	key = key_;		deleted = false;	count = 0; }
	CacheNode(K key_, T value_, int time) :CacheNode(key_, value_) { lastTime = time; }
	CacheNode() { count = 0; }
};


/*
template <class K, class T>
class ICache
{

public:
	virtual int HashFunc1(K key) = 0;
	virtual int HashFunc2(K key) = 0;
	//virtual bool CheckElement(K key) = 0;
	virtual void AddToCache(CacheNode<K,T>*) = 0;
	virtual void Remove(T key) = 0;
	virtual void Rehash() = 0;
	//virtual void Resize() = 0;
	//virtual T GetElem(K key) = 0;

};
*/

template <class K,class T>
class IntCache
{
private:
	DictionaryBin<int, CacheNode<K,T>*>* data;
	
	ArrayVector<CacheNode<K,T>*>* arr;
	int count; //кол-во элементов не удаленных
	int totalCount; // кол-во элементов с удаленными
	int size; //размер таблицы 
	int keyOfLast;
	PQueueBin<int ,K>* que;
	ArrayVector<int>* keysOfLast;
	int minCount;
	const double level = 0.75;
	void AddToCache(CacheNode<K,T>* node, int time = 0);
	void Rehash() ;
	int HashFunc1(int key)  ;
	int HashFunc2(int key) ;
	int GetTime(K key);
	void SetTime(K key, int time);
public:
	
	IntCache(int sz);
	void Add(K key, T value);
	T GetElem(int key);
	void Remove(T key) ;
	bool Check(int key);
	void Clear();
	~IntCache() { Clear(); }
};


template <class K, class T>
class HashTable
{
private:
	ArrayVector<ListVector<CacheNode<K, T>*>*>* arr;
	int size;
	int count;

	int maxListLength = 0;
	CacheNode<K, T>* GetNode(int key);
	int HashFunc(int key);
	
public:
	HashTable(int size_);
	void Add(K key, T elem, int clock = 0);
	void SetTime(K key, int clock);
	T GetElem(int key);
	void Remove(int key);
	bool Check(int key);
	int GetTime(int key);
	int GetSize();
	int GetCount();
	void Clear();
	~HashTable();

	int GetMaxListLength() { return maxListLength; }
};


template <class K, class T>
class IntCache2
{
private:
	DictionaryBin<K, T>* data;
	HashTable<K, T>* table;
	PQueueBin<int, K>* que;

	//void AddToTable(K key, T elem);
public:
	IntCache2(int size_);
	void Add(K key, T elem);
	T GetElem(K key);
	int GetMaxListLength() { return table->GetMaxListLength(); }
	void Clear();
	~IntCache2() { Clear(); }
};