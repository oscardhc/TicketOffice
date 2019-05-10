#include <iostream>
#include <map>
#include <stdlib.h>
#include <algorithm>

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

////.h
//// 当前最大key值->getmax();
//// 当前数据数量->getsize();
//// 插入->insert(key, data);
//// 删除->remove(key);
//// 更新->update(key, data);
//// 查找->search(key);


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
    virtual void split(CNode *fa, int childindex) = 0;
    virtual void merge(CNode *fa, CNode * child, int keyindex) = 0;
    virtual void clear() = 0;
    virtual void steal(CNode * bro, CNode * fa, int keyindex, SIBLING_DIRECTION d) = 0;
    virtual int getchildindex(KeyType key, int keyindex) const = 0;
    
};


class InterNode:public CNode{
private:
    CNode * m_Childs[MAXNUM_CHILD];
public:
    InterNode(){
        m_type = INTERNAL;
        
    }
    
    virtual  ~InterNode(){
        
    }
    
    CNode * getchild(int i) const{
        return m_Childs[i];
    }
    
    void setChild(int i, CNode *child){
        m_Childs[i] = child;
    }
    
    void insert(int keyindex, int childindex, KeyType key, CNode *childnode){
        for (int i = getKeyNum(); i  >= keyindex; --i){
            setChild(i + 1, m_Childs[i]);
        }
        for (int i = getKeyNum(); i > keyindex; --i){
            setKey(i, m_keys[i - 1]);
        }
        setChild(childindex, childnode);
        setKey(keyindex, key);
        m_KeyNum++;
    }
    
    virtual void removekey(int keyindex, int childindex){
        if (childindex == -1){
            for (int i = keyindex; i < getKeyNum()- 1; ++i){
                setKey(i, getKey(i + 1));
                setChild(i, getchild(i + 1));
            }
            setChild(getKeyNum() - 1, getchild(getKeyNum()));
            m_KeyNum--;
            return;
        }
        for (int i = keyindex; i < getKeyNum()- 1; ++i){
            setKey(i, getKey(i + 1));
            setChild(i + 1, getchild(i + 2));
        }
        m_KeyNum--;
    }
    
    virtual void split(CNode *fa, int childindex){
        InterNode *newnode = new InterNode();
        newnode->setKeyNum(MINNUM_KEY);
        int i;
        for (int i = 0; i < MINNUM_KEY; ++i){
            newnode->setKey(i, m_keys[i + MINNUM_KEY + 1]);
        }
        for (int i = 0; i < MINNUM_CHILD; ++i){
            newnode->setChild(i, m_Childs[i + MINNUM_CHILD]);
        }
        setKeyNum(MINNUM_KEY);
        ((InterNode*)fa)->insert(childindex, childindex + 1, m_keys[MINNUM_KEY], newnode);
        
    }
    
    virtual void merge(CNode * fa, CNode * child, int keyindex){
        insert(MINNUM_KEY, MINNUM_KEY + 1, fa->getKey(keyindex),  ((InterNode*)child)->getchild(0));
        int i;
        for (int i = 1; i <= child->getKeyNum(); ++i){
            insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, child->getKey(i - 1), ((InterNode*)child)->getchild(i));
        }
        fa->removekey(keyindex, keyindex + 1);
        delete (InterNode*)child;/////
    }
    
    virtual void clear(){
        for (int i = 0; i <= m_KeyNum; ++i){
            m_Childs[i]->clear();
            delete m_Childs[i];
            m_Childs[i] = nullptr;
        }
        
    }
    
    virtual void steal(CNode * bro, CNode * fa, int keyindex, SIBLING_DIRECTION d){
        if (d == LEFT){
            insert(0, 0, fa->getKey(keyindex), ((InterNode*)bro)->getchild(bro->getKeyNum()));
            fa->setKey(keyindex, bro->getKey(bro->getKeyNum() - 1));
            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum());
        }
        else {
            insert(getKeyNum(), getKeyNum() + 1, fa->getKey(keyindex), ((InterNode*)bro)->getchild(0));
            fa->setKey(keyindex, bro->getKey(0));
            bro->removekey(0, -1);
            ///fa->setKey(keyindex, bro->getKey(0));
        }
        
    }
    
    virtual int getchildindex(KeyType key, int keyindex) const {
        if (keyindex == getKeyNum()) return keyindex;
        if (key == getKey(keyindex)){
            return keyindex + 1;
        }
        else return keyindex;
        
    }
    
};


class LeafNode: public CNode{
private:
    LeafNode * m_left;
    LeafNode * m_right;
    DataType m_Datas[MAXNUM_LEAF];
    
public:
    LeafNode(){
        m_type = LEAF;
        m_left = nullptr;
        m_right = nullptr;
    }
    
    virtual ~LeafNode(){
        
    }
    
    LeafNode *getleft()const{
        return m_left;
    }
    
    void setleft(LeafNode *node){
        m_left = node;
    }
    
    LeafNode *getright()const{
        return m_right;
    }
    
    void setright(LeafNode *node){
        m_right = node;
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
    
    virtual void split(CNode *fa, int childindex){
        LeafNode * newnode = new LeafNode();
        setKeyNum(MINNUM_LEAF);
        newnode->setKeyNum(MINNUM_LEAF + 1);
        newnode->setright(getright());
        setright(newnode);
        newnode->setleft(this);
        for (int i = 0; i <= MINNUM_LEAF; ++i){
            newnode->setKey(i, m_keys[i + MINNUM_LEAF]);
            newnode->setdata(i, m_Datas[i + MINNUM_LEAF]);
        }
        ((InterNode*)fa)->insert(childindex, childindex + 1, m_keys[MINNUM_LEAF], newnode);
        
    }
    
    virtual void merge(CNode * fa, CNode * child, int keyindex){
        for (int i = 0; i < child->getKeyNum(); ++i){
            insert(child->getKey(i), ((LeafNode*)child)->getdata(i));
        }
        setright(((LeafNode*)child)->getright());
        fa->removekey(keyindex, keyindex + 1);
        
    }
    
    virtual void clear(){
        
    }
    
    virtual void steal(CNode * bro, CNode * fa, int keyindex, SIBLING_DIRECTION d){
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
    CNode * m_root;
    LeafNode * m_head;
    KeyType m_maxkey;
    int cnt;
    long long size;
public:
    BPlusTree() {
        m_root = nullptr;
        m_head = nullptr;
        m_maxkey = 0;
        size = 0;
    }
    
    ~BPlusTree(){
        clear();
    }
    
    bool insert(KeyType key, const DataType & data){
        if (search(key) != -1) return false;
        
        size++;
        if (m_root == nullptr){
            m_root = new LeafNode();
            m_head = (LeafNode*)m_root;
            m_maxkey = key;
        }
        
        if (m_root->getKeyNum() >= MAXNUM_KEY){
            InterNode* newnode = new InterNode();
            newnode->setChild(0, m_root);
            m_root->split(newnode, 0);
            m_root = newnode;
        }
        
        if (key > m_maxkey) m_maxkey = key;
        
        inter_insert(m_root, key, data);
        
        return true;
        
    }
    
    long long getsize(){
        return size;
    }
    
    bool remove(KeyType key){
        if (search(key) == -1) return false;
        size--;
        
        if (m_root->getKeyNum() == 1){
            if (m_root->getType() == LEAF){
                clear();
                return true;
            }
            CNode *ch1 = ((InterNode*)m_root)->getchild(0);
            CNode *ch2 = ((InterNode*)m_root)->getchild(1);
            if (ch1->getKeyNum() == MINNUM_KEY && ch2->getKeyNum() == MINNUM_KEY){
                ch1->merge(m_root, ch2, 0);
                delete m_root;
                m_root = ch1;
            }
        }
        
        inter_remove(m_root, key);
        return true;
    }
    
    bool update(KeyType key, DataType _new){
        if (search(key) == -1) return false;
        inter_update(m_root, key, _new);
    }
    
    DataType search(KeyType key){
        return inter_search(m_root, key);
        
    }
    
    void clear(){
        if (m_root != nullptr){
            m_root->clear();
            delete m_root;
            m_root = nullptr;
            m_head = nullptr;
        }
        
    }
    
    
    void print(){
        cnt = 1;
        if (m_root == nullptr) return;
        print(m_root, cnt);
    }
    void print(CNode *p, int cnt){
        cout << "第" << cnt << "层" ;
        printnode(p);
        cout << endl;
        if (p->getType() != LEAF){
            for (int i = 0; i <= p->getKeyNum(); ++i){
                print(((InterNode*)p)->getchild(i), cnt + 1);
            }
        }
    }
    void printnode(CNode *p){
        for (int i = 0; i < p->getKeyNum(); ++i){
            cout << p->getKey(i) << " ";
        }
        
    }
    KeyType getmax(){
        return m_maxkey;
    }
    
private:
    bool inter_update(CNode *p, KeyType key, DataType _new){
        if (p == nullptr) return false;
        int keyindex = p->getKeyIndex(key);
        int childindex = p->getchildindex(key, keyindex);
        if (p->getType() == LEAF){
            if (key == p->getKey(keyindex)){
                ((LeafNode*)p)->setdata(keyindex, _new);
                return true;
            }
            else return false;
        }
        
        return inter_update(((InterNode*)p)->getchild(childindex), key, _new);
    }
    
    
    
    void inter_insert(CNode *fa, KeyType key, const DataType & data){
        if (fa->getType() == LEAF){
            ((LeafNode*)fa)->insert(key, data);
            return;
        }
        
        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);
        
        CNode *childnode = ((InterNode*)fa)->getchild(childindex);
        
        if (childnode->getKeyNum() >= MAXNUM_KEY){
            childnode->split(fa, childindex);
            if (key >= fa->getKey(childindex)){
                childnode = ((InterNode*)fa)->getchild(childindex + 1);
            }
        }
        inter_insert(childnode, key, data);
        
    }
    
    void inter_remove(CNode *fa, KeyType key){
        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);
        if (fa->getType() == LEAF){
            if (key == m_maxkey && keyindex > 0){
                m_maxkey = fa->getKey(keyindex - 1);   //////
            }
            fa->removekey(keyindex, childindex);
            if (childindex == 0 && m_root->getType() != LEAF && fa != m_head){
                changekey(m_root, key, fa->getKey(0));
            }
        }
        else {
            CNode *child = ((InterNode*)fa)->getchild(childindex);
            int u = child->getKeyNum();
            if (child->getKeyNum() == MINNUM_KEY){
                CNode *left = childindex > 0 ? ((InterNode*)fa)->getchild(childindex - 1) : nullptr;
                CNode *right = childindex < fa->getKeyNum() ? ((InterNode*)fa)->getchild(childindex + 1): nullptr;
                if (left && left->getKeyNum() > MINNUM_KEY) child->steal(left, fa, childindex - 1, LEFT);
                else if (right && right->getKeyNum() > MINNUM_KEY) child->steal(right, fa, childindex, RIGHT);
                else if (left){
                    left->merge(fa, child, childindex - 1);
                    child  = left;
                }
                else if (right){
                    child->merge(fa, right, childindex);
                }
            }
            inter_remove(child, key);
        }
    }
    
    DataType inter_search(CNode *p, KeyType key) const{
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
    
    void changekey(CNode * p, KeyType _old, KeyType _new){
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
