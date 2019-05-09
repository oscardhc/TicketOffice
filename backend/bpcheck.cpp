#include "bplustree.hpp"
#include <map>
#include <stdlib.h>
#include <algorithm>
using namespace std;

bplustree bt;
map<int,int> mp;
const int N = 100;
pair<int, int> a[N];

int main(){
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        a[i] = make_pair(i, rand());
    }
    random_shuffle(a, a + N);
    for (int i = 0; i < N; i++) {
        bt.insert(a[i].first, a[i].second);
    }
    random_shuffle(a, a + N);
    for (int i = 0; i < N; i++) {
        if (rand() % 2) {
            a[i].second = rand();
            bt.update(a[i].first, a[i].second);
        }
    }
    random_shuffle(a, a + N);
    for (int i = 0; i < 10; i++) {
        bt.remove(a[i].first);
    }
    for (int i = 10; i < N; i++) {
        // printf("pair(%d, %d) stdmp = %d bptree = %d\n", a[i].first, a[i].second, mp[a[i].first], bt.find(a[i].first));
        if (a[i].second != bt.search(a[i].first)) {
            printf("ERROR! key %d, %d expected, %d find\n", a[i].first, a[i].second, bt.search(a[i].first));
        }
    }
    printf("OKKKKKKKKK!");
    return 0;
}
