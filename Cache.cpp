#pragma once
#include "Cache.h"
#include "Dictionary.h"
#include <math.h>
#include "Sequance.h"

#include <iostream>

template <class K,class T>
IntCache<K,T>::IntCache(int sz)
{
	data = new DictionaryBin<int, CacheNode<K, T>*>();
	arr = new ArrayVector<CacheNode<K, T>*>(sz);
	keysOfLast = new ArrayVector<int>();
	keyOfLast = NULL;
	minCount = NULL;
	que = new PQueueBin<int,K>();
	for (int i = 0; i < sz; i++)
	{
		arr->Set(nullptr, i);
	}
	count = 0; //кол-во элементов не удаленных
	totalCount = 0; // кол-во элементов с удаленными
	size = sz; //размер таблицы 
}

template <class K, class T>
int IntCache<K, T>::HashFunc1(int key) 
{
	double m, x;
	double a = (sqrt(5) - 1) / 2;
	m = modf(key * a, &x);
	return abs( ( (int) round(size * m) - 1) % size);
}

template <class K, class T>
int IntCache<K, T>::HashFunc2(int key)
{
	double m, x;
	double a = (sqrt(5) - 1) / 2;
	m = modf(key * a, &x);
	return abs( ((int) round(size * m) + 1) % size);
}


template <class K, class T>
void IntCache<K, T>::Add(K key, T value)
{
	CacheNode<K,T>* node = new CacheNode<K,T>(key,value);
	data->Add(key, node);
}


template <class K, class T>
void IntCache<K, T>::AddToCache(CacheNode<K,T>* node, int time)
{
	int h1 = HashFunc1(node->key);
	int h2 = HashFunc2(node->key);

	

	int i = 0;
	int firstDeleted = -1;
	while ((arr->Get(h1) != nullptr) && (i < size))
	{
		if (arr->Get(h1)->key == node->key && arr->Get(h1)->deleted == false)
		{
			return ;
		}
		if (arr->Get(h1)->deleted == true && firstDeleted == -1)
		{
			firstDeleted = h1;
			break;
		}
		h1 = (h1 + h2) % size;
		i++;
	}
	if (firstDeleted == -1)
	{
		//CacheNode<K,T>* node = new CacheNode<K,T>(value); //count
		node->deleted = false;
		node->lastTime = time;
		arr->Set(node, h1);
		totalCount++;
	}
	else
	{
		//CacheNode<K,T>* node = new CacheNode<K,T>(value);
		node->deleted = false;
		node->lastTime = time;
		arr->Set(node, firstDeleted);
		
	}
	count++;
	return;
}

template <class K, class T>
bool IntCache<K, T>::Check(int key)
{
	int h1 = HashFunc1(key);
	int h2 = HashFunc2(key);
	int i = 0;

	while (arr->Get(h1) != nullptr && i < size)
	{
		if (arr->Get(h1)->key == key && arr->Get(h1)->deleted == false)
			return true;
		h1 = (h1 + h2) % size;
		i++;
	}
	return false;
}


template <class K, class T>
int IntCache<K, T>::GetTime(K key)
{
	int h1 = HashFunc1(key);
	int h2 = HashFunc2(key);
	int i = 0;

	while (arr->Get(h1) != nullptr && i < size)
	{
		if (arr->Get(h1)->key == key && arr->Get(h1)->deleted == false)
			return arr->Get(h1)->lastTime;
		h1 = (h1 + h2) % size;
		i++;
	}
	return NULL;
}

template <class K, class T>
void IntCache<K, T>::SetTime(K key, int time)
{
	int h1 = HashFunc1(key);
	int h2 = HashFunc2(key);
	int i = 0;

	while (arr->Get(h1) != nullptr && i < size)
	{
		if (arr->Get(h1)->key == key && arr->Get(h1)->deleted == false)
			break;
		h1 = (h1 + h2) % size;
		i++;
	}
	CacheNode<K, T>* node = arr->Get(h1);
	node->lastTime = time;
	arr->Set(node, i);
	return;
}

template <class K, class T>
T IntCache<K, T>::GetElem(int key)
{
	int h1 = HashFunc1(key);
	int h2 = HashFunc2(key);
	int i = 0;
	if (Check(key) == true)
	{
		int t = GetTime(key);
		int cl = clock();
		que->Set(key, t, cl);
		SetTime(key, cl);
		while (arr->Get(h1) != nullptr && i < size)
		{
			if (arr->Get(h1)->key == key)
			{
				//arr->Get(h1)->count++;
				return arr->Get(h1)->value;
			}
			h1 = (h1 + h2) % size;
			i++;
		}
	}
	else if (data->CheckKey(key))
	{
		CacheNode<K, T>* dict = data->Get(key);
		if (count < size)
		{
			int cl = clock();
			que->Add(key, cl);
			AddToCache(dict,cl);
			return dict->value;
		}
		else
		{
			int h3 = HashFunc1(keyOfLast);
			int h4 = HashFunc2(keyOfLast);
			while (arr->Get(h3) != nullptr && i < size)
			{
				if (arr->Get(h3)->key == key && arr->Get(h3)->deleted == false) 
				{
					Remove(que->GetMinValue()); //keysOfLast
					que->PopBack();
					if (totalCount / count >= 1.5)
					{
						Rehash();
						int cl = clock();
						que->Add(key, cl);
						AddToCache(dict,cl);
					}
					break;
				}
				h3 = (h3 + h4) % size;
				i++;
			}
			
			return dict->value;
		}	
	}
	return NULL;
}

template <class K, class T>
void IntCache<K, T>::Remove(T key)
{


	int h1 = HashFunc1(key);
	int h2 = HashFunc1(key);
	int i = 0;
	while (arr->Get(h1) != nullptr && i < size)
	{
		if (arr->Get(h1)->key == key && arr->Get(h1)->deleted == false)
		{
			arr->Get(h1)->deleted = true;
			count--;
			return;
		}
		h1 = (h1 + h2) % size;
		i++;
	}

	return;
}

template <class K, class T>
void IntCache<K, T>::Rehash()
{
	ArrayVector<CacheNode<K, T>*>* save = new ArrayVector<CacheNode<K, T>*>(size);
	for (int i = 0; i < size; i++)
	{
		if (arr->Get(i) != nullptr) save->Set(arr->Get(i), i);
		if (arr->Get(i) == nullptr) delete arr->Get(i);
	}
	delete [] arr;

	count = 0;
	totalCount = 0;
	arr = new ArrayVector<CacheNode<K, T>*>(size);
	for (int i = 0; i < size; i++)
	{
		if (save->Get(i) == nullptr) arr->Set(nullptr, i);
		else if (save->Get(i)->deleted == false)
		{
			arr->Set(save->Get(i), i);
			count++;
			totalCount++;
		}
		else
		{
			arr->Set(nullptr, i);
		}
		if (save->Get(i)) delete save->Get(i);
	}
	delete [] save;
}

template <class K, class T>
void IntCache<K, T>::Clear()
{
	data->Clear();
	arr->Clear();
	que->Clear();
}
//---------------------------------------------------HashTable----------------------------------------------------------------------------------

template <class K, class T>
int HashTable<K, T>::HashFunc(int key)
{
	/*
	double m, x;
	double a = (sqrt(5) + 1) / 2;
	m = modf((key * a), &x);
	return (int)x % size;
	*/
	double m, x;
	double a = (sqrt(5) + 1) / 2;
	m = modf(key * a, &x);
	//modf(size * m, &x);
	return abs( (int)x  % size);
}


template <class K, class T>
HashTable<K, T>::HashTable(int size_)
{
	size = size_;
	arr = new ArrayVector<ListVector<CacheNode<K, T>*>*>(size);
	for (int i = 0; i < size; i++)
	{
		ListVector<CacheNode<K, T>*>* vec = new ListVector<CacheNode<K, T>*>();
		arr->Set(vec, i);
		vec = nullptr;
	}
	count = 0;
}


template <class K, class T>
void HashTable<K, T>::Add(K key_, T elem_, int clock)
{
	int h = HashFunc(key_);
	ListVector<CacheNode<K, T>*>* vec = arr->Get(h);
	CacheNode<K, T>* node = new CacheNode<K, T>(key_, elem_,clock);
	vec->Prepend(node);

	if (vec->GetLength() > maxListLength) maxListLength = vec->GetLength();

	arr->Set(vec, h);
	count++;
}

template <class K, class T>
void HashTable<K, T>::SetTime(K key_, int clock)
{
	int h = HashFunc(key_);
	ListVector<CacheNode<K, T>*>* vec = arr->Get(h);
	if (vec)
		for (int i = 0; i < vec->GetLength(); i++)
		{
			if (vec->Get(i)->key == key_) {
				CacheNode<K, T>* node = vec->Get(i);
				node->lastTime = clock;
				vec->Set(node, i);
				break;
			}
		}
	arr->Set(vec, h);
}


template <class K, class T>
T HashTable<K, T>::GetElem(int key_)
{
	int h = HashFunc(key_);
	if (arr->Get(h))
		for (int i = 0; i < arr->Get(h)->GetLength(); i++)
		{
			if (arr->Get(h)->Get(i)->key == key_) return arr->Get(h)->Get(i)->value;
		}
	return NULL;
}


template <class K, class T>
void HashTable<K, T>::Remove(int key_)
{
	int h = HashFunc(key_);
	ListVector<CacheNode<K, T>*>* vec = arr->Get(h);
	for (int i = 0; i < vec->GetLength(); i++)
	{
		if (vec->Get(i)->key == key_)
		{
			vec->Remove(i);
			arr->Set(vec, h);
			count--;
			return;
		}
	}
	
}


template <class K, class T>
bool HashTable<K, T>::Check(int key_)
{
	int h = HashFunc(key_);
	for (int i = 0; i < arr->Get(h)->GetLength(); i++)
	{
		if (arr->Get(h)->Get(i))
		if (arr->Get(h)->Get(i)->key == key_) return true;
	}
	return false;
}

template <class K, class T>
int HashTable<K, T>::GetTime(int key_)
{
	int h = HashFunc(key_);
	for (int i = 0; i < arr->Get(h)->GetLength(); i++)
	{
		if (arr->Get(h)->Get(i))
			if (arr->Get(h)->Get(i)->key == key_) return arr->Get(h)->Get(i)->lastTime;
	}
	return NULL;
}

template <class K, class T>
CacheNode<K, T>* HashTable<K, T>::GetNode(int key_)
{
	int h = HashFunc(key_);
	for (int i = 0; i < arr->Get(h)->GetLength(); i++)
	{
		if (arr->Get(h)->Get(i))
			if (arr->Get(h)->Get(i)->key == key_) return arr->Get(h);
	}
	return NULL;
}

template <class K, class T>
int HashTable<K, T>::GetSize()
{
	return size;
}


template <class K, class T>
int HashTable<K, T>::GetCount()
{
	return count;
}

template <class K, class T>
void HashTable<K, T>::Clear()
{
	for (int i = 0; i < arr->GetLength(); i++)
	{
		for (int j = 0; j < arr->Get(i)->GetLength(); j++)
		{
			delete arr->Get(i)->Get(j);
		}
		delete arr->Get(i);
	}
	delete arr;
}

template <class K, class T>
HashTable<K, T>::~HashTable()
{
	Clear();
}

//-------------------------------------------------------------Cache#2-----------------------------------------------------------------



template <class K, class T>
IntCache2<K, T>::IntCache2(int size_)
{
	data = new DictionaryBin<K, T>();
	table = new HashTable<K, T>(size_);
	que = new PQueueBin<int, K>();
}



template <class K, class T>
void IntCache2<K, T>::Add(K key_, T elem_)
{
	data->Add(key_, elem_);
}



template <class K, class T>
T IntCache2<K, T>::GetElem(K key_)
{	
	if (table->Check(key_) == true)
	{
		//std::cout << "*\n";
		int time = table->GetTime(key_);
		int cl = clock();
		
		que->Set(key_, time, cl); //
		table->SetTime(key_, cl);
		return table->GetElem(key_);
	}
	else	
	{
		//std::cout << "**\n";
		T ret = data->Get(key_);
		
		if (table->GetCount() < table->GetSize())
		{
			
			int cl = clock();
			
			table->Add(key_, ret, cl);
			que->Add(key_, cl);
			
		}
		else
		{
			int key = que->GetMinValue();
			table->Remove(key);
			que->PopBack();

			int cl = clock();
			table->Add(key_, ret, cl);
			que->Add(key_, cl);
		}
		
		return ret;
	}
}


template <class K, class T>
void IntCache2<K, T>::Clear( )
{
	data->Clear();
	table->Clear();
	que->Clear();
}
