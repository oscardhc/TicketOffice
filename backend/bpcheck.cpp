#include "bplustree.hpp"
#include <map>
#include <stdlib.h>
#include <algorithm>
using namespace std;

bplustree bt;
map<int,int> mp;
const int N = 10;
pair<int, int> a[N];

int main(){
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        a[i] = make_pair(i, rand());
    }
    random_shuffle(a, a + N);
    for (int i = 0; i < N; i++) {
        bt.insert(a[i].first, a[i].second);
        mp.insert(a[i].first, a[i].second);
    }
    random_shuffle(a, a + N);
    for (int i = 0; i < N / 2; i++) {
        bt.erase(a[i].first);
        mp.erase(a[i].first);
    }
    for (int i = N / 2; i < N; i++) {
        printf("pair(%d, %d) stdmp = %d bptree = %d\n", a[i].first, a[i].second, mp[a[i].first], bt.find(a[i].second));
    }
    return 0;
}
