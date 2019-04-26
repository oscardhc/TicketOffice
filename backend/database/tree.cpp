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



class CNode{
protected:
    NODE_TYPE m_type;
    int m_count;
    CNode * m_pfather;
public:
    CNode();
    virtual ~CNode();

    NODE_TYPE getType(){
        return m_type;
    }
    void setType(NODE_TYPE type){
        m_type = type;
    }
    int getCount(){
        return m_count;
    }
    void setCount(int i){
        m_count = i;
    }
    virtual KeyType getElement(int i){
        return 0;
    }
    virtual void setElement(int i, KeyType value) {
        //////// need to do or nothing;
    }
    virtual CNode *getPointer(int i){
        return nullptr;
    }
    virtual void setPointer(int i, CNode *pointer){
        /////// need to do or nothing;
    }
    CNode *getFather(){
        return m_pfather;
    }
    void setFather(CNode * father){
        m_pfather = father;
    }
    CNode * getBrother(int &flag){
        //////need to do;
    }
    void deleteChildren(){
        ////need to do;
    }



};


class InternalNode:public CNode {
protected:
    KeyType m_Keys[MAXNUM_KEY];
    CNode * m_pointers[MAXNUM_POINTER];
public:
    InternalNode():
    virtual ~InternalNode();


    KeyType getElement(int i){
        if (i > 0 && i <= MAXNUM_KEY)
            return m_Keys[i - 1];
        else return INVALID;
    }
    void setElement(int i, KeyType value) {
        if (i > 0 && i <= MAXNUM_KEY)
            m_Keys[i - 1] = value;
    }
    CNode *getPointer(int i){
        if (i > 0 && i <= MAXNUM_POINTER)
            return m_pointers[i - 1];
        else return nullptr;
    }
    void setPointer(int i, CNode *pointer){
        if (i > 0 && i <= MAXNUM_POINTER)
            m_pointers[i - 1] = pointer;
    }
    bool Insert(KeyType value, CNode *pnode){
        ////need to do;
    }
    bool Delete(KeyType value){
        ////need to do;
    }
    KeyType split(InternalNode * pnode, KeyType key){
        ////need to do;
    }
    bool combine(CNode *pnode){
        ///need to do;
    }
    bool move(CNode *pnode){
        ///need to do;
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

    }
    virtual ~LeafNode(){
        ///
    }
    KeyType getElement(int i){
        if (i > 0 && i < MAXNUM_DATA)
            return m_datas[i - 1];
        else return INVALID;
    }
    void setElemet(int i, KeyType data){
        if (i > 0 && i <MAXNUM_DATA)
            m_datas = data;
    }
    CNode *getPointer(int i){
        return nullptr;
    }
    bool Insert(KeyType value){
        //// need to do;
    }
    bool combine(CNode *pnode){
        ////need to do;
    }
};


class bPlusTree{
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
    bPlusTree(){

    }
    virtual ~bPlusTree(){

    }

    KeyType search(KeyType data){

    }
    bool Insert(KeyType data){

    }
    bool Delete(KeyType value){

    }

    void clear(){

    }
    bPlusTree *rotate(){

    }
    //是否是一棵b+树
    bool check(){

    }
    bool checkNode(){

    }
    CNode *getRoot(){
        return m_root;
    }
    void setRoot(CNode * root){
        m_root = root;
    }
    int getDepth(){
        return m_depth;
    }
    void setDepth(int depth){
        m_depth = depth;
    }
};
