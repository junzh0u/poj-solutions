// POJ 2702 - Can a Complicated Program Go Wrong?
// Model: gpt-6.1-sol
// Search the reachable product of program and requirement states.
// An action absent from the requirement leaves its state unchanged, as in
// the sample. All listed transitions are retained, including nondeterminism.
#include <cstdio>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;
struct Edge { int c, v; Edge(int a,int b):c(a),v(b){} };
vector<Edge> p[502];
vector<int> r[502][26];
bool seen[502][502];
int main() {
    int tests; scanf("%d", &tests);
    while (tests--) {
        for(int x=0;x<502;++x) {
            p[x].clear();
            for(int c=0;c<26;++c) r[x][c].clear();
        }
        int s,e,startP,startR,a,b; char c;
        scanf("%d%d%d",&s,&e,&startP); ++startP;
        for(int j=0;j<e;++j) {
            scanf("%d %c %d",&a,&c,&b);
            p[a+1].push_back(Edge(c-'a',b+1));
        }
        scanf("%d%d%d",&s,&e,&startR); ++startR;
        for(int j=0;j<e;++j) {
            scanf("%d %c %d",&a,&c,&b);
            r[a+1][c-'a'].push_back(b+1);
        }
        memset(seen,0,sizeof(seen));
        queue<int> q; q.push(startP*502+startR);
        seen[startP][startR]=true;
        bool bad=startR==0;
        while(!q.empty() && !bad) {
            int x=q.front()/502,y=q.front()%502; q.pop();
            for(unsigned j=0;j<p[x].size() && !bad;++j) {
                Edge z=p[x][j];
                vector<int>& dest=r[y][z.c];
                unsigned count=dest.empty()?1:dest.size();
                for(unsigned k=0;k<count;++k) {
                    int v=dest.empty()?y:dest[k];
                    if(v==0) {bad=true;break;}
                    if(!seen[z.v][v]) {
                        seen[z.v][v]=true; q.push(z.v*502+v);
                    }
                }
            }
        }
        puts(bad?"violated":"satisfied");
    }
    return 0;
}
