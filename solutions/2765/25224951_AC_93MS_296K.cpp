// POJ 2765 - Declaration of Content
// Model: gpt-6.1-sol
// Enumerate each ingredient's percentage and propagate monotone bounds.
// A percentage is feasible iff the minimum and maximum possible sums bracket
// 100. Integer monotone sequences attain every sum between these endpoints.
// Queries ask which products CAN be extremal; explicit zero percentages are
// allowed, whereas an unspecified percentage is at least one.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
bool feasible(vector<int> lo, vector<int> hi, int k, int v) {
    lo[k] = hi[k] = v;
    int n = lo.size(), sl = 0, sh = 0;
    for (int i=n-2; i>=0; --i) lo[i]=max(lo[i],lo[i+1]);
    for (int i=1; i<n; ++i) hi[i]=min(hi[i],hi[i-1]);
    for (int i=0; i<n; ++i) {
        if (lo[i]>hi[i]) return false;
        sl+=lo[i]; sh+=hi[i];
    }
    return sl<=100 && sh>=100;
}
int main() {
    int p;
    while (cin>>p && p) {
        vector<string> products(p);
        vector<map<string,pair<int,int> > > bounds(p);
        for (int t=0; t<p; ++t) {
            int n; cin>>products[t]>>n;
            string line; getline(cin,line);
            vector<string> names(n);
            vector<int> lo(n,1), hi(n,100);
            for (int i=0; i<n; ++i) {
                getline(cin,line); istringstream in(line);
                in>>names[i]; int v;
                if (in>>v) lo[i]=hi[i]=v;
            }
            for (int i=0; i<n; ++i) {
                int a=101,b=-1;
                for (int v=lo[i]; v<=hi[i]; ++v)
                    if (feasible(lo,hi,i,v)) { a=min(a,v); b=max(b,v); }
                bounds[t][names[i]]=make_pair(a,b);
            }
        }
        int q; cin>>q;
        while (q--) {
            string kind,x; cin>>kind>>x;
            vector<int> lo(p),hi(p);
            int threshold = kind=="most" ? 0 : 100;
            for (int i=0; i<p; ++i) {
                if (bounds[i].count(x)) {
                    lo[i]=bounds[i][x].first; hi[i]=bounds[i][x].second;
                }
                if (kind=="most") threshold=max(threshold,lo[i]);
                else threshold=min(threshold,hi[i]);
            }
            bool first=true;
            for (int i=0; i<p; ++i)
                if ((kind=="most" && hi[i]>=threshold) ||
                    (kind=="least" && lo[i]<=threshold)) {
                    if (!first) cout<<' ';
                    cout<<products[i]; first=false;
                }
            cout<<'\n';
        }
    }
}
