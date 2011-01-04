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

#ifndef BIASED_HASHTABLE_H_
#define BIASED_HASHTABLE_H_

template<class K, class V> class BiasedHashtable {

public:

    typedef size_t (*HashFunction)(const K &key);

    BiasedHashtable(size_t initial_size, HashFunction hash) : count(0), size(initial_size), hash(hash)
    {
        nodes = new Node[size]; 
    } 

    virtual ~BiasedHashtable()
    {
        delete[] nodes; 
    }

    void insert(const K &key, const V &value, size_t weight)
    {
        insert(key, value, weight, nodes, size);
        ++count;
        if (count * 10 > size * 9) rebuild();
    } 

    bool find(const K &key, V &result)
    {
        bool found = false;
        size_t index = hash(key) % size;
        while (nodes[index].stored) {
            if (nodes[index].key == key) {
                found = true;
                result = nodes[index].value;
                break;
            }

            ++index;
        }

        return found;
    }

    void remove(const K &key)
    {
        size_t index = hash(key) % size;
        while (nodes[index].stored) {
            if (nodes[index].key == key) {
                nodes[index].stored = false;
                --count;
                break;
            }
        }
    }

    void reweight(const K &key, size_t weight)
    {

    } 

private:

    struct Node {
        K key;
        V value; 
        size_t weight;
        bool stored;

        Node() : stored(false) {};
    };
 
    Node *nodes;
    size_t count;
    size_t size; 
    HashFunction hash;

    void rebuild()
    {
        size_t new_size = size * 2;
        Node *new_nodes = new Node[new_size];

        for (size_t i = 0; i < size; ++i) {
            if (nodes[i].stored) insert(nodes[i].key, nodes[i].value, nodes[i].weight, new_nodes, new_size);
        }

        delete[] nodes;
        nodes = new_nodes;
        size = new_size;
    }

    void insert(const K &key, const V &value, size_t weight, Node *nodes, size_t size)
    {
        size_t index = hash(key) % size;
        if (!nodes[index].stored) {
            //just insert node if no collision
            nodes[index].key = key;
            nodes[index].value = value;
            nodes[index].weight = weight;
            nodes[index].stored = true;

            std::cout << "inserted: " << key << std::endl;
        } else {
            std::cout << "collision: " << key << " index: " << index << std::endl;

            //find the first index of lower weight
            size_t index_to_use = index + 1; 
            while(nodes[index_to_use % size].stored && nodes[index_to_use % size].weight < weight) ++index_to_use; 

            std::cout << "index_to_use: " << index_to_use << std::endl;

            //find the first empty index
            size_t first_empty = index_to_use;
            while(nodes[first_empty % size].stored) ++first_empty; 

            std::cout << "first_empty: " << first_empty << std::endl;

            //shift nodes into place and copy in our new node
            for (size_t i = first_empty; i > index_to_use; --i) nodes[i % size] = nodes[(i - 1) % size];
            nodes[index_to_use % size].key = key;
            nodes[index_to_use % size].value = value;
            nodes[index_to_use % size].weight = weight; 
        } 
    }
};

#endif 

