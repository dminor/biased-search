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

#ifndef BIASED_TREAP_H_
#define BIASED_TREAP_H_

#include <cstdlib>
#include <cmath>
#include <limits>

/*
Treap implementation for weighted / biased searches.

Based upon the description in:
Seidel, R., Aragon, C. R. (1996) Randomized Search Trees.  Algorithmica,
Vol. 16, Number 4/5, pp. 464--497.  
*/

template<class K, class V> class BiasedTreap {

public:

    BiasedTreap() : root(0)
    { 
    } 

    virtual ~BiasedTreap()
    { 
    }

    void insert(const K &key, const V &value, size_t weight)
    { 
        if (!root) {
            root = new Node(key, value, weight, 0); 
        } else {

            //insert in tree based on key
            Node *n = root; 
            while (true) {

                if (key < n->key) {
                    if (n->left) { 
                        n = n->left;
                    } else {
                        n->left = new Node(key, value, weight, n); 
                        n = n->left;
                        break;
                    }
                } else if (n->key < key) {
                    if (n->right) {
                        n = n->right; 
                    } else {
                        n->right = new Node(key, value, weight, n); 
                        n = n->right;
                        break;
                    }
                } else {
                    //already in treap
                    break;
                }
            }

            //re-balance based on priorities 
            while (n->parent && n->parent->priority < n->priority) { 
                if (n->parent->left == n) {
                    rotate_right(n->parent);
                } else {
                    rotate_left(n->parent); 
                } 
            }
        } 
    }

    bool find(const K &key, V &result)
    {
        bool found = false;

        Node *n = root; 
        while (n && !found) {
            if (key < n->key) {
                n = n->left;
            } else if (n->key < key) { 
                n = n->right; 
            } else {
                result = n->value;
                found = true;
            }
        }

        return found; 
    }

    void remove(const K &key)
    {

    }

private:

    struct Node {
        K key;
        V value; 
        float priority;
        Node *parent;
        Node *left;
        Node *right;

        Node(const K &key, const V &value, size_t weight, Node *parent) : key(key), value(value), parent(parent)
        {
            left = right = 0;
            if (weight == 0) weight = 1;
            priority = pow((float)rand()/(float)RAND_MAX, 1.0/(float)weight); 
        } 
    };

    Node *root;

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

    bool verify_treap(Node *n)
    { 
        if (!n) return true;

        if (n->left && n->priority < n->left->priority) return false;
        if (n->right && n->priority < n->right->priority) return false; 

        return verify_treap(n->right) && verify_treap(n->left);
    }

};

#endif 


