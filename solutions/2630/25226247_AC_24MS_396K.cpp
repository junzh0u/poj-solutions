// POJ 2630 - Security Panel
// Model: gpt-6-astra
// Split the buttons into two halves and match complementary XOR effects.
// For each first-half effect retain a minimum-size subset, then minimize
// the total number of presses. The statement permits any optimal witness;
// equal-size witnesses are resolved by the smaller numeric subset mask.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
struct Entry {
    unsigned effect, mask;
    int count;
    bool operator<(const Entry& b) const {
        if (effect != b.effect) return effect < b.effect;
        if (count != b.count) return count < b.count;
        return mask < b.mask;
    }
};
vector<Entry> subsets(const vector<unsigned>& effect, int begin, int end) {
    vector<Entry> a(1 << (end-begin));
    a[0].effect=0; a[0].mask=0; a[0].count=0;
    for (int j=0;j<end-begin;++j) {
        for (int m=0;m<(1<<j);++m) {
            Entry e=a[m];
            e.effect ^= effect[begin+j];
            e.mask |= 1u << (begin+j);
            ++e.count;
            a[m+(1<<j)]=e;
        }
    }
    return a;
}
int main() {
    int r,c,tc=0;
    while (cin>>r>>c && (r||c)) {
        string p[3]; for (int i=0;i<3;++i) cin>>p[i];
        int n=r*c, half=n/2;
        vector<unsigned> effect(n,0);
        for (int i=0;i<r;++i) for (int j=0;j<c;++j)
            for (int u=0;u<3;++u) for (int v=0;v<3;++v) {
                int x=i+u-1,y=j+v-1;
                if (p[u][v]=='*' && x>=0 && x<r && y>=0 && y<c)
                    effect[i*c+j] |= 1u << (x*c+y);
            }
        vector<Entry> left=subsets(effect,0,half);
        vector<Entry> right=subsets(effect,half,n);
        sort(left.begin(),left.end());
        int best=n+1; unsigned answer=0, target=(1u<<n)-1;
        for (unsigned i=0;i<right.size();++i) {
            unsigned want=target^right[i].effect;
            int lo=0,hi=left.size();
            while (lo<hi) {
                int mid=(lo+hi)/2;
                if (left[mid].effect<want) lo=mid+1; else hi=mid;
            }
            if (lo==(int)left.size() || left[lo].effect!=want) continue;
            int count=left[lo].count+right[i].count;
            unsigned mask=left[lo].mask|right[i].mask;
            if (count<best || (count==best && mask<answer)) {
                best=count; answer=mask;
            }
        }
        cout<<"Case #"<<++tc<<'\n';
        if (best>n) cout<<"Impossible.\n";
        else {
            bool first=true;
            for (int i=0;i<n;++i) if (answer&(1u<<i)) {
                if (!first) cout<<' ';
                cout<<i+1; first=false;
            }
            cout<<'\n';
        }
    }
    return 0;
}
