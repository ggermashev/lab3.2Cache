

#include <iostream>
#include "Cache.h"
#include <fstream>
#include "Tests.h"


#include "Cache.cpp"
#include "Dictionary.cpp"
#include "Sequance.cpp"
#include "DynamicArray.cpp"
#include "PriorityQueue.cpp"


int main()
{
    /*
    std::ofstream outf("Time.txt",std::ios::app);
    srand(time(NULL));
    for (int k = 100000; k <= 100000; k += 100000)
    {
        IntCache<int, int>* cache = new IntCache<int, int>(20000);
        IntCache2<int, int>* cache2 = new IntCache2<int, int>(20000);
        DictionaryBin<int, int>* dict = new DictionaryBin<int, int>();
        HashTable<int, int>* table = new HashTable<int, int>(20000);

        for (int i = 0; i < 100000; i++)
        {
            int value = i;
            cache->Add(i, value);
            cache2->Add(i, value);
            dict->Add(i, value);
            table->Add(i, value);
        }
        std::cout << "*\n";
        int time1 = 0;
        int totaltime1 = 0;
        int totaltime2 = 0;
        int totaltime3 = 0;
        int totaltime4 = 0;
        for (int i = 0; i < k; i++)
        {

            int key = abs(rand() % 30000) + 8000;


            time1 = clock();
           // table->GetElem(key);
           // totaltime1 += clock() - time1;
            time1 = clock();
            cache->GetElem(key);
            totaltime2 += clock() - time1;
            time1 = clock();
            cache2->GetElem(key);
            totaltime3 += clock() - time1;
            time1 = clock();
            dict->Get(key);
            totaltime4 += clock() - time1;
            //std::cout << i << "\n";
        }
        //std::cout << cache2->GetMaxListLength();
        table->Clear();
        cache->Clear();
        cache2->Clear();
        dict->Clear();
        table = nullptr;
        cache = nullptr;
        cache2 = nullptr;
        dict = nullptr;
        
        outf << "hashtable: " << totaltime1 << " || " << "Cache: " << totaltime2 << " || " << "cache2: " << totaltime3 << " || dict:" << totaltime4 << "\n";

    }
    */
    
    


    TestCache();
    TestPQueue();
    TestDictionary();

    return 0;
}

