/*
Copyright (c) 2011 Daniel Minor 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <cstdlib>
#include <iostream>
#include <string> 
#include <vector>

#include "biased_hashtable.h"

void runtests(BiasedHashtable<std::string, int> *ht, const std::vector<std::pair<std::string, int> > &elements)
{
    //try finding the elements
    std::cout << "testing find...\n"; 
    for (size_t i = 0; i < elements.size(); ++i) {
        int v;
        if (!ht->find(elements[i].first, v)) {
            std::cerr << "error: find failed to locate element...\n";
        } 
    }

    //try removing half of the elements 
    std::cout << "testing remove...\n"; 
    size_t begin_remove_index = elements.size() / 4;
    size_t end_remove_index = begin_remove_index + elements.size() / 2;

    for (size_t i = begin_remove_index; i < end_remove_index; ++i) { 
        ht->remove(elements[i].first);
    } 

    //try finding the elements 
    for (size_t i = 0; i < elements.size(); ++i) {
        int v;
        bool found = ht->find(elements[i].first, v);

        if ((i < begin_remove_index || i >= end_remove_index) && !found) {
            std::cerr << "error: find failed to locate element " << i << "...\n";
        } else if (i >= begin_remove_index && i < end_remove_index && found) {
            std::cerr << "error: find found deleted element " << i << "...\n"; 
        } 
    }
}

const size_t MURMURHASH2_SEED = 0x5432FEDC;

size_t MurmurHash2 ( const void * key, int len, unsigned int seed );

size_t hash(const std::string &key)
{
    return MurmurHash2(key.c_str(), key.size(), MURMURHASH2_SEED);
}

const int TEST_SIZE = 1000;
const int STRING_SIZE = 8;

int main(int argc, char **argv)
{
    //create some elements to test against
    std::vector<std::pair<std::string, int> > elements;
    for (size_t i = 0; i < TEST_SIZE; ++i) {

        //random string
        char k[STRING_SIZE];
        for (size_t j = 0; j < STRING_SIZE - 1; ++j) {
            k[j] = (char)(96 + rand()%25);
        }
        k[STRING_SIZE - 1] = 0;

        elements.push_back(std::make_pair<std::string, int>(k, i + 1));
    }

    //do tests in non-adaptive mode
    std::cout << "testing in non-adaptive mode\n";
    BiasedHashtable<std::string, int> *ht = new BiasedHashtable<std::string, int>(8, hash, false);

    //insert into the hash table 
    for (size_t i = 0; i < TEST_SIZE; ++i) {
        ht->insert(elements[i].first, elements[i].second, rand()%10); 
    } 

    runtests(ht, elements); 

    delete ht;

    //do tests in adaptive mode
    std::cout << "testing in adaptive mode\n";
    ht = new BiasedHashtable<std::string, int>(8, hash, true);

    //insert into hash table
    for (size_t i = 0; i < TEST_SIZE; ++i) {
        ht->insert(elements[i].first, elements[i].second, rand()%10); 
    } 

    runtests(ht, elements);

    delete ht;

    return 0;
}

//-----------------------------------------------------------------------------
// MurmurHash2, by Austin Appleby

// Note - This code makes a few assumptions about how your machine behaves -

// 1. We can read a 4-byte value from any address without crashing
// 2. sizeof(int) == 4

// And it has a few limitations -

// 1. It will not work incrementally.
// 2. It will not produce the same results on little-endian and big-endian
//    machines.

unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}
