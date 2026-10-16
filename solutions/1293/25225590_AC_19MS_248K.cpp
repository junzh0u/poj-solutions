// POJ 1293 - Duty Free Shop
// Model: gpt-6-astra
// Descending subset-sum DP records each sum's first predecessor.
// Select a sum at most M whose complement is at most L; unused chocolates
// are allowed, but every box must be filled. Only M=L=0 ends the input.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int m,l;
    while (scanf("%d%d",&m,&l)==2 && (m || l)) {
        int n; scanf("%d",&n);
        vector<int> c(n+1), prev(m+1,-1), box(m+1,-1);
        int total=0;
        for(int i=1;i<=n;++i) {
            scanf("%d",&c[i]);
            // Saturation suffices to reject excessive total capacity.
            if(total<=m+l) total += c[i];
        }
        if(total>m+l) { puts("Impossible to distribute"); continue; }
        prev[0]=0;
        for(int i=1;i<=n;++i)
            for(int s=m;s>=c[i];--s)
                if(prev[s]<0 && prev[s-c[i]]>=0) {
                    prev[s]=s-c[i]; box[s]=i;
                }
        int s=m;
        while(s>=0 && (prev[s]<0 || total-s>l)) --s;
        if(s<0) { puts("Impossible to distribute"); continue; }
        vector<int> ans;
        while(s>0) { ans.push_back(box[s]); s=prev[s]; }
        sort(ans.begin(),ans.end());
        printf("%d",(int)ans.size());
        for(int i=0;i<(int)ans.size();++i) printf(" %d",ans[i]);
        puts("");
    }
    return 0;
}
