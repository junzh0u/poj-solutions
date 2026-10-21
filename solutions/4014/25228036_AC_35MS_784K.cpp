// POJ 4014 - Dice
// Model: gpt-6-astra
// A face on a die of size a has weight 1/a in the expectation.
// Assign larger labels to smaller dice by the exchange argument.
// Each die receives a consecutive interval; its mean is its midpoint.
// Print dice in input order. The input describes one instance.
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<pair<int,int> > order;
    vector<int> lo(n), hi(n);
    int total = 0;
    for (int i=0; i<n; ++i) {
        int a; scanf("%d", &a);
        order.push_back(make_pair(a,i)); total += a;
    }
    sort(order.begin(),order.end());
    double answer = 0;
    for (int i=0; i<n; ++i) {
        int id=order[i].second, a=order[i].first;
        hi[id]=total; lo[id]=total-a+1; total-=a;
        answer += (lo[id]+hi[id])/2.0;
    }
    printf("%.5f\n",answer);
    for (int i=0; i<n; ++i) {
        for (int x=lo[i]; x<=hi[i]; ++x)
            printf(x==lo[i] ? "%d" : " %d",x);
        putchar('\n');
    }
    return 0;
}
