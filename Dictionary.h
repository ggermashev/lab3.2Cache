#pragma once


template <class K, class T>
class Pair
{
public:
	K key;
	T element;
	Pair<K,T>* next;
	Pair<K,T>* prev;
	Pair();
	Pair(K k, T elem);
	Pair(Pair<K, T>* pair_);
};


template <class K, class T>
class Dictionary
{
private:
	int size;
	int count;
	Pair<K,T>* head;
	Pair<K,T>* tail;
public:
	Dictionary();
	int GetSize();
	T Get(K key);
	bool CheckKey(K key);
	void Add(K key, T elem);
	void Remove(K key);
};


namespace Dict {
	template <class K, class T>
	class Node
	{
	public:
		Pair<K, T>* value;
		Node<K, T>* left;
		Node<K, T>* right;
		int height;
		Node();
		Node(Pair<K, T>* pair);
		Node(Node<K, T>* node);
	};
}


template <class K,class T>
class DictionaryBin
{
private:
	Dict::Node<K, T>* root;
	int size;

	int Height(Dict::Node<K, T>* node);
	int Bfactor(Dict::Node<K, T>* node);
	void SetHeight(Dict::Node<K, T>* node);
	Dict::Node<K, T>* LRotate(Dict::Node<K, T>* node);
	Dict::Node<K, T>* RRotate(Dict::Node<K, T>* node);
	Dict::Node<K, T>* Balance(Dict::Node<K, T>* node);
	Dict::Node<K, T>* Add(Pair<K,T>* value_, Dict::Node<K, T>* node);
	Dict::Node<K, T>* Remove(K key, Dict::Node<K, T>* node, bool* flag);
	Dict::Node<K, T>* FindMin(Dict::Node<K, T>* node);
	Dict::Node<K, T>* GetMin(Dict::Node<K, T>* node);
	Dict::Node<K, T>* Get(K key_, Dict::Node<K, T>* node);
	void Clear(Dict::Node<K, T>* node);
public:
	~DictionaryBin();
	DictionaryBin();
	void Add(K key_,T value_);
	void Remove(K key_);
	int GetSize() { return size; }
	T Get(K key_);
	K GetKeyMin();
	K GetKeyMax();
	bool CheckKey(K key_);
	int GetHeight(){ return Height(root); }
	void Clear();

};

