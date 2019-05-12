/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    
    template <
    class K,
    class T,
    class Compare = std::less<K>
    > class map {
    public:
        typedef pair<const K, T> value_type;
        
        class RBTree {
        public:
            class Node {
            public:
                value_type dat;
                int c; // 0:RED 1:BLACK
                Node *f;
                Node *s[2];
                Node *pre, *nxt;
                RBTree* tr;
                Node(const K& _key, const T& _val, Node* _nil, RBTree* _tr):dat(value_type(_key, _val)), tr(_tr) {
                    f = s[0] = s[1] = _nil;
                    pre = nxt = nullptr;
                    c = 0;
                }
                Node(const K& _key, const T& _val, Node* _f, Node* _s0, Node* _s1, Node* _pre, Node* _nxt, RBTree *_tr):dat(value_type(_key, _val)), tr(_tr) {
                    f = _f;
                    s[0] = _s0;
                    s[1] = _s1;
                    pre = _pre;
                    nxt = _nxt;
                    c = 0;
                }
                ~Node() {
                }
                
            };
            
            Compare cmp;
            Node* root;
            Node* nil;
            int sz;
            
            Node* uncle(Node* x) {
                return x->f->f->s[x->f->f->s[0] == x->f];
            }
            
            void rotate(Node *x) {
                Node* y = x->f, *z = y->f;
                bool p = y->s[0] == x;
                if (z != nil) z->s[z->s[1] == y] = x; else root = x;
                x->f = z, y->f = x;
                if (x->s[p] != nil) x->s[p]->f = y;
                y->s[p^1] = x->s[p];
                x->s[p] = y;
            }
            
            Node* insert(const K& _key, const T& _val) {
                sz++;
                Node *x = new Node(_key, _val, nil, this);
                Node *cur = root, *pre = nil;
                while (cur != nil) {
                    pre = cur;
                    cur = cur->s[cmp(cur->dat.first, x->dat.first)];
                }
                x->f = pre;
                if (pre != nil) pre->s[cmp(pre->dat.first, x->dat.first)] = x;
                else root = x;
                Node* nx = _nxt(x);
                Node* pr = _pre(x);
                if (pr != nullptr) pr->nxt = x;
                x->pre = pr;
                x->nxt = nx;
                if (nx != nullptr) nx->pre = x;
                rectifyI(x);
                return x;
            }
            
            void rectifyI(Node *x) {
                if (root == x) x->c = 1;
                else{
                    if (x->f->c == 1) return;
                    else {
                        Node* u = uncle(x);
                        if (u->c == 0) {
                            x->f->c = 1;
                            u->c = 1;
                            x->f->f->c = 0;
                            rectifyI(x->f->f);
                        } else {
                            bool p = x->f->f->s[1] == x->f;
                            if (x == x->f->s[p^1]) {
                                rotate(x);
                                x = x->s[p];
                            }
                            x->f->c = 1;
                            x->f->f->c = 0;
                            rotate(x->f);
                        }
                    }
                }
            }
            
            Node* find(const K& _key) const {
                for (Node* cur = root; cur != nil; ) {
                    if (cmp(cur->dat.first, _key) == false && cmp(_key, cur->dat.first) == false) {
                        return cur;
                    } else {
                        cur = cur->s[cmp(cur->dat.first, _key)];
                    }
                }
                return nil;
            }
            
            Node* _pre(Node* cur) const {
                if (cur->s[0] != nil) {
                    for (cur = cur->s[0]; cur != nil; cur = cur->s[1]) {
                        if (cur->s[1] == nil) return cur;
                    }
                } else {
                    Node* pr = nil;
                    while (cur != nil) {
                        pr = cur, cur = cur->f;
                        if (cur->s[1] == pr) return cur;
                    }
                }
                return nullptr;
            }
            
            Node* _nxt(Node* cur) const {
                if (cur->s[1] != nil) {
                    for (cur = cur->s[1]; cur != nil; cur = cur->s[0]) {
                        if (cur->s[0] == nil) return cur;
                    }
                } else {
                    Node* pr = nil;
                    while (cur != nil) {
                        pr = cur, cur = cur->f;
                        if (cur->s[0] == pr) return cur;
                    }
                }
                return nullptr;
            }
            
            void remove(const K& _key) {
                sz--;
                Node* fnd = find(_key), *cur;
                if (fnd->s[0] == nil || fnd->s[1] == nil) cur = fnd;
                else cur = _nxt(fnd);
                Node* x = cur->s[cur->s[1] != nil];
                
                Node* pr = _pre(cur);
                Node* nx = _nxt(cur);
                if (pr != nullptr) pr->nxt = nx;
                if (nx != nullptr) nx->pre = pr;

                x->f = cur->f;
                if (cur->f == nil) root = x;
                else cur->f->s[cur->f->s[1] == cur] = x;
                
                Node *tmpf = nil->f;
                int CURC = cur->c;
                
               if (cur != fnd) {

                    if (fnd->f != nil) fnd->f->s[fnd->f->s[1] == fnd] = cur;
                    else root = cur;

                    if (fnd->nxt != nullptr) fnd->nxt->pre = cur;
                    cur->nxt = fnd->nxt;
                    if (fnd->pre != nullptr) fnd->pre->nxt = cur;
                    cur->pre = fnd->pre;

                    cur->c = fnd->c;

                    fnd->s[0]->f = cur;
                    fnd->s[1]->f = cur;

                    cur->f = fnd->f;

                    cur->s[0] = fnd->s[0];
                    cur->s[1] = fnd->s[1];

                }
                
                if (tmpf == fnd) nil->f = cur;
                else nil->f = tmpf;
                if (CURC == 1) {
                    rectifyD(x);
                }
                delete fnd;
            }
            
            void rectifyD(Node *N) {
                while (1) {
                    if (N->c == 0 || root == N) break;
                    Node* S = N->f->s[N->f->s[0] == N];
                    if (S->c == 0) {
                        S->c = 1;
                        N->f->c = 0;
                        rotate(S);
                        S = N->f->s[N->f->s[0] == N];
                    }
                    if (S->s[0]->c == 1 && S->s[1]->c == 1) {
                        S->c = 0;
                        N = N->f;
                        continue;
                    }
                    bool p = N->f->s[1] == N;
                    if (S->c == 1 && S->s[p]->c == 0 && S->s[p^1]->c == 1) {
                        S->s[p]->c = 1;
                        S->c = 0;
                        rotate(S->s[p]);
                        S = N->f->s[N->f->s[0] == N];
                    }
                    p = N->f->s[1] == N;
                    S->c = N->f->c;
                    N->f->c = 1;
                    S->s[p^1]->c = 1;
                    rotate(S);
                    N = root;
                }
                N->c = 1;
            }
            
            int TMP;
            int chk(Node* x, int dep) {
                if (x == nil) {
                    if (TMP == -1) TMP = dep;
                    if (TMP != dep) {
                        return 0;
                    }
                    return 1;
                }
                if (x->c == 0) {
                    if (x->s[0]->c == 0 || x->s[1]->c == 0) {
                        return 0;
                    }
                }
                if (chk(x->s[0], dep + x->c) == 0 || chk(x->s[1], dep + x->c) == 0) return 0;
                return 1;
            }
            
            void destruct(Node *x) {
                for (int i = 0; i < 2; i++) {
                    if (x->s[i] != nil) destruct(x->s[i]);
                }
                delete x;
            }
            
            RBTree() {
                nil = (Node*)operator new(sizeof(Node));
                nil->s[0] = nil->s[1] = nil->f = nil;
                nil->c = 1;
                nil->tr = this;
                root = nil;
                sz = 0;
            }
            ~RBTree() {
                clear();
                operator delete(nil);
            }
            void clear() {
                if (root != nil) destruct(root);
                root = nil;
                sz = 0;
            }
            void copyFrom(Node *cur, Node *_nil) {
                insert(cur->dat.first, cur->dat.second);
                if (cur->s[0] != _nil) copyFrom(cur->s[0], _nil);
                if (cur->s[1] != _nil) copyFrom(cur->s[1], _nil);
            }
            Node* first() const {
                Node* cur = root;
                while (cur != nil && cur->s[0] != nil) {
                    cur = cur->s[0];
                }
                return cur == nil ? nullptr : cur;
            }
            Node* end() const {
                Node* cur = root;
                while (cur != nil && cur->s[1] != nil) {
                    cur = cur->s[1];
                }
                return cur == nil ? nullptr : cur;
            }
        };

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class iterator {
            friend void map::erase(iterator pos);
//        private:
        public:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            typename RBTree::Node *cur;
            bool end;
            
        public:
            iterator() {
                cur = nullptr;
                end = false;
            }
            iterator(typename RBTree::Node* ptr, bool a) {
                cur = ptr;
                end = a;
            }
            iterator(const iterator &other) {
                cur = other.cur;
                end = other.end;
            }
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp = iterator(*this);
                static typename RBTree::Node* hhh = nullptr;
//                printf("~~~~~~ %lld %lld %d\n", cur, cur->nxt, end);
                if (!end) hhh = cur->nxt;
                else throw index_out_of_bound();
                if (hhh == nullptr) end = 1;
                else cur = hhh;
//                printf("------ %lld %lld    %lld %lld\n", tmp.cur, cur, tmp->first, (*this)->first);
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                static typename RBTree::Node* hhh;
                if (!end) hhh = cur->nxt;
                else throw index_out_of_bound();
                if (hhh == nullptr) end = 1;
                else cur = hhh;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp = iterator(*this);
                if (end) {
                    if (cur->tr->sz == 0) throw index_out_of_bound();
                    end = 0;
                    cur = cur->tr->end();
                } else {
                    if (cur->pre == nullptr) throw index_out_of_bound();
                    else cur = cur->pre;
                }
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                
                if (end) {
                    if (cur->tr->sz == 0) throw index_out_of_bound();
                    end = 0;
                    cur = cur->tr->end();
                } else {
                    if (cur->pre == nullptr) throw index_out_of_bound();
                    else cur = cur->pre;
                }
                return *this;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {
                return cur->dat;
            }
            bool operator==(const iterator &rhs) const {
                return (cur == rhs.cur && end == rhs.end) || (end && end == rhs.end && cur->tr == rhs.cur->tr);
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return !(*this == rhs);}
            
            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return &(this->operator*());
            }
        };
        using const_iterator = iterator;
//        class const_iterator {
//            // it should has similar member method as iterator.
//            //  and it should be able to construct from an iterator.
//        private:
//            // data members.
//        public:
//            const_iterator() {
//                // TODO
//            }
//            const_iterator(const const_iterator &other) {
//                // TODO
//            }
//            const_iterator(const iterator &other) {
//                // TODO
//            }
//            // And other methods in iterator.
//            // And other methods in iterator.
//            // And other methods in iterator.
//        };
        
//    private:
        RBTree t;
        
    public:
        /**
         * TODO two constructors
         */
        map() {
        }
        map(const map &other) {
            t.copyFrom(other.t.root, other.t.nil);
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other) {
            if (this == &other) {
                return *this;
            }
            t.clear();
            t.copyFrom(other.t.root, other.t.nil);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() {}
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const K &key) {
            auto fnd = t.find(key);
//            printf("%d  %d\n", fnd->dat.first.val, key.val);
            if (fnd == t.nil) {
                throw index_out_of_bound();
            } else return fnd->dat.second;
        }
        const T & at(const K &key) const {
            auto fnd = t.find(key);
            if (fnd == t.nil) {
                throw index_out_of_bound();
            } else return fnd->dat.second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const K &key) {
            auto fnd = t.find(key);
            if (fnd == t.nil) {
                t.insert(key, T());
            }
            return t.find(key)->dat.second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const K &key) const {
            auto fnd = t.find(key);
            if (fnd == t.nil) {
                throw index_out_of_bound();
            } else return fnd->dat.second;
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin() const {if (!t.sz) return end(); return iterator(t.first(), 0);}
        iterator cbegin() const {if (!t.sz) return cend(); return iterator(t.first(), 0);}
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() const {return iterator(t.root, 1);}
        iterator cend() const {return iterator(t.root, 1);}
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return t.sz == 0;
        }
        /**
         * returns the number of elements.
         */
        size_t size() const {
            return t.sz;
        }
        /**
         * clears the contents
         */
        void clear() {
            t.clear();
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            typename RBTree::Node *fnd = t.find(value.first);
            if (fnd != t.nil) {
                return pair<iterator, bool>(iterator(fnd, 0), 0);
            }
            fnd = t.insert(value.first, value.second);
            return pair<iterator, bool>(iterator(fnd, 0), 1);
        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.cur == nullptr || pos.cur->tr != &t) throw invalid_iterator();
            if (pos == end()) throw invalid_iterator();
            t.remove(pos.cur->dat.first);
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const K &key) const {
            typename RBTree::Node *fnd = t.find(key);
//            if (fnd == t.nil) {
//                t.print(t.root);
//            }
            return fnd == t.nil ? 0 : 1;
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const K &key) const {
            typename RBTree::Node* re = t.find(key);
            if (re == t.nil) return end();
            else return iterator(re, 0);
        }
    };
    
}

#endif
