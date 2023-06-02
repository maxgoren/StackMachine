#ifndef SM_SYMBOL_TABLE_HPP
#define SM_SYMBOL_TABLE_HPP
#include <iostream>
#include <limits>
using namespace std;

template <class K, class V>
class smSymbolTable {
    private:
        struct node {
            K key;
            V value;
            bool color;
            node* left;
            node* right;
            node(K k, V v, bool c, node *l, node* r) {
                key = k; value = v; color = c; left = l; right = r;
            }
        };
        typedef node* link;
        link head, z;
        link x, p, g, gg;
        int N;
        V nullValue;
        bool color(link h) {
            return (h == nullptr) ? false:h->color;
        }
        bool nil(link h) {
            return h == z;
        }
        bool less(K a, K b) {
            return a < b;
        }
        void split(K key) {
            x->color = true; x->left->color = false; x->right->color = false;
            if (color(p)) {
                g->color = true;
                if (less(key, p->key) != less(key, g->key)) 
                    p = rotate(key, g);
                x = rotate(key, gg);
                x->color = false;
            }
        }
        link rotate(K v, link y) {
            link gc, c;
            c = (less(v,y->key)) ? y->left:y->right;
            if (less(v,c->key)) {
                gc = c->left; c->left = gc->right; gc->right = c;
            } else {
                gc = c->right; c->right = gc->left; gc->left = c;
            }
            if (less(v,y->key)) y->left = gc; 
            else y->right = gc;
            return gc;
        }
        link min(link h) {
            x = h;
            while (!nil(x->left)) x = x->left;
            return x;
        }
        link max(link h) {
            x = h;
            while (!nil(x->right)) x = x->right;
            return x;
        }
        link deleteMin(link h) {
            if (nil(h->left))
                return h->right;
            h->left = deleteMin(h->left);
            return h;
        }
        void visit(link h) {
            if (!nil(h)) {
                cout<<h->key<<" ";
                visit(h->left);
                visit(h->right);
            }
        }
    public:
        smSymbolTable() {
            z = new node(std::numeric_limits<K>::max(), 0, false, nullptr, nullptr);
            z->left = z; z->right = z;
            head = new node(std::numeric_limits<K>::min(), 0, false, z, z);
            N = 0;
        }
        void insert(K key, V value) {
            x = head; p = x; g = p;
            while (!nil(x)) {
                gg = g; g = p; p = x;
                x = less(key,x->key) ? x->left:x->right;
                if (color(x->left) && color(x->right))
                    split(key);
            }
            x = new node(key, value, true, z, z);
            if (less(key, p->key)) p->left = x; 
            else p->right = x;
            split(key);
            N++;
            head->right->color = false;
        }
        V& lookup(K key) {
            x = head->right;
            z->key = key;
            z->value = nullValue;
            while (x->key != key)
                x = less(key, x->key) ? x->left:x->right;
            return x->value;
        }
        void remove(K key) {
            x = head->right; p = head;
            while (!nil(x)) {
                if (key == x->key)
                    break;
                p = x;
                x = less(key, x->key) ? x->left:x->right;
            }
            link t = x;
            if (nil(x->left) && nil(x->right)) {
                x = z;
            } else if (nil(x->left)) {
                x = x->right;                
            } else if (nil(x->right)) {
                x = x->left;
            } else {
               x = min(t->right);
               x->right = deleteMin(t->right);
               x->left = t->left;
            }
            if (x->key < p->key) p->left = x; else p->right = x;
            N--;
            t->left = nullptr; t->right = nullptr;
            delete t;
        }
        void walk() {
            visit(head->right);
            cout<<endl;
        }
        int size() const {
            return N;
        }
        bool empty() const {
            return head->right == z;
        }
        const V& null() const { 
            return nullValue; 
        }
};

#endif