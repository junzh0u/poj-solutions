// POJ 1235 - Galactic Breakup
// Model: gpt-6.1-sol
// Restore monarchies in reverse secession order, maintaining connected
// components with disjoint sets. Numbering is x + n*y + n*m*z, as in
// the figure. Empty and fully restored empires are not disconnected.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int parent[27000], sz[27000];
bool active[27000];
int root(int a) {
    if (parent[a] != a) parent[a] = root(parent[a]);
    return parent[a];
}
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int n,m,k,l;
        scanf("%d%d%d%d", &n,&m,&k,&l);
        int total=n*m*k;
        vector< vector<int> > groups(l);
        for (int i=0;i<l;++i) {
            int p; scanf("%d", &p);
            groups[i].resize(p);
            for (int j=0;j<p;++j) scanf("%d", &groups[i][j]);
        }
        fill(active,active+total,false);
        int components=0,answer=0;
        for (int i=l-1;i>=0;--i) {
            for (int j=0;j<(int)groups[i].size();++j) {
                int a=groups[i][j];
                active[a]=true; parent[a]=a; sz[a]=1; ++components;
                int x=a%n,y=a/n%m,z=a/(n*m);
                int adj[6]={x? a-1:-1,x+1<n? a+1:-1,
                            y? a-n:-1,y+1<m? a+n:-1,
                            z? a-n*m:-1,z+1<k? a+n*m:-1};
                for (int d=0;d<6;++d) {
                    int b=adj[d];
                    if (b<0 || !active[b]) continue;
                    int ra=root(a),rb=root(b);
                    if (ra==rb) continue;
                    if (sz[ra]<sz[rb]) swap(ra,rb);
                    parent[rb]=ra; sz[ra]+=sz[rb]; --components;
                }
            }
            if (components>1) ++answer;
        }
        printf("%d\n",answer);
    }
    return 0;
}
