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

#ifndef BIASED_SKIPLIST_H_
#define BIASED_SKIPLIST_H_

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <limits>

/* 
Skip list implementation for weighted / biased searches.

Based upon the description in: 
Bagchi, A., Buchsbaum, A., Goodrich, M. T., (2005) Biased Skip Lists.
Algorithmica, Vol 42, Number 1, pp. 31--48.  
*/

template<class K, class V> class BiasedSkiplist {

public:

	BiasedSkiplist(size_t max_level) : level(1), max_level(max_level)
	{
		head = new Node(max_level, max_level, 0);
	}

	virtual ~BiasedSkiplist()
	{
		Node *n = head;
		while (n != 0) {
			Node *t = n->next[0];
			delete n;
			n = t;
		}
	}

	void insert(const K &key, const V &value, size_t weight)
	{
        //pick level based upon weight 
		size_t insert_level = random_level(weight);
		if (insert_level > max_level) insert_level = max_level;
        if (insert_level > level) level = insert_level;

		//allocate space on stack for nodes which need to be updated
		Node **update = (Node **)alloca(insert_level * sizeof(Node *));

		//search through skip list to find predecessor at each level
        Node *t = head;
		for (size_t i = insert_level - 1; i >= 0 && i < insert_level; --i) {
			while (t->next[i] != 0 && t->next[i]->key < key) t = t->next[i];
			update[i] = t;
		}

        //we don't handle duplicate keys
        if (t->next[0] && t->next[0]->key == key) { 
            return;
        }

        //create new node 
		Node *n = new Node(insert_level, insert_level, weight); 
       
		n->key = key;
		n->value = value; 

		//splice into skip list
		for (size_t i = 0; i < insert_level; ++i) {
			n->next[i] = update[i]->next[i];
			update[i]->next[i] = n;
		} 
	}

	bool find(const K &key, V &result)
	{
		bool found = false;

        Node *t = head;
		for (size_t i = level - 1; i >= 0 && i < level; --i) {
			while (t->next[i] != 0 && t->next[i]->key < key) t = t->next[i];
			if (t->next[i] && t->next[i]->key == key) {
				result = t->next[i]->value;
				found = true;
				break;
			}
		}

		return found;
	}

	void remove(const K &key)
	{
		//search through skip list to find predecessor at each level
        //and update links to splice out removed key
        Node *t = head;
		for (size_t i = level - 1; i >= 0 && i < level; --i) {
			while (t->next[i] != 0 && t->next[i]->key < key) t = t->next[i]; 
            if (t->next[i]) { 
                if (i == 0) {
                    //if at lowest level, also free memory
                    Node *temp = t->next[i];
                    t->next[i] = t->next[i]->next[i]; 
                    delete temp;
                } else {
                    t->next[i] = t->next[i]->next[i]; 
                }
            }
		} 
	}

	void reweight(const K &key, size_t weight) 
	{
        Node *t = head;
		for (size_t i = level - 1; i >= 0 && i < level; --i) {
			while (t->next[i] != 0 && t->next[i]->key < key) t = t->next[i];
			if (t->next[i] && t->next[i]->key == key) { 
                Node *n = t->next[i];

                size_t new_level = random_level(weight);
                if (new_level > n->level) {

                    //search through skip list to find predecessor at each level and update
                    Node *p = head;
                    for (size_t j = new_level - 1; j > i && j < new_level; --j) {
                        while (p->next[j] != 0 && p->next[j]->key < n->key) p = p->next[j];
                        n->next[j] = p->next[j];
                        p->next[j] = n; 
                    } 

                } else if (new_level < t->next[i]->level) { 
                    //remove from levels
                    for (size_t j = n->level; j >= new_level && j < n->level; --j) {
                        t->next[i] = t->next[i]->next[i]; 
                    } 
                }

                n->level = new_level;
            } 
            break;
		}
	}

    void render_tree(const char *filename)
    {
        std::ofstream o(filename);
        if (o) { 
            for (size_t i = level - 1; i >= 0 && i < level; --i) {
                Node *t = head;
                o << i << " ";
                while (t->next[i] != 0) { 
                    t = t->next[i];
                    o << t->key << ", ";
                }
                o << ".\n";
            } 

            o.close();
        }
    }

private:

	struct Node {
		K key;
		V value;
		size_t level;
		Node **next;

		Node(size_t level, size_t max_level, size_t weight) : level(level)
		{
			next = new Node *[max_level];

			for (size_t i = 0; i < max_level; ++i)
			{
				next[i] = 0;
			}
		}

		virtual ~Node()
		{
			delete[] next;
		}
	};

	Node *head;
	size_t level;
	size_t max_level;

	size_t random_level(size_t weight)
	{
		if (weight == 0) weight = 1;

        size_t level = 1 + (int)(log((float)weight)/log(2.0f));

		while ((float)rand() / (float)RAND_MAX < 0.5) ++level;
		return level;
	}
};

#endif

