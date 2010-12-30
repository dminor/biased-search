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

#include "biased_treap.h"

bool operator<(const std::string &a, const std::string &b)
{
    return a.compare(b) < 0;
}

int main(int argc, char **argv)
{

    //check command line
    if (argc != 3) {
        std::cerr << "usage: -map | -treap | -skiplist | -bst <operations>" << std::endl;
        return 1; 
    }

    //read data file
    std::ifstream data(argv[2]);

    if (!data) {
        std::cout << "error: could not open data file: " << argv[2] << std::endl;
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

                if (itor == map.end()) {
                    std::cout << "bad search" << std::endl;
                }
            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                std::map<std::string, int>::iterator itor = map.find(key);

                if (itor != map.end()) {
                    map.erase(itor);
                } 
            } 
        } 

    } else if (!strcmp(argv[1], "-treap")) {

        BiasedTreap<std::string, int> *treap = new BiasedTreap<std::string, int>;

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
                treap->find(key, result);

                if (result == -1) {
                    std::cout << "bad search" << std::endl;
                }
            } else if (cmd[1] == 'd') { 
                std::string key(&cmd[2]); 

                //delete not implemented
            } 
        }

    } else if (!strcmp(argv[1], "-skiplist")) {

    } else if (!strcmp(argv[1], "-bst")) {

    } else {
        std::cerr << "usage: -map | -treap | -skiplist | -bst <operations>" << std::endl;
        return 1; 
    }

    data.close();
}
