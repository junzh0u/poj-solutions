// POJ 1890 - Switching Channels
// Model: gpt-6-astra
// Enumerate all distinct programme orders and minimize the five importance
// totals lexicographically. Include both endpoints among programme boundaries.
// Output prefixes follow the sample; any equally optimal order is valid.
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
typedef long long Int;
Int distanceTo(Int x, Int y) { return x > y ? x-y : y-x; }
int main() {
    int n, cs=0;
    while (cin >> n && n) {
        vector<Int> p(n);
        for(int j=0;j<n;++j) cin >> p[j];
        int a; cin >> a;
        int importance[8]; Int point[8];
        for(int j=0;j<a;++j) cin >> importance[j] >> point[j];
        sort(p.begin(),p.end());
        vector<Int> best, order;
        do {
            vector<Int> cost(5,0);
            for(int j=0;j<a;++j) {
                Int time=0, miss=distanceTo(point[j],0);
                for(int k=0;k<n;++k) {
                    time+=p[k]; miss=min(miss,distanceTo(point[j],time));
                }
                cost[importance[j]-1]+=miss;
            }
            if(best.empty() || cost<best) {best=cost; order=p;}
        } while(next_permutation(p.begin(),p.end()));
        Int error=0;
        for(int j=0;j<5;++j) error+=best[j];
        cout << "Data set " << ++cs << "\nOrder:";
        for(int j=0;j<n;++j) cout << ' ' << order[j];
        cout << "\nError: " << error << '\n';
    }
    return 0;
}
