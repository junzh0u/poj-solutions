// POJ 1834 - 线段处理
// Model: gpt-6.1-sol
// Count strict endpoint-order inversions separately in each vertical strip.
// Rational heights use exact cross products; equal heights at either boundary
// are excluded because those intersections are endpoints of the cut segments.
// Normalize reversed endpoints. All products and the pair count fit signed int.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Segment { int x, y, z, w; };
struct Part { int l, r, d, rank; };
bool leftLess(const Part &a, const Part &b) { return a.l*b.d < b.l*a.d; }
bool rightLess(const Part &a, const Part &b) { return a.r*b.d < b.r*a.d; }
int main() {
    int cases; scanf("%d", &cases);
    while (cases--) {
        int n, m; scanf("%d%d", &n, &m);
        vector<Segment> s(m);
        for (int i=0;i<m;++i) {
            scanf("%d%d%d%d", &s[i].x,&s[i].y,&s[i].z,&s[i].w);
            if(s[i].x>s[i].z) { swap(s[i].x,s[i].z); swap(s[i].y,s[i].w); }
        }
        int answer=0;
        for(int k=0;k<n-1;++k) {
            vector<Part> a;
            for(int i=0;i<m;++i) if(s[i].x<=k && s[i].z>k) {
                Part p; p.d=s[i].z-s[i].x;
                p.l=s[i].y*(s[i].z-k)+s[i].w*(k-s[i].x);
                p.r=s[i].y*(s[i].z-k-1)+s[i].w*(k+1-s[i].x);
                p.rank=0; a.push_back(p);
            }
            sort(a.begin(),a.end(),rightLess);
            int ranks=0;
            for(int i=0;i<(int)a.size();++i) {
                if(i==0 || rightLess(a[i-1],a[i])) ++ranks;
                a[i].rank=ranks;
            }
            sort(a.begin(),a.end(),leftLess);
            vector<int> bit(ranks+1,0);
            for(int i=0;i<(int)a.size();) {
                int j=i+1;
                while(j<(int)a.size() && !leftLess(a[i],a[j])) ++j;
                for(int t=i;t<j;++t) {
                    int count=0;
                    for(int x=a[t].rank;x>0;x-=x&-x) count+=bit[x];
                    answer+=i-count;
                }
                for(int t=i;t<j;++t)
                    for(int x=a[t].rank;x<=ranks;x+=x&-x) ++bit[x];
                i=j;
            }
        }
        printf("%d\n",answer);
    }
    return 0;
}
