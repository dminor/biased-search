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

#ifndef SPLAYTREE_H_
#define SPLAYTREE_H_

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <limits>

/*
Splay Tree implementation

Based upon the description in:
Sleator, D. and Tarjan, R. (1985) Self-Adjusting Binary Search Trees,
Journal of the Association for Computing Machinery. Vol. 32, No. 3, July 1985, pp. 652-686.

and the Wikipedia article at:
http://en.wikipedia.org/wiki/Splay_tree
*/

template<class K, class V> class SplayTree {

public:

	SplayTree() : root(0)
	{
	}

	virtual ~SplayTree()
	{
	}

	void insert(const K &key, const V &value)
	{
		if (!root) {
			root = new Node(key, value, 0);
		} else {

			//insert in tree based on key
			Node *n = root;
			while (true) {

				if (key < n->key) {
					if (n->left) {
						n = n->left;
					} else {
						n->left = new Node(key, value, n);
						n = n->left;
						break;
					}
				} else if (n->key < key) {
					if (n->right) {
						n = n->right;
					} else {
						n->right = new Node(key, value, n);
						n = n->right;
						break;
					}
				} else {
					//already in splay tree
					break;
				}
			}

			splay(n);
		}
	}

	V *find(const K &key)
	{
	    V *result = 0;	

		Node *n = root;
		while (n && !result) {
			if (key < n->key) {
				n = n->left;
			} else if (n->key < key) {
				n = n->right;
			} else {
				result = &n->value;
				splay(n);
			}
		}

		return result;
	}

	void remove(const K &key)
	{
		Node *n = root;
		while (n) {
			if (key < n->key) {
				n = n->left;
			} else if (n->key < key) {
				n = n->right;
			} else {

                Node *p = n->parent;

				if (n->left == 0 && n->right == 0) {
                    //leaf node, splice out
					if (n->parent) {
						if (n->parent->left == n) n->parent->left = 0;
						else n->parent->right = 0;
					} else {
						root = 0;
					}

                    delete n;
                } else if (n->left == 0) { 
                    //just one right child
                    Node *t = n->right;
                    n->key = t->key;
                    n->value = t->value;
                    n->left = t->left;
                    if (n->left) n->left->parent = n;
                    n->right = t->right;
                    if (n->right) n->right->parent = n;
                    delete t; 
                } else if (n->right == 0) {
                    //just one left child
                    Node *t = n->left;
                    n->key = t->key;
                    n->value = t->value;
                    n->left = t->left;
                    if (n->left) n->left->parent = n;
                    n->right = t->right;
                    if (n->right) n->right->parent = n;
                    delete t; 
                } else {

                    //find in-order predecessor
                    Node *t = n->left;
                    while (t->right) t = t->right;

                    //copy in fields from t
                    n->key = t->key;
                    n->value = t->value;

                    //splice out t
                    if (t->left) {
                        if (t->parent->right == t) {
                            t->parent->right = t->left;
                            t->left->parent = t->parent;
                        } else {
                            t->parent->left = t->left;
                            t->left->parent = t->parent;
                        }
                    } else {
                        if (t->parent->right == t) t->parent->right = 0;
                        else t->parent->left = 0;
                    } 

                    delete t;
				}

                //splay parent to root
                if (p) splay(p); 
                break;
			}
		}
	}

private:

	struct Node {
		K key;
		V value;
		Node *parent;
		Node *left;
		Node *right;

		Node(const K &key, const V &value, Node *parent) : key(key), value(value), parent(parent)
		{
			left = right = 0;
		}
	};

	Node *root;

	void splay(Node *n)
	{
		//while n is not the root
		while (n->parent != 0) {

			//child of root, zig step
			if (n->parent->parent == 0) {
				if (n->parent->left == n) {
					rotate_right(n->parent);
				} else {
					rotate_left(n->parent);
				}
			} else {

				//pointers to parent and grandparent of n
				Node *p = n->parent;
				Node *gp = p->parent;

				//if n and its parent are both left or both right children, zig-zig step
				if (p->left == n && gp->left == p) {
					rotate_right(gp);
					rotate_right(p);
				} else if (p->right == n && gp->right == p) {
					rotate_left(gp);
					rotate_left(p);
				} else {
					
					//if n is a left child and p a right child, or n a right child and p a
					//left child, zig-zag step
					if (p->left == n && gp->right == p) {
						rotate_right(p);
						rotate_left(gp);
					} else if (p->right == n && gp->left == p) {
						rotate_left(p);
						rotate_right(gp);
					}
				}
			}
		}

		root = n;
	}

	// [T, T->left, T->left->right] <- [T->left, T->left->right, T]
	void rotate_right(Node *n)
	{
		Node *nl = n->left;

		nl->parent = n->parent;

		if (nl->parent != 0) {
			if (nl->parent->left == n) nl->parent->left = nl;
			else nl->parent->right = nl;
		} else {
			root = nl;
		}

		n->left = nl->right;
		if (n->left != 0) n->left->parent = n;

		n->parent = nl;
		nl->right = n;
	}

	// [T, T->right, T->right->left] <- [T->right, T->right->left, T]
	void rotate_left(Node *n)
	{
		Node *nr = n->right;

		nr->parent = n->parent;

		if (nr->parent != 0) {
			if (nr->parent->left == n) nr->parent->left = nr;
			else nr->parent->right = nr;
		} else {
			root = nr;
		}

		n->right = nr->left;
		if (n->right != 0) n->right->parent = n;

		n->parent = nr;
		nr->left = n;
	}
};

#endif


