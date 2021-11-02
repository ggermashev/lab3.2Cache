

#include <iostream>
#include "Cache.h"
#include "Cache.cpp"
#include "Dictionary.cpp"
#include "Sequance.cpp"
#include "DynamicArray.cpp"
#include "PriorityQueue.cpp"


int main()
{
    IntCache<int, int>* cache = new IntCache<int, int>(10);
    Dictionary<int,int>* dict = new Dictionary<int, int>();
    for (int i = 0; i < 10000; i++)
    {
        int value = i;
        cache->Add(i,value);
        dict->Add(i, value);
    }
    double time1 = 0;
    double time2 = 0;
    double totaltime1 = 0;
    double totaltime2 = 0;
    for (int i = 0; i < 1000000; i++)
    {
       
        int key = abs( rand() % 15 * 500 );
       // if (i % 2 == 0) key = i % 100;
        time1 = clock();
        cache->GetElem(key);
        totaltime1 += clock() - time1;
        time2 = clock();
        dict->Get(key);
        totaltime2 += clock() - time2;
    }
    std::cout << "cache: " << totaltime1 << " ||||| dict: " << totaltime2 << std::endl;
}

