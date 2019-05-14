#include <iostream>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include "../dhc/IOManager.hpp"
//
using namespace std;
enum NODE_TYPE{INTERNAL = 0, LEAF = 1}; // 结点类型：内结点、叶子结点
enum SIBLING_DIRECTION{LEFT, RIGHT}; // 兄弟结点方向：左兄弟结点、右兄弟结点
typedef int KeyType; // 键类型
typedef int DataType; // 值类型
const int ORDER = 40; // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1; // 最小键值个数
const int MAXNUM_KEY = 2*ORDER-1; // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY; // 最小叶子结点键值个数
const int MAXNUM_LEAF = MAXNUM_KEY; // 最大叶子结点键值个数

const int NODE_SIZE = ORDER * 16 + 24;
const int ALTER_SIZE = 30;

sjtu::IOManager DataBase("./out.txt");

template <class KeyType, class DataType>
class CNode{
protected:
public:
    NODE_TYPE m_type;
    int m_KeyNum;
    KeyType m_keys[MAXNUM_KEY];
    //NODE_TYPE m_type;
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
    virtual void split(int fa_off, int childindex, int this_off) = 0;
    virtual void merge(int fa_off, int child_off, int keyindex, int this_off) = 0;
    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off) = 0;
    virtual int getchildindex(KeyType key, int keyindex) const = 0;

};


class InterNode:public CNode<KeyType, DataType>{
private:
public:
    int m_Childs[MAXNUM_CHILD];
    int m_left;
    int m_right;
    //NODE_TYPE m_type;
    //InterNode * alter_inter[3];
public:
    InterNode(){
        m_type = INTERNAL;
    }

    virtual  ~InterNode(){

    }

    int getchild(int i) const{
        return m_Childs[i];
    }

    void setChild(int i, int child_off){
        m_Childs[i] = child_off;
    }

    void insert(int keyindex, int childindex, KeyType key, int offset){
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

    virtual void split(int fa_off, int childindex, int this_off){
        //cout << "nonon" << endl;
        int new_off = DataBase.createElementVirt(NODE_SIZE);
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
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        fa->insert(childindex, childindex + 1, m_keys[MINNUM_KEY], new_off);
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
        //cout << newnode->getType();

        delete newnode;
        delete fa;

    }

    virtual void merge(int fa_off, int child_off, int keyindex, int this_off){
        InterNode * fa = new InterNode();
        InterNode * child = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
        insert(MINNUM_KEY, MINNUM_KEY + 1, fa->getKey(keyindex),  (child)->getchild(0));
        int i;
        for (int i = 1; i <= child->getKeyNum(); ++i){
            insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, child->getKey(i - 1), (child)->getchild(i));
        }
        fa->removekey(keyindex, keyindex + 1);
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)child, child_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
        delete fa;
        delete child;
    }

    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off){
        InterNode * fa = new InterNode();
        InterNode * bro = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElementVirt((char*)bro, bro_off, NODE_SIZE);
        if (d == LEFT){
            insert(0, 0, fa->getKey(keyindex), ((InterNode*)bro)->getchild(bro->getKeyNum()));
            fa->setKey(keyindex, bro->getKey(bro->getKeyNum() - 1));
            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum());
        }
        else {
            insert(getKeyNum(), getKeyNum() + 1, fa->getKey(keyindex), (bro)->getchild(0));
            fa->setKey(keyindex, bro->getKey(0));
            bro->removekey(0, -1);
        }
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)bro, bro_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
        delete fa;
        delete bro;

    }

    virtual int getchildindex(KeyType key, int keyindex) const {
        if (keyindex >= getKeyNum()) return keyindex;
        if (key == getKey(keyindex)){
            return keyindex + 1;
        }
        else return keyindex;

    }

};


class LeafNode: public CNode<KeyType, DataType>{
private:
    int m_left;
    int m_right;
    DataType m_Datas[MAXNUM_CHILD];

public:
    LeafNode(){
        m_type = LEAF;
    }

    virtual ~LeafNode(){

    }

    int getleft()const{
        return m_left;
    }

    void setleft(int left){
        m_left = left;
    }

    int getright()const{
        return m_right;
    }

    void setright(int right){
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

    virtual void split(int fa_off, int childindex, int this_off){
        //cout << "nonon";
        int new_off = DataBase.createElementVirt(NODE_SIZE);
        LeafNode * newnode = new LeafNode();
        InterNode * fa = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
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
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
        //cout << fa->getType() << endl;
        delete newnode;
    }

    virtual void merge(int fa_off, int child_off, int keyindex, int this_off){
        InterNode * fa = new InterNode();
        InterNode * child = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);

        for (int i = 0; i < child->getKeyNum(); ++i){
            insert(child->getKey(i), ((LeafNode*)child)->getdata(i));
        }
        setright(((LeafNode*)child)->getright());
        fa->removekey(keyindex, keyindex + 1);
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)child, child_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
        delete fa;
        delete child;

    }


    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off){
        InterNode * fa = new InterNode();
        InterNode * bro = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.getElementVirt((char*)bro, bro_off, NODE_SIZE);
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
        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        DataBase.setElementVirt((char*)bro, bro_off, NODE_SIZE);
        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
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
    int meta_off;
    int root_off;
    int head_off;
    KeyType m_maxkey;
    int cnt;
    int size;
    InterNode *inter_buf[ALTER_SIZE];
    LeafNode  *leaf_buf[ALTER_SIZE];
    int tot1, tot2;
public:
    BPlusTree() {
    }
    BPlusTree(int _meta_off, int is_init) {
        meta_off = _meta_off;
        if (is_init) {
            tot1 = 0;
            tot2 = 0;
            for (int i = 0; i < ALTER_SIZE; ++i){
                inter_buf[i] = new InterNode();
                leaf_buf[i] = new LeafNode();
                inter_buf[i]->setType(INTERNAL);
            }
            root_off = -1;
            DataBase.setElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
            head_off = -1;
            DataBase.setElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
            m_maxkey = 0;
            DataBase.setElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
            size = 0;
            DataBase.setElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
        } else {
            tot1 = 0;
            tot2 = 0;
            for (int i = 0; i < ALTER_SIZE; ++i){
                inter_buf[i] = new InterNode();
                leaf_buf[i] = new LeafNode();
                inter_buf[i]->setType(INTERNAL);
            }
            DataBase.getElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
            DataBase.getElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
            DataBase.getElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
            DataBase.getElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
        }
    }

    ~BPlusTree(){
        DataBase.setElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
        DataBase.setElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
        DataBase.setElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
        DataBase.setElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
    }

    int getRootOff() {
        return root_off;
    }
    bool insert(KeyType key, const DataType & data){
        if (search(key) != -1) return false;
        size++;
        CNode < KeyType, DataType > *m_root = inter_buf[(++tot1) % ALTER_SIZE];
        if (root_off == -1){
            root_off = DataBase.createElementVirt(NODE_SIZE);
            m_root = leaf_buf[(++tot2) % ALTER_SIZE];
            m_root->setKeyNum(0);
            head_off = root_off;
            m_maxkey = key;
            int u = root_off;
            printf("root_off %d\n", root_off);
        }
        else {
            DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
            if (m_root->getType() == LEAF){
                m_root = leaf_buf[(++tot2) % ALTER_SIZE];
                DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
            }
        }

        if (m_root->getKeyNum() >= MAXNUM_KEY){
            int new_off = DataBase.createElementVirt(NODE_SIZE);
            InterNode *newnode = inter_buf[(++tot1) % ALTER_SIZE];
            newnode->setType(INTERNAL);
            newnode->setKeyNum(0);
            newnode->setChild(0, root_off);
            DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
            m_root->split(new_off, 0, root_off);
            root_off = new_off;

            DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
        }

        DataBase.setElementVirt((char*)m_root, root_off, NODE_SIZE);

        if (key > m_maxkey) m_maxkey = key;
        inter_insert(root_off, key, data, 0);

        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);

        return true;

    }

    long long getsize(){
        return size;
    }

    bool remove(KeyType key){
        if (search(key) == -1) return false;
        size--;

        CNode < KeyType , DataType > *m_root = new InterNode();
        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
        if (m_root->getKeyNum() == 1){
            if (m_root->getType() == LEAF){
                m_root->setKey(0, 0);
                m_root->setKeyNum(0);
                root_off = -1;
                head_off = -1;
                //clear();
                return true;
            }
            int ch1_off = ((InterNode*)m_root)->getchild(0);
            int ch2_off = ((InterNode*)m_root)->getchild(1);
            CNode < KeyType , DataType > *ch1 = new InterNode(), *ch2 = new InterNode();
            DataBase.getElementVirt((char*)ch1, ch1_off, NODE_SIZE);
            DataBase.getElementVirt((char*)ch2, ch2_off, NODE_SIZE);
            if (ch1->getType() == LEAF){
                delete ch1;
                delete ch2;
                ch1 = new LeafNode();
                ch2 = new LeafNode();
                DataBase.getElementVirt((char*)ch1, ch1_off, NODE_SIZE);
                DataBase.getElementVirt((char*)ch2, ch2_off, NODE_SIZE);
            }

            if (ch1->getKeyNum() == MINNUM_KEY && ch2->getKeyNum() == MINNUM_KEY){
                ch1->merge(root_off, ch2_off, 0, ch1_off);
                root_off = ch1_off;
            }


        }
        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
        inter_remove(root_off, key);


        return true;
    }

    bool update(KeyType key, DataType _new){
        if (search(key) == -1) return false;
        inter_update(root_off, key, _new, 0);
    }

    DataType search(KeyType key){
        return inter_search(root_off, key);
    }


    void print(){
//        cout << "root_off: " << root_off << endl;
        cnt = 1;
        if (root_off == -1) return;
        print(root_off, cnt);
    }
    void print(int p_off, int cnt){
        CNode < KeyType, DataType > *p = new InterNode();
        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        printf("type = %d\n", p->getType());
        if (p->getType() == LEAF){
            delete p;
            p = new LeafNode();
            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        }
        cout << "第" << cnt << "层" << p_off  << "      ";
        printnode(p_off);
        if (p->getType() != LEAF) {
            printf("[%u]", p->getKeyNum());
            for (int i = 0; i <= p->getKeyNum(); ++i){
                printf("%u ", ((InterNode*)p)->getchild(i));
            }
            printf("\n");
        }
        cout << endl;
        if (p->getType() != LEAF){
            for (int i = 0; i <= p->getKeyNum(); ++i){
                print(((InterNode*)p)->getchild(i), cnt + 1);
            }
        }
    }
    void printnode(int p_off){
        CNode < KeyType, DataType > *p = new InterNode();
        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        for (int i = 0; i < p->getKeyNum(); ++i){
            cout << p->getKey(i) << " ";
        }

    }
    KeyType getmax(){
        return m_maxkey;
    }

private:
    bool inter_update(int p_off, KeyType key, DataType _new, int depth){
        CNode < KeyType, DataType > *p = inter_buf[(++tot1) % ALTER_SIZE];
        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            p = leaf_buf[(++tot2) % ALTER_SIZE];
            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        }
        if (p == nullptr) return false;
        int keyindex = p->getKeyIndex(key);
        int childindex = p->getchildindex(key, keyindex);
        if (p->getType() == LEAF){
            if (key == p->getKey(keyindex)){
                ((LeafNode*)p)->setdata(keyindex, _new);
                DataBase.setElementVirt((char*)p, p_off, NODE_SIZE);
                return true;
            }
            else return false;
        }

        return inter_update(((InterNode*)p)->getchild(childindex), key, _new, depth + 1);
    }



    void inter_insert(int fa_off, KeyType key, const DataType & data, int depth){
        CNode < KeyType , DataType > *fa = inter_buf[(++tot1) % ALTER_SIZE];
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);

        if (fa->getType() == LEAF){
            fa = leaf_buf[(++tot2) % ALTER_SIZE];
            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);

            ((LeafNode*)fa)->insert(key, data);

            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
            return;
        }

        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);

        int child_off = ((InterNode*)fa)->getchild(childindex);

        CNode < KeyType, DataType > *childnode = inter_buf[(++tot1) % ALTER_SIZE];
        DataBase.getElementVirt((char*)childnode, child_off, NODE_SIZE);
        if (childnode->getType() == LEAF) {
            childnode = leaf_buf[(++tot2) % ALTER_SIZE];
            DataBase.getElementVirt((char*)childnode, child_off, NODE_SIZE);
        }

        if (childnode->getKeyNum() >= MAXNUM_KEY){

            childnode->split(fa_off, childindex, child_off);

            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);

            if (key >= fa->getKey(childindex)){
                child_off = ((InterNode*)fa)->getchild(childindex + 1);
            }

        }
        inter_insert(child_off, key, data, depth + 1);
    }

    void inter_remove(int fa_off, KeyType key){
        CNode < KeyType , DataType > *fa = new InterNode();
        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        if (fa->getType() == LEAF){
            delete fa;
            fa = new LeafNode();
            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
        }
        int keyindex = fa->getKeyIndex(key);
        int childindex = fa->getchildindex(key, keyindex);
        if (fa->getType() == LEAF){
            if (key == m_maxkey && keyindex > 0){
                m_maxkey = fa->getKey(keyindex - 1);   //////
            }
            fa->removekey(keyindex, childindex);
            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
            CNode < KeyType , DataType > *root = new InterNode();
            DataBase.getElementVirt((char*)root, root_off, NODE_SIZE);
            if (childindex == 0 && root->getType() != LEAF && fa_off != head_off){
                changekey(root_off, key, fa->getKey(0));
            }
            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
        }
        else {
            int child_off = ((InterNode*)fa)->getchild(childindex);
            CNode < KeyType , DataType > *child = new InterNode;
            DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
            if (child->getType() == LEAF){
                delete child;
                child = new LeafNode();
                DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
            }

            if (child->getKeyNum() == MINNUM_KEY){
                int left_off = childindex > 0 ? ((InterNode*)fa)->getchild(childindex - 1) : -1;
                int right_off = childindex < fa->getKeyNum() ? ((InterNode*)fa)->getchild(childindex + 1): -1;
                CNode < KeyType , DataType > *left = new InterNode(), *right = new InterNode();
                if (left_off != -1) {
                    DataBase.getElementVirt((char*)left, left_off, NODE_SIZE);
                    if (left->getType() == LEAF){
                        delete left;
                        left = new LeafNode();
                        DataBase.getElementVirt((char*)left, left_off, NODE_SIZE);
                        if (right_off != -1){
                            delete right;
                            right = new LeafNode();
                            DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
                        }
                    }
                    else{
                        DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
                    }

                }
                else if (right_off != -1){
                    DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
                    if (right->getType() == LEAF){
                        delete right;
                        right = new LeafNode();
                        DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
                    }

                }

                if (left_off != -1 && left->getKeyNum() > MINNUM_KEY) child->steal(left_off, fa_off, childindex - 1, LEFT, child_off);
                else if (right_off != -1 && right->getKeyNum() > MINNUM_KEY) child->steal(right_off, fa_off, childindex, RIGHT, child_off);
                else if (left_off != -1){
                    left->merge(fa_off, child_off, childindex - 1, left_off);
                    child_off  = left_off;
                }
                else if (right_off != -1){
                    child->merge(fa_off, right_off, childindex, child_off);
                }
            }
            DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
            inter_remove(child_off, key);
        }
    }

    DataType inter_search(int p_off, KeyType key){
        //cout << "hhh" << endl;
        if (root_off == -1) return -1;
        CNode < KeyType, DataType > *p = inter_buf[(++tot1) % ALTER_SIZE];
        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            //delete p;
            //p = new LeafNode();
            p = leaf_buf[(++tot2) % ALTER_SIZE];
            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        }
        int keyindex = p->getKeyIndex(key);
        int childindex = p->getchildindex(key, keyindex);
        if (p->getType() == LEAF){
            if (key == p->getKey(keyindex)){
                return ((LeafNode*)p)->getdata(keyindex);
            }

            else {
                //cout << "fuck" << endl;
                return -1;
            }
        }
        return inter_search(((InterNode*)p)->getchild(childindex), key);
    }

    void changekey(int p_off, KeyType _old, KeyType _new){
        CNode < KeyType, DataType > *p = inter_buf[(++tot1) % ALTER_SIZE];
        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        if (p->getType() == LEAF){
            //delete p;
            //p = new LeafNode();
            p = leaf_buf[(++tot2) % ALTER_SIZE];
            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
        }
        if (p != nullptr && p->getType() != LEAF){
            int keyindex = p->getKeyIndex(_old);
            if (_old == p->getKey(keyindex)){
                p->setKey(keyindex, _new);
                DataBase.setElementVirt((char*)p, p_off, NODE_SIZE);
            }
            else {
                changekey(((InterNode*)p)->getchild(keyindex), _old, _new);
            }
        }

    }
};

//#include <iostream>
//#include <map>
//#include <stdlib.h>
//#include <algorithm>
//#include "../dhc/IOManager.hpp"
//
//using namespace std;
//enum NODE_TYPE{INTERNAL = 0, LEAF = 1}; // 结点类型：内结点、叶子结点
//enum SIBLING_DIRECTION{LEFT, RIGHT}; // 兄弟结点方向：左兄弟结点、右兄弟结点
//typedef int KeyType; // 键类型
//typedef int DataType; // 值类型
//const int ORDER = 40; // B+树的阶（非根内结点的最小子树个数）
//const int MINNUM_KEY = ORDER-1; // 最小键值个数
//const int MAXNUM_KEY = 2*ORDER-1; // 最大键值个数
//const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
//const int MAXNUM_CHILD = MAXNUM_KEY+1; // 最大子树个数
//const int MINNUM_LEAF = MINNUM_KEY; // 最小叶子结点键值个数
//const int MAXNUM_LEAF = MAXNUM_KEY; // 最大叶子结点键值个数
//
//const int NODE_SIZE = ORDER * 16 + 24;
//
//
//sjtu::IOManager DataBase("./out.txt");
//
//template <class KeyType, class DataType>
//class CNode{
//protected:
//public:
//    NODE_TYPE m_type;
//    int m_KeyNum;
//    KeyType m_keys[MAXNUM_KEY];
//public:
//    CNode(){
//        m_type = LEAF;
//        m_KeyNum = 0;
//    }
//    virtual ~CNode(){
//        m_KeyNum = 0;
//    }
//
//    NODE_TYPE getType()const{
//        return m_type;
//    }
//
//    void setType(NODE_TYPE type){
//        m_type = type;
//    }
//
//    int getKeyNum() const{
//        return m_KeyNum;
//    }
//
//    void setKeyNum(int i){
//        m_KeyNum = i;
//    }
//
//    KeyType getKey(int i) const{
//        return m_keys[i];
//    }
//
//    void setKey(int i, KeyType key){
//        m_keys[i] = key;
//    }
//
//    int getKeyIndex(KeyType key) const{
//        int l, r;
//        l = 0;
//        r = getKeyNum() - 1;
//
//        if (key > getKey(r)) return r + 1;
//        while (l < r){
//            int m = (r + l) >> 1;
//            if (getKey(m) < key) l = m + 1;
//            else r = m;
//        }
//        return l;
//    }
//
//    virtual void removekey(int keyindex, int childindex) = 0;
//    virtual void split(int fa_off, int childindex, int this_off) = 0;
//    virtual void merge(int fa_off, int child_off, int keyindex, int this_off) = 0;
//    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off) = 0;
//    virtual int getchildindex(KeyType key, int keyindex) const = 0;
//
//};
//
//
//class InterNode:public CNode<KeyType, DataType>{
//private:
//public:
//    int m_Childs[MAXNUM_CHILD];
//    int m_left;
//    int m_right;
//public:
//    InterNode(){
//        m_type = INTERNAL;
//    }
//
//    virtual  ~InterNode(){
//
//    }
//
//    int getchild(int i) const{
//        return m_Childs[i];
//    }
//
//    void setChild(int i, int child_off){
//        m_Childs[i] = child_off;
//    }
//
//    void insert(int keyindex, int childindex, KeyType key, int offset){
//        for (int i = getKeyNum(); i  >= keyindex; --i){
//            setChild(i + 1, m_Childs[i]);
//        }
//        for (int i = getKeyNum(); i > keyindex; --i){
//            setKey(i, m_keys[i - 1]);
//        }
//        setChild(childindex, offset);
//        setKey(keyindex, key);
//        m_KeyNum++;
//    }
//
//    virtual void removekey(int keyindex, int childindex){
//        if (childindex == -1){
//            for (int i = keyindex; i < getKeyNum()- 1; ++i){
//                setKey(i, getKey(i + 1));
//                setChild(i, m_Childs[i + 1]);
//            }
//            setChild(getKeyNum() - 1, m_Childs[getKeyNum()]);
//            m_KeyNum--;
//            return;
//        }
//        for (int i = keyindex; i < getKeyNum()- 1; ++i){
//            setKey(i, getKey(i + 1));
//            setChild(i + 1, m_Childs[i + 2]);
//        }
//        m_KeyNum--;
//    }
//
//    virtual void split(int fa_off, int childindex, int this_off){
//        int new_off = DataBase.createElementVirt(NODE_SIZE);
//        InterNode *newnode  = new InterNode();
//        newnode->setKeyNum(MINNUM_KEY);
//        int i;
//        for (int i = 0; i < MINNUM_KEY; ++i){
//            newnode->setKey(i, m_keys[i + MINNUM_KEY + 1]);
//        }
//        for (int i = 0; i < MINNUM_CHILD; ++i){
//            newnode->setChild(i, m_Childs[i + MINNUM_CHILD]);
//        }
//        setKeyNum(MINNUM_KEY);
//        InterNode* fa = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        fa->insert(childindex, childindex + 1, m_keys[MINNUM_KEY], new_off);
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//
//        delete newnode;
//        delete fa;
//
//    }
//
//    virtual void merge(int fa_off, int child_off, int keyindex, int this_off){
//        InterNode * fa = new InterNode();
//        InterNode * child = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
//        insert(MINNUM_KEY, MINNUM_KEY + 1, fa->getKey(keyindex),  (child)->getchild(0));
//        int i;
//        for (int i = 1; i <= child->getKeyNum(); ++i){
//            insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, child->getKey(i - 1), (child)->getchild(i));
//        }
//        fa->removekey(keyindex, keyindex + 1);
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)child, child_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//        delete fa;
//        delete child;
//    }
//
//    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off){
//        InterNode * fa = new InterNode();
//        InterNode * bro = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.getElementVirt((char*)bro, bro_off, NODE_SIZE);
//        if (d == LEFT){
//            insert(0, 0, fa->getKey(keyindex), ((InterNode*)bro)->getchild(bro->getKeyNum()));
//            fa->setKey(keyindex, bro->getKey(bro->getKeyNum() - 1));
//            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum());
//        }
//        else {
//            insert(getKeyNum(), getKeyNum() + 1, fa->getKey(keyindex), (bro)->getchild(0));
//            fa->setKey(keyindex, bro->getKey(0));
//            bro->removekey(0, -1);
//        }
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)bro, bro_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//        delete fa;
//        delete bro;
//
//    }
//
//    virtual int getchildindex(KeyType key, int keyindex) const {
//        if (keyindex >= getKeyNum()) return keyindex;
//        if (key == getKey(keyindex)){
//            return keyindex + 1;
//        }
//        else return keyindex;
//
//    }
//
//};
//
//
//class LeafNode: public CNode<KeyType, DataType>{
//private:
//    int m_left;
//    int m_right;
//    DataType m_Datas[MAXNUM_CHILD];
//
//public:
//    LeafNode(){
//        m_type = LEAF;
//    }
//
//    virtual ~LeafNode(){
//
//    }
//
//    int getleft()const{
//        return m_left;
//    }
//
//    void setleft(int left){
//        m_left = left;
//    }
//
//    int getright()const{
//        return m_right;
//    }
//
//    void setright(int right){
//        m_right = right;
//    }
//
//    DataType getdata(int i) const {
//        return m_Datas[i];
//    }
//
//    void setdata(int i, const DataType &data){
//        m_Datas[i] = data;
//    }
//
//    void insert(KeyType key, const DataType &data){
//        int i = 0;
//        for (i = m_KeyNum; i >= 1 && m_keys[i - 1] > key; --i){
//            setKey(i, m_keys[i - 1]);
//            setdata(i, m_Datas[i -1]);
//        }
//        setdata(i, data);
//        setKey(i, key);
//        m_KeyNum++;
//    }
//
//    virtual void removekey(int keyindex, int childindex){
//        for (int i = keyindex; i < getKeyNum() - 1; ++i){
//            setKey(i, getKey(i + 1));
//            setdata(i, getdata(i + 1));
//        }
//        m_KeyNum--;
//
//    }
//
//    virtual void split(int fa_off, int childindex, int this_off){
//            cout << "nonono";
//        int new_off = DataBase.createElementVirt(NODE_SIZE);
//        LeafNode * newnode = new LeafNode();
//        InterNode * fa = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        setKeyNum(MINNUM_LEAF);
//        newnode->setKeyNum(MINNUM_LEAF + 1);
//        newnode->setright(getright());
//        setright(new_off);
//        newnode->setleft(this_off);
//        for (int i = 0; i <= MINNUM_LEAF; ++i){
//            newnode->setKey(i, m_keys[i + MINNUM_LEAF]);
//            newnode->setdata(i, m_Datas[i + MINNUM_LEAF]);
//        }
//        (fa)->insert(childindex, childindex + 1, m_keys[MINNUM_LEAF], new_off);
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//          cout << fa->getType() << endl;
//        delete newnode;
//    }
//
//    virtual void merge(int fa_off, int child_off, int keyindex, int this_off){
//        InterNode * fa = new InterNode();
//        InterNode * child = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
//
//        for (int i = 0; i < child->getKeyNum(); ++i){
//            insert(child->getKey(i), ((LeafNode*)child)->getdata(i));
//        }
//        setright(((LeafNode*)child)->getright());
//        fa->removekey(keyindex, keyindex + 1);
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)child, child_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//        delete fa;
//        delete child;
//
//    }
//
//
//    virtual void steal(int bro_off, int fa_off, int keyindex, SIBLING_DIRECTION d, int this_off){
//        InterNode * fa = new InterNode();
//        InterNode * bro = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.getElementVirt((char*)bro, bro_off, NODE_SIZE);
//        if (d == LEFT){
//            insert(bro->getKey(bro->getKeyNum() - 1), ((LeafNode*)bro)->getdata(bro->getKeyNum() - 1));
//            bro->removekey(bro->getKeyNum() - 1, bro->getKeyNum() - 1);
//            fa->setKey(keyindex, getKey(0));
//        }
//        else {
//            insert(bro->getKey(0), ((LeafNode*)bro)->getdata(0));
//            bro->removekey(0, 0);
//            fa->setKey(keyindex, bro->getKey(0));
//        }
//        DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)bro, bro_off, NODE_SIZE);
//        DataBase.setElementVirt((char*)this, this_off, NODE_SIZE);
//        delete fa;
//        delete bro;
//    }
//
//    virtual int getchildindex(KeyType key, int keyindex) const {
//        return keyindex;
//    }
//
//
//};
//
//class BPlusTree{
//    struct ans{
//        int keyindex;
//        LeafNode * node;
//    };
//private:
//    int meta_off;
//    int root_off;
//    int head_off;
//    KeyType m_maxkey;
//    int cnt;
//    int size;
//public:
//    BPlusTree() {
//    }
//    BPlusTree(int _meta_off, int is_init) {
//        meta_off = _meta_off;
//        if (is_init) {
//            root_off = -1;
//            DataBase.setElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
//            head_off = -1;
//            DataBase.setElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
//            m_maxkey = 0;
//            DataBase.setElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
//            size = 0;
//            DataBase.setElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
//        } else {
//            DataBase.getElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
//            DataBase.getElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
//            DataBase.getElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
//            DataBase.getElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
//        }
//    }
//
//    ~BPlusTree(){
//        DataBase.setElement((char*)&root_off, meta_off + 0 * sizeof(int), sizeof(int));
//        DataBase.setElement((char*)&head_off, meta_off + 1 * sizeof(int), sizeof(int));
//        DataBase.setElement((char*)&m_maxkey, meta_off + 2 * sizeof(int), sizeof(int));
//        DataBase.setElement((char*)&size, meta_off + 3 * sizeof(int), sizeof(int));
//    }
//
//    int getRootOff() {
//        return root_off;
//    }
//    bool insert(KeyType key, const DataType & data){
//        if (search(key) != -1) return false;
//
////        printf("insert %d %d\n", key, data);
//
//        size++;
//        CNode < KeyType, DataType > *m_root = new InterNode();
//        if (root_off == -1){
//            root_off = DataBase.createElementVirt(NODE_SIZE);
//            delete m_root;
//            m_root = new LeafNode();
//            m_root->setKeyNum(0);
//            head_off = root_off;
//            m_maxkey = key;
////            printf("root_off %d\n", root_off);
//        }
//        else {
//            DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//            if (m_root->getType() == LEAF){
//                delete m_root;
//                m_root = new LeafNode();
//                DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//            }
//        }
//
//        if (m_root->getKeyNum() >= MAXNUM_KEY){
//            int new_off = DataBase.createElementVirt(NODE_SIZE);
//            InterNode *newnode = new InterNode();
//            newnode->setKeyNum(0);
//            newnode->setChild(0, root_off);
//            DataBase.setElementVirt((char*)newnode, new_off, NODE_SIZE);
//            m_root->split(new_off, 0, root_off);
//            root_off = new_off;
//
//
//            DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//        }
//
//        DataBase.setElementVirt((char*)m_root, root_off, NODE_SIZE);
//
//        if (key > m_maxkey) m_maxkey = key;
//        inter_insert(root_off, key, data, 0);
//
//        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//
//        return true;
//
//    }
//
//    long long getsize(){
//        return size;
//    }
//
//    bool remove(KeyType key){
//        if (search(key) == -1) return false;
//        size--;
//
//        CNode < KeyType , DataType > *m_root = new InterNode();
//        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//        if (m_root->getKeyNum() == 1){
//            if (m_root->getType() == LEAF){
//                m_root->setKey(0, 0);
//                m_root->setKeyNum(0);
//                root_off = -1;
//                head_off = -1;
//                //clear();
//                return true;
//            }
//            int ch1_off = ((InterNode*)m_root)->getchild(0);
//            int ch2_off = ((InterNode*)m_root)->getchild(1);
//            CNode < KeyType , DataType > *ch1 = new InterNode(), *ch2 = new InterNode();
//            DataBase.getElementVirt((char*)ch1, ch1_off, NODE_SIZE);
//            DataBase.getElementVirt((char*)ch2, ch2_off, NODE_SIZE);
//            if (ch1->getType() == LEAF){
//                delete ch1;
//                delete ch2;
//                ch1 = new LeafNode();
//                ch2 = new LeafNode();
//                DataBase.getElementVirt((char*)ch1, ch1_off, NODE_SIZE);
//                DataBase.getElementVirt((char*)ch2, ch2_off, NODE_SIZE);
//            }
//
//            if (ch1->getKeyNum() == MINNUM_KEY && ch2->getKeyNum() == MINNUM_KEY){
//                ch1->merge(root_off, ch2_off, 0, ch1_off);
//                root_off = ch1_off;
//            }
//
//
//        }
//        DataBase.getElementVirt((char*)m_root, root_off, NODE_SIZE);
//        inter_remove(root_off, key);
//
//
//        return true;
//    }
//
//    bool update(KeyType key, DataType _new){
//        if (search(key) == -1) return false;
//        inter_update(root_off, key, _new, 0);
//    }
//
//    DataType search(KeyType key){
//        return inter_search(root_off, key);
//    }
//
//
//    void print(){
////        cout << "root_off: " << root_off << endl;
//        cnt = 1;
//        if (root_off == -1) return;
//        print(root_off, cnt);
//    }
//    void print(int p_off, int cnt){
//        CNode < KeyType, DataType > *p = new InterNode();
//        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        printf("type = %d\n", p->getType());
//        if (p->getType() == LEAF){
//            delete p;
//            p = new LeafNode();
//            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        }
//        cout << "第" << cnt << "层" << p_off  << "      ";
//        printnode(p_off);
//        if (p->getType() != LEAF) {
//            printf("[%u]", p->getKeyNum());
//            for (int i = 0; i <= p->getKeyNum(); ++i){
//                printf("%u ", ((InterNode*)p)->getchild(i));
//            }
//            printf("\n");
//        }
//        cout << endl;
//        if (p->getType() != LEAF){
//            for (int i = 0; i <= p->getKeyNum(); ++i){
//                print(((InterNode*)p)->getchild(i), cnt + 1);
//            }
//        }
//    }
//    void printnode(int p_off){
//        CNode < KeyType, DataType > *p = new InterNode();
//        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        for (int i = 0; i < p->getKeyNum(); ++i){
//            cout << p->getKey(i) << " ";
//        }
//
//    }
//    KeyType getmax(){
//        return m_maxkey;
//    }
//
//private:
//    bool inter_update(int p_off, KeyType key, DataType _new, int depth){
//        CNode < KeyType, DataType > *p = new InterNode();
//        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        if (p->getType() == LEAF){
//            delete p;
//            p = new LeafNode();
//            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        }
//        if (p == nullptr) return false;
//        int keyindex = p->getKeyIndex(key);
//        int childindex = p->getchildindex(key, keyindex);
//        if (p->getType() == LEAF){
//            if (key == p->getKey(keyindex)){
//                ((LeafNode*)p)->setdata(keyindex, _new);
//                DataBase.setElementVirt((char*)p, p_off, NODE_SIZE);
//                return true;
//            }
//            else return false;
//        }
//
//        return inter_update(((InterNode*)p)->getchild(childindex), key, _new, depth + 1);
//    }
//
//
//
//    void inter_insert(int fa_off, KeyType key, const DataType & data, int depth){
//        CNode < KeyType , DataType > *fa = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//
//        if (fa->getType() == LEAF){
//            delete fa;
//            fa = new LeafNode();
//            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//
//            ((LeafNode*)fa)->insert(key, data);
//
//            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//            return;
//        }
//
//        int keyindex = fa->getKeyIndex(key);
//        int childindex = fa->getchildindex(key, keyindex);
//
//        int child_off = ((InterNode*)fa)->getchild(childindex);
//
//        CNode < KeyType, DataType > *childnode = new InterNode();
//        DataBase.getElementVirt((char*)childnode, child_off, NODE_SIZE);
//        if (childnode->getType() == LEAF) {
//
//            delete childnode;
//            childnode = new LeafNode();
//            DataBase.getElementVirt((char*)childnode, child_off, NODE_SIZE);
//        }
//
//        if (childnode->getKeyNum() >= MAXNUM_KEY){
//
//            childnode->split(fa_off, childindex, child_off);
//
//            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//
//            if (key >= fa->getKey(childindex)){
//                child_off = ((InterNode*)fa)->getchild(childindex + 1);
//            }
//
//        }
//        inter_insert(child_off, key, data, depth + 1);
//    }
//
//    void inter_remove(int fa_off, KeyType key){
//        CNode < KeyType , DataType > *fa = new InterNode();
//        DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        if (fa->getType() == LEAF){
//            delete fa;
//            fa = new LeafNode();
//            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//        }
//        int keyindex = fa->getKeyIndex(key);
//        int childindex = fa->getchildindex(key, keyindex);
//        if (fa->getType() == LEAF){
//            if (key == m_maxkey && keyindex > 0){
//                m_maxkey = fa->getKey(keyindex - 1);   //////
//            }
//            fa->removekey(keyindex, childindex);
//            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//            DataBase.getElementVirt((char*)fa, fa_off, NODE_SIZE);
//            CNode < KeyType , DataType > *root = new InterNode();
//            DataBase.getElementVirt((char*)root, root_off, NODE_SIZE);
//            if (childindex == 0 && root->getType() != LEAF && fa_off != head_off){
//                changekey(root_off, key, fa->getKey(0));
//            }
//            DataBase.setElementVirt((char*)fa, fa_off, NODE_SIZE);
//        }
//        else {
//            int child_off = ((InterNode*)fa)->getchild(childindex);
//            CNode < KeyType , DataType > *child = new InterNode;
//            DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
//            if (child->getType() == LEAF){
//                delete child;
//                child = new LeafNode();
//                DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
//            }
//
//            if (child->getKeyNum() == MINNUM_KEY){
//                int left_off = childindex > 0 ? ((InterNode*)fa)->getchild(childindex - 1) : -1;
//                int right_off = childindex < fa->getKeyNum() ? ((InterNode*)fa)->getchild(childindex + 1): -1;
//                CNode < KeyType , DataType > *left = new InterNode(), *right = new InterNode();
//                if (left_off != -1) {
//                    DataBase.getElementVirt((char*)left, left_off, NODE_SIZE);
//                    if (left->getType() == LEAF){
//                        delete left;
//                        left = new LeafNode();
//                        DataBase.getElementVirt((char*)left, left_off, NODE_SIZE);
//                        if (right_off != -1){
//                            delete right;
//                            right = new LeafNode();
//                            DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
//                        }
//                    }
//                    else{
//                        DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
//                    }
//
//                }
//                else if (right_off != -1){
//                    DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
//                    if (right->getType() == LEAF){
//                        delete right;
//                        right = new LeafNode();
//                        DataBase.getElementVirt((char*)right, right_off, NODE_SIZE);
//                    }
//
//                }
//
//                if (left_off != -1 && left->getKeyNum() > MINNUM_KEY) child->steal(left_off, fa_off, childindex - 1, LEFT, child_off);
//                else if (right_off != -1 && right->getKeyNum() > MINNUM_KEY) child->steal(right_off, fa_off, childindex, RIGHT, child_off);
//                else if (left_off != -1){
//                    left->merge(fa_off, child_off, childindex - 1, left_off);
//                    child_off  = left_off;
//                }
//                else if (right_off != -1){
//                    child->merge(fa_off, right_off, childindex, child_off);
//                }
//            }
//            DataBase.getElementVirt((char*)child, child_off, NODE_SIZE);
//            inter_remove(child_off, key);
//        }
//    }
//
//    DataType inter_search(int p_off, KeyType key) const{
//        if (root_off == -1) return -1;
//        CNode < KeyType, DataType > *p = new InterNode();
//        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        if (p->getType() == LEAF){
//            delete p;
//            p = new LeafNode();
//            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        }
//        int keyindex = p->getKeyIndex(key);
//        int childindex = p->getchildindex(key, keyindex);
//        if (p->getType() == LEAF){
//            if (key == p->getKey(keyindex)){
//                return ((LeafNode*)p)->getdata(keyindex);
//            }
//            else return -1;
//        }
//        return inter_search(((InterNode*)p)->getchild(childindex), key);
//    }
//
//    void changekey(int p_off, KeyType _old, KeyType _new){
//        CNode < KeyType, DataType > *p = new InterNode();
//        DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        if (p->getType() == LEAF){
//            delete p;
//            p = new LeafNode();
//            DataBase.getElementVirt((char*)p, p_off, NODE_SIZE);
//        }
//        if (p != nullptr && p->getType() != LEAF){
//            int keyindex = p->getKeyIndex(_old);
//            if (_old == p->getKey(keyindex)){
//                p->setKey(keyindex, _new);
//                DataBase.setElementVirt((char*)p, p_off, NODE_SIZE);
//            }
//            else {
//                changekey(((InterNode*)p)->getchild(keyindex), _old, _new);
//            }
//        }
//
//    }
//};
//
