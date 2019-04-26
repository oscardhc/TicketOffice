#include <iostream>
#include <mach/vm_statistics.h>

#define nullptr 0
#define INVALID 0
#define FLAG_LEFT 1
#define FLAG_RIGHT 2

typedef int KeyType;
typedef int DataType;
#define ORDER 5
#define MAXNUM_KEY (ORDER * 2)
#define MAXNUM_POINTER (MAXNUM_KEY + 1)
#define MAXNUM_DATA (ORDER * 2)

enum NODE_TYPE{
    NODE_TYPE_ROOT = 1,
    NODE_TYPE_INTERNAL = 2,
    NODE_TYPE_LEAF = 3,
};

/// in 0 - n-1  out 1-n
class CNode{
protected:
    NODE_TYPE m_type;
    int m_count; //the number of pointer
    CNode * m_pfather;
public:
    CNode(){
        m_type = NODE_TYPE_LEAF;
        m_count = 1;
        m_pfather = nullptr;
    }
    virtual ~CNode(){
        deletechildren();
    }

    NODE_TYPE gettype(){
        return m_type;
    }
    void settype(NODE_TYPE type){
        m_type = type;
    }
    int getcount(){
        return m_count;
    }
    void setcount(int i){
        m_count = i;
    }
    virtual KeyType getelement(int i){
        return 0;
    }
    virtual void setelement(int i, KeyType value) {}

    virtual CNode *getpointer(int i){
        return nullptr;
    }
    virtual void setpointer(int i, CNode *pointer) {}

    CNode *getfather(){
        return m_pfather;
    }
    void setfather(CNode * father){
        m_pfather = father;
    }
    CNode * getbrother(int &flag){
        CNode *pfather = getfather();
        if (pfather == nullptr) return nullptr;
        CNode *bro = nullptr;
        for (int i = 1; i <= pfather->getcount(); ++i){
            if (this == pfather->getpointer(pfather->getcount())) {
                flag = FLAG_LEFT;
                bro = pfather->getpointer(pfather->getcount() - 1);
                return bro;
            }

            if (pfather->getpointer(i) == this){
                flag = FLAG_RIGHT;
                bro = pfather->getpointer(i + 1);
                return bro;
            }
        }
    }

    void deletechildren(){
        for (int i = 1; i <= getcount(); ++i){
            CNode *p = getpointer(i);
            if (p != nullptr){
                p->deletechildren();
            }
            delete p;
        }
    }



};


class InternalNode:public CNode {
protected:
    KeyType m_Keys[MAXNUM_KEY];
    CNode * m_pointers[MAXNUM_POINTER];
public:
    InternalNode(){
        m_type = NODE_TYPE_INTERNAL;
        for (int i = 0; i < MAXNUM_KEY; ++i){
            m_Keys[i] = 0;
        }
        for (int i = 1; i < MAXNUM_POINTER; ++i){
            m_pointers[i] = nullptr;
        }
    }
    virtual ~InternalNode(){
        for (int i = 1; i < MAXNUM_POINTER; ++i){
            m_pointers[i] = nullptr;
        }
    }


    KeyType getelement(int i){
        if (i > 0 && i <= MAXNUM_KEY)
            return m_Keys[i - 1];
        else return INVALID;
    }
    void setelement(int i, KeyType value) {
        if (i > 0 && i <= MAXNUM_KEY)
            m_Keys[i - 1] = value;
    }
    CNode *getpointer(int i){
        if (i > 0 && i <= MAXNUM_POINTER)
            return m_pointers[i - 1];
        else return nullptr;
    }
    void setpointer(int i, CNode *pointer){
        if (i > 0 && i <= MAXNUM_POINTER)
            m_pointers[i - 1] = pointer;
    }
    bool Insert(KeyType value, CNode *pnode){
        if (getcount() >= MAXNUM_POINTER) return false;
        pnode->setfather(this);

        int k, i;
        for (i = 0; i < getcount() - 1 && value > m_Keys[i]; ++i);
        for (k = getcount() - 1; k > i; --k){
            m_Keys[k] = m_Keys[k - 1];
        }
        for (k = getcount(); k > i + 1; --k){
            m_pointers[k] = m_pointers[k - 1];
        }

        m_Keys[i] = value;
        m_pointers[i + 1] = pnode;
        m_count++;

        return true;
    }
    bool Delete(KeyType value){
        int i, k;
        for (i = 0; i < getcount() - 1 && value != m_Keys[i]; ++i);
        if (i == getcount() - 1) return false;
        m_count--;
        for (k = i; k < getcount() - 1; ++k){
            m_Keys[k] = m_Keys[k + 1];
        }
        for (k = i + 1; k < getcount(); ++k){
            m_pointers[k] = m_pointers[k + 1];
        }
        m_pointers[m_count] = nullptr;
        return true;

    }

    KeyType split(InternalNode * pnode, KeyType key){
        int cnt = 0;
        if (key > m_Keys[ORDER - 1] && key < m_Keys[ORDER]){
            for (int i = ORDER + 1; i <= MAXNUM_KEY; ++i){
                ++cnt;
                pnode->setelement(cnt, m_Keys[i - 1]);
                this->setelement(i, 0);
            }
            cnt = 0;
            for (int i = ORDER + 2; i <= MAXNUM_POINTER; ++i){
                ++cnt;
                m_pointers[i - 1]->setfather(pnode);
                pnode->setpointer(i, m_pointers[i - 1]);
                this->setpointer(i, nullptr);
            }
            this->setcount(ORDER);
            pnode->setcount(ORDER);
            return key;
        }
        int pos;
        if (key < m_Keys[ORDER - 1]) pos = ORDER;
        else pos = ORDER + 1;
        for (int i = pos + 1; i <= MAXNUM_KEY; ++i){
            ++cnt;
            pnode->setelement(cnt, m_Keys[i - 1]);
            this->setelement(i, 0);
        }
        cnt = 0;
        for (int i = pos + 1; i <= MAXNUM_POINTER; ++i){
            ++cnt;
            m_pointers[i - 1]->setfather(pnode);
            pnode->setpointer(i, m_pointers[i - 1]);
            this->setpointer(i, nullptr);
        }
        int tmp = getelement(pos);
        this->setelement(pos, 0);
        this->setcount(pos - 1);
        pnode->setcount(MAXNUM_KEY - pos);
        return tmp;
    }
    bool combine(CNode *pnode){
        if (this->getcount() + pnode->getcount() + 1 > MAXNUM_KEY){
            return false;
        }
        KeyType tmp = pnode->getpointer(1)->getelement(1);

        m_Keys[m_count - 1] = tmp;
        m_count++;
        m_pointers[m_count - 1] = pnode->getpointer(1);

        for (int i = 1; i < pnode->getcount(); ++i){
            m_Keys[m_count - 1] = pnode->getelement(i);
            m_count++;
            m_pointers[m_count - 1] = pnode->getpointer(i + 1);
        }

        return true;


    }
    ///steal
    bool move(CNode *pnode){
        if (this->getcount() >= MAXNUM_KEY) return false;

        if (pnode->getelement(1) < getelement(1)){
            for (int i = getcount(); i; --i){
                m_pointers[i] = m_pointers[i - 1];
            }
            for (int i = getcount() - 1; i; --i){
                m_Keys[i] = m_Keys[i - 1];
            }

            m_Keys[0] = getpointer(1)->getelement(1);
            m_pointers[0] = pnode->getpointer(pnode->getcount());
            m_pointers[0]->setfather(this);
            m_count++;
            pnode->setcount(pnode->getcount() - 1);

        }
        else {
            m_Keys[m_count - 1] = pnode->getpointer(1)->getelement(1);
            m_pointers[m_count] = pnode->getpointer(1);
            m_pointers[m_count]->setfather(this);
            m_count++;
            pnode->setcount(pnode->getcount() - 1);

            for (int i = 1; i <= pnode->getcount(); ++i){
                pnode->setelement(i, pnode->getelement(i + 1));
            }
            for (int i = 1; i < pnode->getcount(); ++i){
                pnode->setpointer(i, pnode->getpointer(i + 1));
            }

        }
        return true;


    }

};

class LeafNode:public CNode{
private:
    LeafNode * prev;
    LeafNode * next;

protected:
    KeyType m_datas[MAXNUM_DATA];

public:
    LeafNode(){
        m_type = NODE_TYPE_LEAF;
        m_count = 0;
        for (int i = 0; i < MAXNUM_DATA; ++i){
            m_datas[i] = 0;
        }
        prev = nullptr;
        next = nullptr;

    }
    virtual ~LeafNode(){

    }
    KeyType getelement(int i){
        if (i > 0 && i < MAXNUM_DATA)
            return m_datas[i - 1];
        else return INVALID;
    }
    void setelemet(int i, KeyType data){
        if (i > 0 && i <MAXNUM_DATA)
            m_datas[i - 1] = data;
    }

    CNode *getpointer(int i){
        return nullptr;
    }

    bool Insert(KeyType value){
        if (getcount() > MAXNUM_DATA) return false;

        int i;
        for (i = 0; i < getcount() && m_datas[i] <= value; ++i);
        for (int k = getcount(); k > i; --k){
            m_datas[k] = m_datas[k - 1];
        }
        m_datas[i] = value;
        m_count++;

        return true;
    }

    bool Delete(KeyType value){
        bool flag = false;
        int i;
        for (i = 0; i < getcount(); ++i){
            if (value == m_datas[i]) {
                flag = true;
                break;
            }
        }
        if (i == getcount()) return flag;
        for (int k = i; k < getcount() - 1; ++k){
            m_datas[k] = m_datas[k + 1];
        }
        m_datas[getcount() - 1] = 0;
        m_count--;

        return true;
    }

    KeyType split(CNode *pnode){
        int j = 0;
        for (int i = ORDER + 1; i <= MAXNUM_DATA){
            ++j;
            pnode->setelement(j, getelement(i));
            setelement(i, 0);
        }
        setcount(ORDER);
        pnode->setcount(ORDER);
        return pnode->getelement(1);

    }


    bool combine(CNode *pnode){
        if (getcount() + pnode->getcount() > MAXNUM_DATA) return false;

        for (int i = getcount(); i < getcount() + pnode->getcount(); i++)
        {
            m_datas[i] = pnode->getelement(i - getcount());
        }
        return true;
    }
};


class bplustree{
private:
    LeafNode *head;
    LeafNode *tail;
protected:
    LeafNode *find(KeyType data){

    }
    bool InsertInter(InternalNode *pnode, KeyType key, CNode * rson){

    }
    bool DeleteInter(InternalNode *pnode, KeyType key){

    }

    CNode *m_root;
    int m_depth;
public:
    bplustree(){

    }
    virtual ~bplustree(){

    }

    KeyType search(KeyType data){

    }
    bool Insert(KeyType data){

    }
    bool Delete(KeyType value){

    }

    void clear(){

    }
    bplustree *rotate(){

    }
    //是否是一棵b+树
    bool check(){

    }
    bool checknode(){

    }
    CNode *getroot(){
        return m_root;
    }
    void setroot(CNode * root){
        m_root = root;
    }
    int getdepth(){
        return m_depth;
    }
    void setdepth(int depth){
        m_depth = depth;
    }
};

int main(){



    return 0;
}
