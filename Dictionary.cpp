#pragma once
#include "Dictionary.h"
#include <fstream>
//-----------------------Pair--------------------

template <class K, class T>
Pair<K, T>::Pair()
{
	key = NULL;
	element = NULL;
	next = nullptr;
	prev = nullptr;
}

template <class K,class T>
Pair<K, T>::Pair(K k, T elem)
	:Pair()
{
	key = k;
	element = elem;
	next = nullptr;
	prev = nullptr;
}

template <class K, class T>
Pair<K, T>::Pair(Pair<K,T>* pair)
	:Pair()
{
	if (pair)
	{
		key = pair->key;
		element = pair->element;
		next = pair->next;
		prev = pair->prev;
	}
}


//-------------Dictionary------------------------------------

template <class K, class T>
Dictionary<K,T>::Dictionary()
{
	size = 0;
	count = 0;
	head = nullptr;
	tail = nullptr;
}


template <class K, class T>
int Dictionary<K, T>::GetSize()
{
	return size;
}

template <class K, class T>
T Dictionary<K, T>::Get(K k)
{
	Pair<K, T>* ptr = head;
	while (ptr != nullptr)
	{
		if (ptr->key == k) return ptr->element;
		ptr = ptr->next;
	}
}

template <class K, class T>
bool Dictionary<K, T>::CheckKey(K k)
{
	Pair<K, T>* ptr = head;
	while (ptr != nullptr)
	{
		if (ptr->key == k) return true;
		ptr = ptr->next;
	}
	return false;
}

template <class K, class T>
void Dictionary<K, T>::Add(K k, T elem)
{
	if (!head)
	{
		Pair<K, T>* ptr = new Pair<K, T>(k, elem);
		head = ptr;
		tail = ptr;
		count = 1;
		size = 1;
		return;
	}
	Pair<K, T>* ptr = tail;
	ptr->next = new Pair<K, T>(k, elem);
	ptr = ptr->next;
	ptr->prev = tail;
	tail = ptr;
	count++;
	size++;
	return;
}

template <class K, class T>
void Dictionary<K, T>::Remove(K k)
{
	if (!head) return;
	Pair<K, T>* ptr = head;
	while (ptr != nullptr)
	{
		if (ptr->key == k)
		{
			if ((ptr != head) && (ptr != tail))
			{
				ptr->next->prev = ptr->prev;
				ptr->prev->next = ptr->next;
			}
			else if (ptr == head)
			{
				head = ptr->next;
				ptr->next->prev = nullptr;
			}
			else if (ptr == tail)
			{
				tail = ptr->prev;
				ptr->prev->next = nullptr;
			}
			delete ptr;
		}
	}
	return;
}

//---------------------------------------------------------Bin---------------------------------------------------

template <class K, class T>
Dict::Node<K, T>::Node()
{
	value = new Pair<K, T>();
	left = nullptr;
	right = nullptr;
	height = 0;
}

template <class K, class T>
Dict::Node<K, T>::Node(Pair<K,T>* pair)
{
	value = pair;
	left = nullptr;
	right = nullptr;
	height = 0;
}

template <class K, class T>
int DictionaryBin<K, T>::Height(Dict::Node<K, T>* node)
{
	if (!node) return 0;
	return node->height;
}

template <class K, class T>
int DictionaryBin<K, T>::Bfactor(Dict::Node<K, T>* node)
{
	if (!node) return 0;
	return Height(node->right) - Height(node->left);
}

template <class K, class T>
void DictionaryBin<K, T>::SetHeight(Dict::Node<K, T>* node)
{
	if (node)
	{
		if (Height(node->left) > Height(node->right))
			node->height = Height(node->left) + 1;
		else
			node->height = Height(node->right) + 1;
	}
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::LRotate(Dict::Node<K, T>* node)
{
	Dict::Node<K, T>* q = node->left;
	node->left = q->right;
	q->right = node;
	SetHeight(node);
	SetHeight(q);
	return q;
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::RRotate(Dict::Node<K, T>* node)
{
	Dict::Node<K, T>* q = node->right;
	node->right = q->left;
	q->left = node;
	SetHeight(node);
	SetHeight(q);
	return q;
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::Balance(Dict::Node<K, T>* node)
{
	SetHeight(node);
	if (Bfactor(node) > 1)
	{
		if (Bfactor(node->right) < 0)
			node->right = LRotate(node->right);
		return RRotate(node);
	}
	else if (Bfactor(node->left) < -1)
	{
		if (Bfactor(node->left) > 0)
			node->left = RRotate(node->left);
		return LRotate(node);
	}
	return node;
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::Add(Pair<K,T>* value_, Dict::Node<K, T>* node)
{
	if (!node)
	{
		node = new Dict::Node<K, T>(value_);
		size++;
		return node;
	}
	else if (value_->key < node->value->key)
	{
		node->left = Add(value_, node->left);
	}
	else
		node->right = Add(value_, node->right);
	return Balance(node);
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::Remove(K key_, Dict::Node<K, T>* node, bool* flag)
{
	if (!node)
	{
		*flag = false;
		return nullptr;
	}
	if (key_ < node->value->key)
		node->left = Remove(key_, node->left, flag);
	else if (key_ > node->value->key)
		node->right = Remove(key_ ,node->right, flag);
	else
	{
		Dict::Node<K, T>* l = node->left;
		Dict::Node<K, T>* r = node->right;
		delete node;
		if (!r) return l;
		Dict::Node<K, T>* min = FindMin(r);
		min->right = GetMin(r);
		min->left = l;
		return Balance(min);
	}
	return Balance(node);
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::FindMin(Dict::Node<K, T>* node)
{
	if (node->left)
		return FindMin(node->left);
	else
		return node;
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::GetMin(Dict::Node<K, T>* node)
{
	if (!node->left)
		return node->right;
	node->left = GetMin(node->left);
	return Balance(node);
}

template <class K, class T>
Dict::Node<K, T>* DictionaryBin<K, T>::Get(K key_, Dict::Node<K,T>* node)
{
	if (!node)
		return nullptr;
	else if (key_ < node->value->key)
		Get(key_, node->left);
	else if (key_ > node->value->key)
		Get(key_, node->right);
	else
	{
		return node;
	}
}

template <class K, class T>
void DictionaryBin<K, T>::Clear(Dict::Node<K, T>* node)
{
	if (node)
	{
		Clear(node->left);
		Clear(node->right);
		delete node;
		node = nullptr;
	}
}

template <class K, class T>
DictionaryBin<K, T>::DictionaryBin()
{
	root = nullptr;
	size = 0;
}

template <class K, class T>
DictionaryBin<K, T>::~DictionaryBin()
{
	Clear();
}


template <class K, class T>
void DictionaryBin<K, T>::Add(K key_,T value_)
{
	Pair<K, T>* pair = new Pair<K, T>(key_, value_);
	root = Add(pair, root);
}

template <class K, class T>
void DictionaryBin<K, T>::Remove(K key_)
{
	if (!root)
		throw std::runtime_error("\nErrorRemove: Dict is empty\n");
	bool *flag = new bool;
	*flag = true;
	root = Remove(key_, root, flag);
	size--;
	if (!flag)
	{
		throw std::runtime_error("\nErrorRemove: Element doesn't exist\n");
	}
}

template <class K, class T>
T DictionaryBin<K, T>::Get(K key_)
{
	Dict::Node<K, T>* node = Get(key_, root);
	if (!node) return NULL;
	return node->value->element;
}

template <class K, class T>
K DictionaryBin<K, T>::GetKeyMin()
{
	Dict::Node<K, T>* node = root;
	while (node->left) node = node->left;
	return node->value->key;
}

template <class K, class T>
K DictionaryBin<K, T>::GetKeyMax()
{
	Dict::Node<K, T>* node = root;
	while (node->right) node = node->right;
	return node->value->key;
}

template <class K, class T>
bool DictionaryBin<K, T>::CheckKey(K key_)
{
	Dict::Node<K, T>* node = Get(key_, root);
	if (!node) return false;
	return true;
}

template <class K, class T>
void DictionaryBin<K, T>::Clear()
{
	Clear(root);
}

