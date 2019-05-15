#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;
typedef int KeyType;
typedef int DataType;

//template <KeyType, DataType>
class SplayTree{
private:
    struct Node{
        KeyType key;
        DataType data;
        Node *left;
        Node *right;

        Node():left(nullptr), right(nullptr){}

        Node(KeyType _key, DataType _data, Node *l, Node *r):key(_key), data(_data), left(l), right(r){}

        ~Node() = default;
    };

    Node *m_root;
    KeyType maxkey;
    KeyType minkey;
    bool tag_max, tag_min;

    DataType inter_search(Node * t, KeyType key) {

        if (t == nullptr) return -1;
        if (key < t->key) {
            return inter_search(t->left, key);
        }
        else if (key > t->key){
            return inter_search(t->right, key);
        }
        else {
            m_root = splay(m_root, key);
            return t->data;
        }


    }

    Node * splay(Node * t, KeyType key){
        Node n, *ll, *rr, *c;
        if (t == nullptr)
            return t;
        n.left = n.right = nullptr;
        ll = rr = &n;

        while(true){
            if (key == t->key) break;
            if (key < t->key){
                if (t->left == nullptr) break;
                if (key < t->left->key){
                    c = t->left;
                    t->left = c->right;
                    c->right = t;
                    t = c;
                    if (t->left == nullptr) break;
                }
                rr->left = t;
                rr = t;
                t = t->left;
            }
            else if (key > t->key){
                if (t->right == nullptr) break;
                if (key > t->right->key){
                    c = t->right;
                    t->right = c->left;
                    c->left = t;
                    t = c;
                    if (t->right == nullptr) break;
                }
                ll->right = t;
                ll = t;
                t = t->right;
            }
        }

        ll->right = t->left;
        rr->left = t->right;
        t->left = n.right;
        t->right = n.left;

        return t;
    }

    bool inter_insert(Node * &t, KeyType key, DataType data){

        Node *newnode = new Node(key, data, nullptr, nullptr);

        if (t == nullptr) {
            m_root = newnode;
            return true;
        }
        t = splay(t, key);
        if(t->key > key) {
            newnode->left = m_root->left;
            newnode->right = m_root;
            m_root->left = nullptr;
            m_root = newnode;
        }
        else if(m_root->key < key) {
            newnode->right = m_root->right;
            m_root->right = nullptr;
            newnode->left = m_root;
            m_root = newnode;
        }
        else {
            delete newnode;
            return false;
        }

        return true;
//        if (t == nullptr){
//            t = new Node(key, data, nullptr, nullptr);
//            return true;
//        }
//        else if (key < t->key){
//            return inter_insert(t->left, key, data);
//        }
//        else if (key > t->key){
//            return inter_insert(t->right, key, data);
//        }
//        else {
//            return false;
//        }
    }

    KeyType inter_getmax(Node *t){
        if (t == nullptr) return 0;
        tag_max = 0;
        while (t->right){
            t = t->right;
        }
        maxkey = t->key;
        return maxkey;
    }

    KeyType inter_getmin(Node *t){
        if (t == nullptr) return 0;
        tag_min = 0;
        while (t->left){
            t = t->left;
        }
        minkey = t->key;
        return minkey;

    }

    void inter_remove(Node * t, KeyType key){
        t = splay(t, key);
        Node *newroot;
        if (t->left){
            newroot = splay(t->left, key);
            newroot->right = t->right;
        }
        else newroot = t->right;

        delete t;
        m_root = newroot;
    }
    void clear(Node *t){
        if (t == nullptr) return;
        clear(t->left);
        clear(t->right);
        delete t;
    }


    void print(Node *t, KeyType key, int direction)
    {
        if(t != NULL)
        {
            if (direction==0)
                cout << t->key << " is root" << endl;
            else
                cout<< t->key << " is " << key << "'s "  << (direction == 1?"right child" : "left child") << endl;

            print(t->left, t->key, -1);
            print(t->right,t->key,  1);
        }
    }


public:

    SplayTree(){
        m_root = nullptr;
        tag_max = 0;
        tag_min = 0;
        maxkey = 0;
    }

    ~SplayTree(){
        clear(m_root);
    }

    DataType search(KeyType key){
        return inter_search(m_root, key);
    }

    KeyType getmax(){
        if (!tag_max) return maxkey;
        else return inter_getmax(m_root);
    }

    KeyType getmin(){
        if (!tag_min) return minkey;
        else return inter_getmin(m_root);
    }


    bool insert(KeyType key, DataType data){
        if (m_root == nullptr){
            minkey = key;
            maxkey = key;
        }
        if (key > maxkey) maxkey = key;
        if (key < minkey) minkey = key;
        inter_insert(m_root, key, data);
    }

    bool remove(KeyType key){
        if (search(key) == -1) return false;
        if (key == maxkey) tag_max = 1;
        if (key == minkey) tag_min = 1;

        inter_remove(m_root, key);
        return true;
    }


    void print()
    {
        if (m_root != NULL)
            print(m_root, m_root->key, 0);
    }

};