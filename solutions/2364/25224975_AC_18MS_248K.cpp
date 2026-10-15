// POJ 2364 - Faucet Flow
// Model: gpt-6.1-sol
// Let H be the smaller maximum barrier height on the two sides.
// Fill the central basin to H, then divide flow equally. The easier
// side spills after L units; the other side takes R units to spill or
// fill back to H. Equal side maxima take 2*min(L,R); unequal
// maxima take L + min(L,R), returning flow when the other side fills.
// Equal-height barriers split flow; a filled side returns its share.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int lx, rx;
    while (cin >> lx >> rx && (lx || rx)) {
        int n = (rx-lx)/2+1, s = (-lx-1)/2;
        vector<long long> h(n);
        for (int i=0;i<n;++i) cin >> h[i];
        long long a=0,b=0;
        for (int i=0;i<=s;++i) a=max(a,h[i]);
        for (int i=s+1;i<n;++i) b=max(b,h[i]);
        if (a>b) { reverse(h.begin(),h.end()); s=n-2-s; swap(a,b); }
        int l=s,r=s+1;
        while (h[l]<a) --l;
        while (h[r]<a) ++r;
        long long central=2*a*(r-l), left=0,right=0,peak=0;
        for (int i=0;i<l;++i) { peak=max(peak,h[i]); left+=2*peak; }
        if (a==b) {
            peak=0;
            for (int i=n-1;i>r;--i) { peak=max(peak,h[i]); right+=2*peak; }
        } else {
            int j=r;
            while (h[j]<=a) { right+=2*a; ++j; }
        }
        cout << central+(a==b ? 2*min(left,right) : left+min(left,right)) << '\n';
    }
    return 0;
}
