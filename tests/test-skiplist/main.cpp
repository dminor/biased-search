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

#include "biased_skiplist.h"

void runtests(BiasedSkiplist<std::string, int> *sl, const std::vector<std::pair<std::string, int> > &elements)
{
    //try finding the elements
    std::cout << "testing find...\n"; 
    for (size_t i = 0; i < elements.size(); ++i) {
        if (!sl->find(elements[i].first)) {
            std::cerr << "error: find failed to locate element...\n";
        } 
    }

    //try removing half of the elements 
    std::cout << "testing remove...\n"; 
    size_t begin_remove_index = elements.size() / 4;
    size_t end_remove_index = begin_remove_index + elements.size() / 2;

    for (size_t i = begin_remove_index; i < end_remove_index; ++i) { 
        sl->remove(elements[i].first);
    } 

    //try finding the elements 
    for (size_t i = 0; i < elements.size(); ++i) {
        bool found = sl->find(elements[i].first) != 0;

        if ((i < begin_remove_index || i >= end_remove_index) && !found) {
            std::cerr << "error: find failed to locate element " << i << "...\n";
        } else if (i >= begin_remove_index && i < end_remove_index && found) {
            std::cerr << "error: find found deleted element " << i << "...\n"; 
        } 
    }
}

const unsigned int TEST_SIZE = 1000;
const unsigned int STRING_SIZE = 8;

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

    //do tests
    BiasedSkiplist<std::string, int> *sl = new BiasedSkiplist<std::string, int>(20);

    //insert into the hash table 
    for (size_t i = 0; i < TEST_SIZE; ++i) {
        sl->insert(elements[i].first, elements[i].second, rand()%10); 
    } 

    runtests(sl, elements); 

    delete sl;

    return 0;
}

