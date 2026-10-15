// POJ 1533 - Mystery
// Model: gpt-6.1-sol
// Emit the smallest safe integer. A nonfinal copy is always safe; a final
// copy must wait until all required predecessors have appeared. On the
// first occurrence, remove its outgoing requirements. Pairs mean some
// earlier x and later y, not adjacency or an ordering of all copies.
// Lexicographic comparison uses integer values, as the discussion confirms.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
int main() {
    int m,d;
    while (scanf("%d%d",&m,&d)==2 && d) {
        int cnt[100], in[100]; bool edge[100][100], seen[100];
        memset(in,0,sizeof(in)); memset(edge,0,sizeof(edge));
        memset(seen,0,sizeof(seen));
        for(int i=0;i<m;++i) scanf("%d",&cnt[i]);
        bool ok=true;
        for(int i=0;i<d;++i) {
            int x,y; scanf("%d%d",&x,&y);
            if(x<0 || x>=m || y<0 || y>=m) {ok=false; continue;}
            if(cnt[x]==0 || cnt[y]==0) ok=false;
            if(!edge[x][y]) {edge[x][y]=true; ++in[y];}
        }
        vector<int> ans;
        while(ok) {
            int x=-1; bool remaining=false;
            for(int i=0;i<m;++i) {
                if(cnt[i]>0) remaining=true;
                if(cnt[i]>0 && (cnt[i]>1 || in[i]==0)) {x=i; break;}
            }
            if(x==-1) {if(remaining) ok=false; break;}
            ans.push_back(x); --cnt[x];
            if(!seen[x]) {
                seen[x]=true;
                for(int j=0;j<m;++j) if(edge[x][j]) --in[j];
            }
        }
        if(!ok) puts("Impossible!");
        else {
            for(unsigned i=0;i<ans.size();++i) printf(i?" %d":"%d",ans[i]);
            puts("");
        }
    }
    return 0;
}
