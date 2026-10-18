// POJ 3780 - Tour Route
// Model: gpt-6-astra
// Build a Hamiltonian path of the tournament by insertion, then grow a
// directed cycle by splicing prefixes of the remaining path. O(n^2).
// A singleton is the trivial tour; other routes need a closing edge.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
static unsigned char a[1005][1005];
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        for (int i=0;i<n;++i) for(int j=0;j<n;++j) {
            int x; scanf("%d", &x); a[i][j]=(unsigned char)x;
        }
        if(n==1) { puts("1"); continue; }
        vector<int> p;
        for(int v=0;v<n;++v) {
            int k=0;
            while(k<(int)p.size() && a[p[k]][v]) ++k;
            p.insert(p.begin()+k,v);
        }
        int end=-1;
        for(int k=2;k<n;++k) if(a[p[k]][p[0]]) end=k;
        if(end<0) { puts("-1"); continue; }
        vector<int> c(p.begin(),p.begin()+end+1);
        int first=end+1;
        bool ok=true;
        while(first<n) {
            int last=first, at=-1;
            for(;last<n && at<0;++last) {
                for(int k=0;k<(int)c.size();++k) {
                    int prev=(k+(int)c.size()-1)%(int)c.size();
                    if(a[c[prev]][p[first]] && a[p[last]][c[k]]) { at=k; break; }
                }
            }
            if(at<0) { ok=false; break; }
            c.insert(c.begin()+at,p.begin()+first,p.begin()+last);
            first=last;
        }
        if(!ok) { puts("-1"); continue; }
        int start=(int)(find(c.begin(),c.end(),0)-c.begin());
        for(int k=0;k<n;++k) printf("%d%c",c[(start+k)%n]+1,k+1==n?'\n':' ');
    }
    return 0;
}
