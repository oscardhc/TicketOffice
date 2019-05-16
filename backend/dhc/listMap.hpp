//
// Created by Haichen Dong on 2019-05-14.
//

#ifndef FLY_LISTMAP_HPP
#define FLY_LISTMAP_HPP

class ListMap {

public:
    struct Node {
        int index;
        int value;
        short isEdited;
        Node *nxt, *pre;
        Node() {
            nxt = pre = nullptr;
        }
        Node(int _k, int _v) {
            index = _k;
            value = _v;
            isEdited = 0;
            nxt = pre = nullptr;
        }
    };
    Node *begin;
    Node *end;

    ListMap() {
        begin = end = new Node();
    }

    void insertAtFront(Node* node) {
        begin->pre = node;
        begin->pre->nxt = begin;
        begin = begin->pre;
    }
    Node* find(int _key) {
        for (auto cur = begin; cur != end; cur = cur->nxt) {
            if (cur->index == _key) {
                return cur;
            }
        }
        return end;
    }
    void makeYoung(Node* node) {
//        std::cerr << "make young " << node << "     begin " << begin << std::endl;
        if (node == begin) return;
        if (node->pre != nullptr) {
            node->pre->nxt = node->nxt;
        }
        if (node->nxt != nullptr) {
            node->nxt->pre = node->pre;
        }
        node->pre = nullptr;
        node->nxt = begin;
        begin->pre = node;
        begin = node;
//        std::cerr << "after " << node << " " << node->nxt << std::endl;
    }
    Node* getLast() {
        return end->pre;
    }

};

#endif //FLY_LISTMAP_HPP
