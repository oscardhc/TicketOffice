#include <iostream>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include "Database.hpp"

using namespace std;
enum NODE_TYPE{INTERNAL = 0, LEAF = 1}; // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT}; // 兄弟结点方向：左兄弟结点、右兄弟结点
typedef int KeyType; // 键类型
typedef int DataType; // 值类型
const int ORDER = 7; // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1; // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1; // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY; // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY; // 最大叶子结点键值个数

const size_t NODE_SIZE = sizeof(NODE_TYPE) + sizeof(int) + sizeof(KeyType) * MAXNUM_KEY + 2 * sizeof(size_t) + sizeof(DataType) * MAXNUM_KEY;


DATABASE DataBase;

template <class KeyType, class DataType>
class CNode{
protected:
    NODE_TYPE m_type;
    int m_KeyNum;
    KeyType m_keys[MAXNUM_KEY];
public:
    CNode(){
        m_type = LEAF;
        m_KeyNum = 0;
    }
    virtual ~CNode(){
        m_KeyNum = 0;
    }

    NODE_TYPE getType()const{
        return m_type;
    }

    void setType(NODE_TYPE type){
        m_type = type;
    }

    int getKeyNum() const{
        return m_KeyNum;
    }

    void setKeyNum(int i){
        m_KeyNum = i;
    }

    KeyType getKey(int i) const{
        return m_keys[i];
    }

    void setKey(int i, KeyType key){
        m_keys[i] = key;
    }

    int getKeyIndex(KeyType key) const{
        int l, r;
        l = 0;
        r = getKeyNum() - 1;

        if (key > getKey(r)) return r + 1;
        while (l < r){
            int m = (r + l) >> 1;
            if (getKey(m) < key) l = m + 1;
            else r = m;
        }
        return l;
    }

    virtual void removekey(int keyindex, int childindex) = 0;
    virtual void split(size_t fa_off, int childindex, size_t this_off) = 0;
    virtual void merge(size_t fa_off, size_t child_off, int keyindex) = 0;
    //virtual void clear() = 0;
    virtual void steal(size_t bro_off, size_t fa_off, int keyindex, SIBLING_DIRECTION d) = 0;
    virtual int getchildindex(KeyType key, int keyindex) const = 0;

};


class InterNode:public CNode<KeyType, DataType>{
private:
//    CNode<KeyType, DataType> * m_Childs[MAXNUM_CHILD];
    size_t m_Childs[MINNUM_CHILD];
public:
    InterNode(){
        m_type = INTERNAL;

    }

    virtual  ~InterNode(){

    }

    size_t getchild(int i) const{
        return m_Childs[i];
    }

    void setChild(int i, size_t child_off){
        m_Childs[i] = child_off;
    }

    void insert(int keyindex, int childindex, KeyType key, size_t offset){
        for (int i = getKeyNum(); i  >= keyindex; --i){
            setChild(i + 1, m_Childs[i]);
        }
        for (int i = getKeyNum(); i > keyindex; --i){
            setKey(i, m_keys[i - 1]);
        }
        setChild(childindex, offset);
        setKey(keyindex, key);
        m_KeyNum++;
    }

    virtual void removekey(int keyindex, int childindex){
        if (childindex == -1){
            for (int i = keyindex; i < getKeyNum()- 1; ++i){
                setKey(i, getKey(i + 1));
                setChild(i, m_Childs[i + 1]);
            }
            setChild(getKeyNum() - 1, m_Childs[getKeyNum()]);
            m_KeyNum--;
            return;
        }
        for (int i = keyindex; i < getKeyNum()- 1; ++i){
            setKey(i, getKey(i + 1));
            setChild(i + 1, m_Childs[i + 2]);
        }
        m_KeyNum--;
    }

    virtual void split(size_t fa_off, int childindex, size_t this_off){
        size_t new_off = DataBase.createElement();
        InterNode *newnode  = new InterNode();
        newnode->setKeyNum(MINNUM_KEY);
        int i;
        for (int i = 0; i < MINNUM_KEY; ++i){
            newnode->setKey(i, m_keys[i + MINNUM_KEY + 1]);
        }
        for (int i = 0; i < MINNUM_CHILD; ++i){
            newnode->setChild(i, m_Childs[i + MINNUM_CHILD]);
        }
        setKeyNum(MINNUM_KEY);
        InterNode* fa = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        fa->insert(childindex, childindex + 1, m_keys[MINNUM_KEY], new_off);
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)newnode, new_off, NODE_SIZE);


        delete newnode;
        delete fa;

    }

    virtual void merge(size_t fa_off, size_t child_off, int keyindex){
        InterNode * fa = new InterNode();
        InterNode * child = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElement((char*)child, child_off, NODE_SIZE);
        insert(MINNUM_KEY, MINNUM_KEY + 1, fa->getKey(keyindex),  (child)->getchild(0));
        int i;
        for (int i = 1; i <= child->getKeyNum(); ++i){
            insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, child->getKey(i - 1), (child)->getchild(i));
        }
        fa->removekey(keyindex, keyindex + 1);
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)child, child_off, NODE_SIZE);
        delete fa;
        delete child;
        //delete (InterNode*)child;/////
    }

//    virtual void clear(){
//        for (int i = 0; i <= m_KeyNum; ++i){
//            m_Childs[i]->clear();
//            delete m_Childs[i];
//            m_Childs[i] = nullptr;
//        }
//
//    }

    virtual void steal(size_t bro_off, size_t fa_off, int keyindex, SIBLING_DIRECTION d){
        InterNode * fa = new InterNode();
        InterNode * bro = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElement((char*)bro, bro_off, NODE_SIZE);
        if (d == LEFT){
            insert(0, 0, fa->getKey(keyindex), ((InterNode*)bro)->getchild(bro->getKeyNum()));
            fa->setKey(keyindex, bro->getKey(bro->getKeyNum() - 1));
            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum());
        }
        else {
            insert(getKeyNum(), getKeyNum() + 1, fa->getKey(keyindex), (bro)->getchild(0));
            fa->setKey(keyindex, bro->getKey(0));
            bro->removekey(0, -1);
            ///fa->setKey(keyindex, bro->getKey(0));
        }
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)bro, bro_off, NODE_SIZE);
        delete fa;
        delete bro;

    }

    virtual int getchildindex(KeyType key, int keyindex) const {
        if (keyindex == getKeyNum()) return keyindex;
        if (key == getKey(keyindex)){
            return keyindex + 1;
        }
        else return keyindex;

    }

};


class LeafNode: public CNode<KeyType, DataType>{
private:
    size_t m_left;
    size_t m_right;
    DataType m_Datas[MAXNUM_LEAF];

public:
    LeafNode(){
        m_type = LEAF;
    }

    virtual ~LeafNode(){

    }

    size_t getleft()const{
        return m_left;
    }

    void setleft(size_t left){
        m_left = left;
    }

    size_t getright()const{
        return m_right;
    }

    void setright(size_t right){
        m_right = right;
    }

    DataType getdata(int i) const {
        return m_Datas[i];
    }

    void setdata(int i, const DataType &data){
        m_Datas[i] = data;
    }

    void insert(KeyType key, const DataType &data){
        int i = 0;
        for (i = m_KeyNum; i >= 1 && m_keys[i - 1] > key; --i){
            setKey(i, m_keys[i - 1]);
            setdata(i, m_Datas[i -1]);
        }
        setdata(i, data);
        setKey(i, key);
        m_KeyNum++;

    }

    virtual void removekey(int keyindex, int childindex){
        for (int i = keyindex; i < getKeyNum() - 1; ++i){
            setKey(i, getKey(i + 1));
            setdata(i, getdata(i + 1));
        }
        m_KeyNum--;

    }

    virtual void split(size_t fa_off, int childindex, size_t this_off){
        size_t new_off = DataBase.createElement();
        LeafNode * newnode = new LeafNode();
        InterNode * fa = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        setKeyNum(MINNUM_LEAF);
        newnode->setKeyNum(MINNUM_LEAF + 1);
        newnode->setright(getright());
        setright(new_off);
        newnode->setleft(this_off);
        for (int i = 0; i <= MINNUM_LEAF; ++i){
            newnode->setKey(i, m_keys[i + MINNUM_LEAF]);
            newnode->setdata(i, m_Datas[i + MINNUM_LEAF]);
        }
        (fa)->insert(childindex, childindex + 1, m_keys[MINNUM_LEAF], new_off);
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)newnode, new_off, NODE_SIZE);
        delete fa;
        delete newnode;
    }

    virtual void merge(size_t fa_off, size_t child_off, int keyindex){
        InterNode * fa = new InterNode();
        InterNode * child = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElement((char*)child, child_off, NODE_SIZE);

        for (int i = 0; i < child->getKeyNum(); ++i){
            insert(child->getKey(i), ((LeafNode*)child)->getdata(i));
        }
        setright(((LeafNode*)child)->getright());
        fa->removekey(keyindex, keyindex + 1);
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)child, child_off, NODE_SIZE);
        delete fa;
        delete child;

    }

//    virtual void clear(){
//
//    }

    virtual void steal(size_t bro_off, size_t fa_off, int keyindex, SIBLING_DIRECTION d){
        InterNode * fa = new InterNode();
        InterNode * bro = new InterNode();
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElement((char*)bro, bro_off, NODE_SIZE);
        if (d == LEFT){
            insert(bro->getKey(bro->getKeyNum() - 1), ((LeafNode*)bro)->getdata(bro->getKeyNum() - 1));
            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum() - 1);
            fa->setKey(keyindex, getKey(0));
        }
        else {
            insert(bro->getKey(0), ((LeafNode*)bro)->getdata(0));
            bro->removekey(0, 0);
            fa->setKey(keyindex, bro->getKey(0));
        }
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElement((char*)bro, bro_off, NODE_SIZE);
        delete fa;
        delete bro;


    }

    virtual int getchildindex(KeyType key, int keyindex) const {
        return keyindex;
    }


};

class BPlusTree{
    struct ans{
        int keyindex;
        LeafNode * node;
    };
private:
    size_t root_off;
    size_t head_off;
    KeyType m_maxkey;
    int cnt;
    long long size;
    InterNode *alter_inter[4];
    LeafNode *alter_leaf[4];
public:
    BPlusTree() {
        for (int i = 0; i < 4; ++i){
            alter_inter[i] = new InterNode();
            alter_leaf[i] = new LeafNode();
        }
        root_off = 0;
        head_off = 0;
        m_maxkey = 0;
        size = 0;
    }

    ~BPlusTree(){
        for (int i = 0; i < 4; ++i){
            delete alter_leaf[i];
            delete alter_inter[i];
        }
        for (int i = 0)
        root_off = 0;
        m_maxkey = 0;
        size = 0;
        //clear();

    }

    bool insert(KeyType key, const DataType & data){
        if (search(key) != -1) return false;


        size++;
        CNode < KeyType, DataType > *m_root = alter_inter[0];
        if (root_off == 0){
            size_t root_off = DataBase.createElement();
            m_root = alter_leaf[0];
            DataBase.getElement((char*)m_root, root_off, NODE_SIZE);
            head_off = root_off;
            m_maxkey = key;
        }
        else {
            m_root = alter_inter[0];
            DataBase.getElement((char*)m_root, root_off, NODE_SIZE);
            if (m_root->getType() == LEAF){
                m_root = alter_leaf[0];
                DataBase.getElement((char*)m_root, root_off, NODE_SIZE);
            }
        }
        DataBase.setElement((char*)m_root, root_off, NODE_SIZE);

        if (m_root->getKeyNum() >= MAXNUM_KEY){
            size_t new_off = DataBase.createElement();
            InterNode *newnode = alter_inter[1];
            DataBase.getElement((char*)newnode, new_off, NODE_SIZE);
            newnode->setChild(0, root_off);
            m_root->split(new_off, 0, root_off);
            root_off = new_off;
            DataBase.setElement((char*)newnode, new_off, NODE_SIZE);
        }

        if (key > m_maxkey) m_maxkey = key;

        inter_insert(root_off, key, data);
        DataBase.setElement((char*)m_root, root_off, NODE_SIZE);

        return true;

    }

    long long getsize(){
        return size;
    }

    bool remove(KeyType key){
        if (search(key) == -1) return false;
        size--;

        CNode < KeyType , DataType > *m_root = alter_inter[0];
        DataBase.getElement((char*)m_root, root_off, NODE_SIZE);
        if (m_root->getKeyNum() == 1){
            if (m_root->getType() == LEAF){
                m_root->setKey(0, 0);
                m_root->setKeyNum(0);
                root_off = 0;
                head_off = 0;
                //clear();
                return true;
            }
            size_t ch1_off = ((InterNode*)m_root)->getchild(0);
            size_t ch2_off = ((InterNode*)m_root)->getchild(1);
            CNode < KeyType , DataType > *ch1 = alter_inter[1], *ch2 = alter_inter[2];
            DataBase.getElement((char*)ch1, ch1_off, NODE_SIZE);
            DataBase.getElement((char*)ch2, ch2_off, NODE_SIZE);
            if (ch1->getType() == LEAF){
                ch1 = alter_leaf[0];
                ch2 = alter_leaf[1];
                DataBase.getElement((char*)ch1, ch1_off, NODE_SIZE);
                DataBase.getElement((char*)ch2, ch2_off, NODE_SIZE);
            }

            if (ch1->getKeyNum() == MINNUM_KEY && ch2->getKeyNum() == MINNUM_KEY){
                ch1->merge(root_off, ch2_off, 0);
                root_off = ch1_off;
            }
            DataBase.setElement((char*)ch1, ch1_off, NODE_SIZE);

        }

        inter_remove(root_off, key);
        DataBase.setElement((char*)m_root, root_off, NODE_SIZE);
        return true;
    }

    bool update(KeyType key, DataType _new){
        if (search(key) == -1) return false;
        inter_update(root_off, key, _new);
    }

    DataType search(KeyType key){
        return inter_search(root_off, key);

    }

//    void clear(){
//        if (m_root != nullptr){
//            m_root->clear();
//            delete m_root;
//            m_root = nullptr;
//            m_head = nullptr;
//        }
//
//    }


//    void print(){
//        cnt = 1;
//        if (m_root == nullptr) return;
//        print(m_root, cnt);
//    }
//    void print(CNode < KeyType, DataType > *p, int cnt){
//        cout << "第" << cnt << "层" ;
//        printnode(p);
//        cout << endl;
//        if (p->getType() != LEAF){
//            for (int i = 0; i <= p->getKeyNum(); ++i){
//                print(((InterNode*)p)->getchild(i), cnt + 1);
//            }
//        }
//    }
//    void printnode(CNode < KeyType, DataType > *p){
//        for (int i = 0; i < p->getKeyNum(); ++i){
//            cout << p->getKey(i) << " ";
//        }
//
//    }
    KeyType getmax(){
        return m_maxkey;
    }

private:
    bool inter_update(size_t p_off, KeyType key, DataType _new){
        CNode < KeyType, DataType > *p = alter_inter[0];
        DataBase.getElement((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            p = alter_leaf[0];
            DataBase.getElement((char*)p, p_off, NODE_SIZE);
        }
        if (p == nullptr) return false;
        int keyindex = p->getKeyIndex(key);
        int childindex = p->getchildindex(key, keyindex);
        if (p->getType() == LEAF){
            if (key == p->getKey(keyindex)){
                ((LeafNode*)p)->setdata(keyindex, _new);
                DataBase.setElement((char*)p, p_off, NODE_SIZE);
                return true;
            }
            else return false;
        }

        return inter_update(((InterNode*)p)->getchild(childindex), key, _new);
    }



    void inter_insert(size_t fa_off, KeyType key, const DataType & data){
        CNode < KeyType , DataType > *fa = alter_inter[0];
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        if (fa->getType() == LEAF){
            fa = alter_leaf[0];
            DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        }
        if (fa->getType() == LEAF){
            ((LeafNode*)fa)->insert(key, data);
            return;
        }

        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);

        size_t child_off = ((InterNode*)fa)->getchild(childindex);
        CNode < KeyType, DataType > *childnode = alter_inter[0];
        DataBase.getElement((char*)childnode, child_off, NODE_SIZE);
        if (childnode->getType() == LEAF) {
            childnode = alter_leaf[0];
            DataBase.getElement((char*)childnode, child_off, NODE_SIZE);
        }

        if (childnode->getKeyNum() >= MAXNUM_KEY){
            childnode->split(fa_off, childindex, child_off);
            if (key >= fa->getKey(childindex)){
                child_off = ((InterNode*)fa)->getchild(childindex + 1);
            }
        }
        DataBase.setElement((char*)childnode, child_off, NODE_SIZE);
        inter_insert(child_off, key, data);
        DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
    }

    void inter_remove(size_t fa_off, KeyType key){
        CNode < KeyType , DataType > *fa = alter_inter[0];
        DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        if (fa->getType() == LEAF){
            fa = alter_leaf[0];
            DataBase.getElement((char*)fa, fa_off, NODE_SIZE);
        }
        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);
        if (fa->getType() == LEAF){
            if (key == m_maxkey && keyindex > 0){
                m_maxkey = fa->getKey(keyindex - 1);   //////
            }
            fa->removekey(keyindex, childindex);
            CNode < KeyType , DataType > *root = alter_inter[0];
            DataBase.getElement((char*)root, root_off, NODE_SIZE);
            if (childindex == 0 && root->getType() != LEAF && fa_off != head_off){
                changekey(root_off, key, fa->getKey(0));
            }
            DataBase.setElement((char*)fa, fa_off, NODE_SIZE);
            //DataBase.setElement((char*)root, root_off, NODE_SIZE);
        }
        else {
            size_t child_off = ((InterNode*)fa)->getchild(childindex);
            CNode < KeyType , DataType > *child = alter_inter[1];
            DataBase.getElement((char*)child, child_off, NODE_SIZE);
            if (child->getType() == LEAF){
                child = alter_leaf[1];
                DataBase.getElement((char*)child, child_off, NODE_SIZE);
            }

            if (child->getKeyNum() == MINNUM_KEY){
                size_t left_off = childindex > 0 ? ((InterNode*)fa)->getchild(childindex - 1) : 0;
                size_t right_off = childindex < fa->getKeyNum() ? ((InterNode*)fa)->getchild(childindex + 1): 0;
                CNode < KeyType , DataType > *left = alter_inter[2], *right = alter_inter[3];
                DataBase.getElement((char*)left, left_off, NODE_SIZE);
                DataBase.getElement((char*)right, right_off, NODE_SIZE);
                if (left->getType() == LEAF){
                    left = alter_leaf[2];
                    DataBase.getElement((char*)left, left_off, NODE_SIZE);
                    right = alter_leaf[3];
                    DataBase.getElement((char*)right, right_off, NODE_SIZE);
                }

                if (left && left->getKeyNum() > MINNUM_KEY) child->steal(left_off, fa_off, childindex - 1, LEFT);
                else if (right && right->getKeyNum() > MINNUM_KEY) child->steal(right_off, fa_off, childindex, RIGHT);
                else if (left){
                    left->merge(fa_off, child_off, childindex - 1);
                    child_off  = left_off;
                }
                else if (right){
                    child->merge(fa_off, right_off, childindex);
                }
                DataBase.setElement((char*)left, left_off, NODE_SIZE);
                DataBase.setElement((char*)right, right_off, NODE_SIZE);
                DataBase.setElement((char*)child, child_off, NODE_SIZE);
            }
            inter_remove(child_off, key);
            DataBase.setElement((char*)child, child_off, NODE_SIZE);
        }
    }

    DataType inter_search(size_t p_off, KeyType key) const{
        CNode < KeyType, DataType > *p = alter_inter[0];
        DataBase.getElement((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            p = alter_leaf[0];
            DataBase.getElement((char*)p, p_off, NODE_SIZE);
        }
        if (p == nullptr) return -1;
        int keyindex = p->getKeyIndex(key);
        int childindex = p->getchildindex(key, keyindex);
        if (p->getType() == LEAF){
            if (key == p->getKey(keyindex)){
                return ((LeafNode*)p)->getdata(keyindex);
            }
            else return -1;
        }

        return inter_search(((InterNode*)p)->getchild(childindex), key);

    }

    void changekey(size_t p_off, KeyType _old, KeyType _new){
        CNode < KeyType, DataType > *p = alter_inter[0];
        DataBase.getElement((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            p = alter_leaf[0];
            DataBase.getElement((char*)p, p_off, NODE_SIZE);
        }
        if (p != nullptr && p->getType() != LEAF){
            int keyindex = p->getKeyIndex(_old);
            if (_old == p->getKey(keyindex)){
                p->setKey(keyindex, _new);
            }
            else {
                changekey(((InterNode*)p)->getchild(keyindex), _old, _new);
            }
        }
    }






};
