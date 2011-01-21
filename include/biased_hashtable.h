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

    BiasedHashtable(size_t initial_size, HashFunction hash, bool adapt_weights) : count(0), size(initial_size), hash(hash), adapt_weights(adapt_weights)
    {
        nodes = new Node[size]; 
    } 

    virtual ~BiasedHashtable()
    {
        for (size_t i = 0; i < size; ++i) {
            Node *n = nodes[i].next;
            while(n) {
                Node *t = n;
                n = n->next;
                delete t;
            }
        }

        delete[] nodes; 
    }

    void insert(const K &key, const V &value, size_t weight)
    {
        size_t index = hash(key) % size;
        if (!nodes[index].stored) {
            //just insert node if no collision
            nodes[index].key = key;
            nodes[index].value = value;
            nodes[index].weight = weight;
            nodes[index].stored = true;
        } else {

            //find place to insert node in list
            Node *n = &nodes[index];
            while(n->next && n->next->weight > weight) n = n->next;

            //create node and insert
            Node *t = n->next;
            n->next = new Node;
            n->next->key = key;
            n->next->value = value;
            n->next->weight = weight;
            n->next->next = t;
        } 

        ++count;
    } 

    bool find(const K &key, V &result)
    {
        bool found = false;
        size_t index = hash(key) % size;
        if (nodes[index].stored) {
            Node *p = 0; 
            Node *n = &nodes[index];
            while (n && n->key != key) {
                p = n;
                n = n->next;
            }

            if (n) {
                found = true;
                result = n->value;

                //if adapting weights, reweight and make sure in proper order
                if (adapt_weights) {
                    ++n->weight;

                    //if our weight is greater than our predecessor's
                    //resinsert at proper place in list
                    if (p && n->weight > p->weight) {
                        p->next = n->next;

                        //find place to insert node in list
                        p = &nodes[index];
                        while(p->next && p->next->weight > n->weight) p = p->next; 
                        n->next = p->next;
                        p->next = n;

                    }
                }
            }
        }

        return found;
    }

    void remove(const K &key)
    { 
        size_t index = hash(key) % size;
        if (nodes[index].stored) {
            Node *p = 0; 
            Node *n = &nodes[index];
            while (n && n->key != key) {
                p = n;
                n = n->next;
            }

            //if we found n
            if (n) {
                //if it is in the original node
                if (n == &nodes[index]) {
                    if (n->next == 0) {
                        //if this is only node, just clear the stored flag
                        nodes[index].stored = false;
                    } else {
                        //otherwise, copy next node into original node 
                        Node *t = n->next;
                        n->key = n->next->key;
                        n->value = n->next->value;
                        n->weight = n->next->weight; 
                        n->next = n->next->next;
                        delete t; 
                    }
                } else {
                    //otherwise, remove from linked list
                    p->next = n->next;
                    delete n;
                } 
            }
        } 
    }

private:

    struct Node {
        K key;
        V value; 
        size_t weight;
        bool stored;
        Node *next;

        Node() : stored(false), next(0) {};
    };
 
    Node *nodes;
    size_t count;
    size_t size; 
    HashFunction hash;
    bool adapt_weights;
};

#endif 

