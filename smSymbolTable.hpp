#ifndef SMSYMBOLTABLE_HPP
#define SMSYMBOLTABLE_HPP
#include <iostream>
using namespace std;

template <class K, class V>
class SymbolTable {
    private:
        struct node {
            K key;
            V value;
            node* left;
            node* right;
            node(K k, V val) {
                key = k; value = val;
                left = right = nullptr;
            }
            ~node() {
                if (left != nullptr)
                    delete left;
                if (right != nullptr)
                    delete right;
            }
            node(const node& o) {
                left = o.left;
                right = o.right;
                key = o.key;
                value = o.value;
            }
        };
        typedef node* link;
        link root;
        int n;
        link put(link h, K key, V value) {
            if (h == nullptr) {
                n++;
                return new node(key, value);
            }
            if (key == h->key) {
                h->value = value;
                return h;
            }
            if (key < h->key) h->left = put(h->left, key, value);
            else h->right = put(h->right, key, value);
            return h;
        }
        V get(link h, K key) {
            if (h == nullptr) {
                return (V)21;
            } else if (key == h->key) {
                return h->value;
            } else if (key < h->key) {
                return get(h->left, key);
            } else {
                return get(h->right, key);
            }
        }
        link delMin(link h) {
            if (h->left == nullptr) {
                return h->right;
            }
            h->left = delMin(h->left);
            return h;
        }

        link min(link h) {
            link x = h;
            while (x->left != nullptr) x = x->left;
            return x;
        }
        link del(link h, K key) {
            link p, t = h;
            if (h == nullptr) 
                return nullptr;
            if (key < h->key) 
                h->left = del(h->left, key);
            else if (key > h->key) 
                h->right = del(h->right, key);
            else  {
                if (h->right == nullptr) 
                    return h->left;

                t = h;
                h = min(t->right);
                h->right = delMin(t->right);
                h->left = t->left;
            }
            return h;
        }
        void preorder(link h) {
            if (h != nullptr) {
                cout<<h->key<<" ";
                preorder(h->left);
                preorder(h->right);
            }
        }
    public:
        SymbolTable() {
            n = 0;
            root = nullptr;
        }
        ~SymbolTable() {
            if (root != nullptr)
                delete root;
        }
        SymbolTable(const SymbolTable& o) {
            root = o.root;
            n = o.n;
        }
        void insert(K key, V value) {
            root = put(root, key, value);
            n++;
        }
        V lookup(K key) {
            return get(root, key);
        }
        void remove(K key) {
            root = del(root, key);
        }
        int size() {
            return n;
        }
        bool empty() {
            return root == nullptr;
        }
        void walk() {
            preorder(root);
            cout<<endl;
        }
};

#endif