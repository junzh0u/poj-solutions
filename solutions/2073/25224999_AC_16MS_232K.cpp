// POJ 2073 - Doggone Moles
// Model: gpt-6.1-sol
// Maintain all surviving positions. After each terrier move, remove
// newly threatened positions before expanding by one mole move, then
// remove threatened destinations. Coordinates include both W and L.
#include <cstdio>
#include <vector>
#include <utility>
using namespace std;
int main() {
    int W,L,N,T,cs=0;
    const int dx[5]={0,1,-1,0,0},dy[5]={0,0,0,1,-1};
    while(scanf("%d%d%d%d",&W,&L,&N,&T)==4 && (W||L||N||T)) {
        vector< vector< pair<int,int> > > dogs(T);
        for(int i=0;i<N;++i) for(int t=0;t<T;++t) {
            int x,y; scanf("%d%d",&x,&y);
            dogs[t].push_back(make_pair(x,y));
        }
        int stride=W+1,sz=stride*(L+1);
        vector<unsigned char> cur(sz,1),next(sz),bad(sz);
        for(int t=0;t<T;++t) {
            fill(bad.begin(),bad.end(),0);
            for(int i=0;i<N;++i) for(int d=0;d<5;++d) {
                int x=dogs[t][i].first+dx[d],y=dogs[t][i].second+dy[d];
                if(x>=0&&x<=W&&y>=0&&y<=L) bad[y*stride+x]=1;
            }
            for(int p=0;p<sz;++p) if(bad[p]) cur[p]=0;
            if(t==0) continue;
            fill(next.begin(),next.end(),0);
            for(int y=0;y<=L;++y) for(int x=0;x<=W;++x) if(cur[y*stride+x]) {
                for(int d=0;d<5;++d) {
                    int a=x+dx[d],b=y+dy[d];
                    if(a>=0&&a<=W&&b>=0&&b<=L&&!bad[b*stride+a]) next[b*stride+a]=1;
                }
            }
            cur.swap(next);
        }
        printf("Observation Set %d\n",++cs);
        int count=0;
        for(int y=0;y<=L;++y) for(int x=0;x<=W;++x) if(cur[y*stride+x]) {
            if(count%8) putchar(' ');
            printf("(%d,%d)",x,y);
            if(++count%8==0) putchar('\n');
        }
        if(!count) puts("No possible locations");
        else if(count%8) putchar('\n');
    }
    return 0;
}
