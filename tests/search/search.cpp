/*
Copyright (c) 2010 Daniel Minor 

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

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "biased_hashtable.h"
#include "biased_skiplist.h"
#include "biased_treap.h"

const size_t MURMURHASH2_SEED = 0x5432FEDC;

size_t MurmurHash2 ( const void * key, int len, unsigned int seed );

bool operator<(const std::string &a, const std::string &b)
{
    return a.compare(b) < 0;
}

size_t hash(const std::string &key)
{
    return MurmurHash2(key.c_str(), key.size(), MURMURHASH2_SEED);
}

int main(int argc, char **argv)
{

    //check command line
    if (argc != 3) {
        std::cerr << "usage: -map | -treap | -skiplist | -hashtable | -nop <operations>" << "\n";
        return 1; 
    }

    //read data file
    std::ifstream data(argv[2]);

    if (!data) {
        std::cerr << "error: could not open data file: " << argv[2] << "\n"; 
        return 1;
    } 

    srand(time(0));

    //see which data structure to use and run operations on it
    if (!strcmp(argv[1], "-map")) {

        std::map<std::string, int> map;

        char cmd[80];
        while (!data.eof()) {
            data.getline(cmd, 80); 

            if (cmd[0] == 'i') {

                //extract word
                size_t i = 2;
                while (cmd[i] != ' ') ++i;
                cmd[i] = 0;
                std::string key(&cmd[2]);

                //extract weight
                ++i;
                float weight = atof(&cmd[i]);

                map[key] = 0;
            } else if (cmd[0] == 's') {
                std::string key(&cmd[2]); 

                std::map<std::string, int>::iterator itor = map.find(key);

                if (itor != map.end()) std::cout << key << ": " << itor->second << "\n";
                else std::cout << key << ": not found" << "\n"; 

            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                std::map<std::string, int>::iterator itor = map.find(key);

                if (itor != map.end()) {
                    map.erase(itor);
                } 
            } 
        } 

    } else if (!strcmp(argv[1], "-treap")) {

        BiasedTreap<std::string, int> *treap = new BiasedTreap<std::string, int>(false);

        char cmd[80];
        while (!data.eof()) {
            data.getline(cmd, 80); 

            if (cmd[0] == 'i') {

                //extract word
                size_t i = 2;
                while (cmd[i] != ' ') ++i;
                cmd[i] = 0;
                std::string key(&cmd[2]);

                //extract weight
                ++i;
                size_t weight = atoi(&cmd[i]);
                //std::cout << weight << std::endl;

                treap->insert(key, 0, weight); 
            } else if (cmd[0] == 's') {
                std::string key(&cmd[2]); 

                int result = -1;
                if (treap->find(key, result)) {
                    std::cout << key << ": " << result << "\n"; 
                } else { 
                    std::cout << key << ": not found" << "\n";
                }

            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                //delete not implemented
            } 
        }

    } else if (!strcmp(argv[1], "-skiplist")) {

        BiasedSkiplist<std::string, int> *skiplist = new BiasedSkiplist<std::string, int>(15);

        char cmd[80];
        while (!data.eof()) {
            data.getline(cmd, 80); 

            if (cmd[0] == 'i') {

                //extract word
                size_t i = 2;
                while (cmd[i] != ' ') ++i;
                cmd[i] = 0;
                std::string key(&cmd[2]);

                //extract weight
                ++i;
                size_t weight = atoi(&cmd[i]);
                //std::cout << weight << std::endl;

                skiplist->insert(key, 0, weight); 
            } else if (cmd[0] == 's') {
                std::string key(&cmd[2]); 

                int result = -1;
                if (skiplist->find(key, result)) {
                    std::cout << key << ": " << result << "\n"; 
                } else { 
                    std::cout << key << ": not found" << "\n"; 
                }

            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                //delete not implemented
            } 
        }

        skiplist->dump_tree();

    } else if (!strcmp(argv[1], "-hashtable")) {
        BiasedHashtable<std::string, int> *ht = new BiasedHashtable<std::string, int>(10000, hash, false);

        char cmd[80];
        while (!data.eof()) {
            data.getline(cmd, 80); 

            if (cmd[0] == 'i') {

                //extract word
                size_t i = 2;
                while (cmd[i] != ' ') ++i;
                cmd[i] = 0;
                std::string key(&cmd[2]);

                //extract weight
                ++i;
                size_t weight = atoi(&cmd[i]);
                //std::cout << weight << std::endl;

                ht->insert(key, 0, weight); 
            } else if (cmd[0] == 's') {
                std::string key(&cmd[2]); 

                int result = -1;
                if (ht->find(key, result)) {
                    std::cout << key << ": " << result << "\n"; 
                } else { 
                    std::cout << key << ": not found" << "\n"; 
                }

            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                //delete not implemented
            } 
        }
    } else if (!strcmp(argv[1], "-nop")) {

        char cmd[80];
        while (!data.eof()) {
            data.getline(cmd, 80); 

            if (cmd[0] == 'i') {

                //extract word
                size_t i = 2;
                while (cmd[i] != ' ') ++i;
                cmd[i] = 0;
                std::string key(&cmd[2]);

                //extract weight
                ++i;
                size_t weight = atoi(&cmd[i]); 

            } else if (cmd[0] == 's') {
                std::string key(&cmd[2]); 

                int result = -1;
                std::cout << key << ": " << result << "\n"; 
            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                //delete not implemented
            } 
        }

    } else {
        std::cerr << "usage: -map | -treap | -skiplist | -hashtable | -nop <operations>" << "\n"; 
        return 1; 
    }

    data.close();
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
