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
#include <limits>

template<class K, class V> class BiasedSkiplist {

public:

	BiasedSkiplist(size_t max_height) : height(1), max_height(max_height)
	{
		head = new Node(max_height, max_height);
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
		size_t insert_height = random_height(weight);
		if (insert_height > max_height) insert_height = max_height;
        if (insert_height > height) height = insert_height;
		
		Node *n = new Node(insert_height, max_height);
		n->key = key;
		n->value = value;

		//allocate space on stack for nodes which need to be updated
		Node **update_nodes = (Node **)alloca(insert_height * sizeof(Node *));

		//search through skip list to find predecessor at each level
		for (size_t i = insert_height - 1; i >= 0 && i < insert_height; --i) {
			Node *t = head;
			while (t != 0 && t->next[i] != 0 && t->next[i]->key < key) t = t->next[i];
			update_nodes[i] = t;
		}

		//splice into skip list
		for (size_t i = 0; i < insert_height; ++i) {
			Node *t = update_nodes[i]->next[i];
			update_nodes[i]->next[i] = n;
			n->next[i] = t;
		}

	}

	bool find(const K &key, V &result)
	{
		bool found = false;

		for (size_t i = height - 1; i >= 0 && i < height; --i) {
			Node *t = head;
			while (t != 0 && t->next[i] != 0 && t->next[i]->key <= key) t = t->next[i];
			if (t->key == key) {
				result = t->value;
				found = true;
				break;
			}
		}

		return found;
	}

	void remove(const K &key)
	{

	}

	void reweight(const K &key, const V &value)
	{

	}

private:

	struct Node {
		K key;
		V value;
		size_t height;
		Node **next;

		Node(size_t height, size_t max_height) : height(height)
		{
			next = new Node *[max_height];

			for (size_t i = 0; i < max_height; ++i)
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
	size_t height;
	size_t max_height;

	size_t random_height(size_t weight)
	{
		if (weight == 0) weight = 1;
		size_t height = 1 + (int)(log((float)weight)/log(2.0f));
		while (rand() < RAND_MAX / 2) ++height;
		return height;
	}
};

#endif

