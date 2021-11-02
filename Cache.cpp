#include "Cache.h"
#include "Dictionary.h"
#include <math.h>
#include "Sequance.h"

#include <iostream>

template <class K,class T>
IntCache<K,T>::IntCache(int sz)
{
	data = new Dictionary<int, CacheNode<K, T>*>();
	arr = new ArrayVector<CacheNode<K, T>*>(sz);
	keysOfLast = new ArrayVector<int>();
	keyOfLast = NULL;
	minCount = NULL;
	que = new PQueue<K,double>();
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
void IntCache<K, T>::AddToCache(CacheNode<K,T>* node)
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
		arr->Set(node, h1);
		totalCount++;
	}
	else
	{
		//CacheNode<K,T>* node = new CacheNode<K,T>(value);
		node->deleted = false;
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
T IntCache<K, T>::GetElem(int key)
{
	int h1 = HashFunc1(key);
	int h2 = HashFunc2(key);
	int i = 0;
	if (Check(key) == true)
	{
		que->Set(key, clock());
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
			que->Add(key, clock());
			AddToCache(dict);
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
					que->PopBack(que->GetMinValue());
					Rehash();
					que->Add(key, clock());
					AddToCache(dict);
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
		if (arr->Get(i) != nullptr) delete arr->Get(i);
	}
	delete arr;

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
		else arr->Set(nullptr, i);
		if (save->Get(i)) delete save->Get(i);
	}
	delete save;
}