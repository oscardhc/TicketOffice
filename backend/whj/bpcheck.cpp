#include "bpt_new.hpp"
#include <map>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

BPlusTree bt;
map<int,int> mp;
const int N = 100000;
pair<int, int> a[N + 2];


////ssfd。ssfd。ssfd。ssfd。
int main(){
    srand(138269);
//    printf("%u\n", sizeof(LeafNode));
//    return 0;

    for (int i = 1; i <= N; i++) {
        a[i] = make_pair(i, rand());
    }
    random_shuffle(a + 1, a + N + 1);

    for (int i = 1; i <= N; i++) {
        bt.insert(a[i].first, a[i].second);
        //bt.print();
        //cout << endl;
        //bt.print();
        //cout << endl;
    }

    //return 0;
    //bt.insert(6,6);
    random_shuffle(a + 1, a + N + 1);
    for (int i = 1; i <= N; i++) {
        if (rand() % 2) {
            a[i].second = rand();
            bt.update(a[i].first, a[i].second);
        }
    }
    random_shuffle(a + 1, a + N + 1);
    for (int i = 1; i < 10; i++) {
        printf("removed %d\n", a[i].first);
        bt.remove(a[i].first);
    }
    for (int i = 10; i <= N; i++) {
        if (i % 10000 == 0) printf("pair(%d, %d) bptree = %d\n", a[i].first, a[i].second, bt.search(a[i].first));
        if (a[i].second != bt.search(a[i].first)) {
            printf("ERROR! key %d, %d expected, %d find\n", a[i].first, a[i].second, bt.search(a[i].first));
            return 0;
        }
    }
    printf("OKKKKKKKKK! time = %lf s\n", 1.0 * clock() / 1000000);
    return 0;
}
