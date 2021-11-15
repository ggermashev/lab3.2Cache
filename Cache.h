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
	bool deleted;
	int count;
	CacheNode(K key_, T value_) { value = value_;	key = key_;		deleted = false;	count = 0; }
	CacheNode() { count = 0; }
};



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

template <class K,class T>
class IntCache: ICache<K,T>
{
private:
	Dictionary<int, CacheNode<K,T>*>* data;
	
	ArrayVector<CacheNode<K,T>*>* arr;
	int count; //кол-во элементов не удаленных
	int totalCount; // кол-во элементов с удаленными
	int size; //размер таблицы 
	int keyOfLast;
	PQueue<K ,double>* que;
	ArrayVector<int>* keysOfLast;
	int minCount;
	const double level = 0.75;
	void AddToCache(CacheNode<K,T>* node) override;
	void Rehash() override;
	int HashFunc1(int key) override ;
	int HashFunc2(int key) override;
	//bool CheckElement(int key) override;
public:
	
	IntCache(int sz);
	void Add(K key, T value);
	T GetElem(int key);
	void Remove(T key) override;
	bool Check(int key);

	~IntCache();
};