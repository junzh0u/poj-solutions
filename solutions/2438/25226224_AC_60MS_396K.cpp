// POJ 2438 - Children's Dining
// Model: gpt-6-astra
// Extend a path at both ends, close it by reversing a suffix, then
// break the cycle at a neighbor of an unused child and extend again.
// Every child has at least N/2 friends, so the closure always exists.
// A maximal path has more than N/2 vertices; every unused child has
// a friend on its cycle. Each extension/closure costs O(N), O(N^2) total.
// The output is a circular permutation, with no repeated first child.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
static bool enemy[401][401], used[401];
int main() {
    int n,m;
    while (scanf("%d%d", &n,&m)==2 && (n || m)) {
        int N=2*n;
        for(int i=1;i<=N;++i) {
            used[i]=false;
            for(int j=1;j<=N;++j) enemy[i][j]=(i==j);
        }
        for(int i=0,a,b;i<m;++i) {
            scanf("%d%d",&a,&b); enemy[a][b]=enemy[b][a]=true;
        }
        vector<int> p(1,1); used[1]=true;
        for(;;) {
            for(int side=0;side<2;++side) {
                for(;;) {
                    int v=1;
                    while(v<=N && (used[v] || enemy[p.back()][v])) ++v;
                    if(v>N) break;
                    p.push_back(v); used[v]=true;
                }
                reverse(p.begin(),p.end());
            }
            int k=p.size();
            if(enemy[p[0]][p[k-1]]) {
                int i=0;
                while(i+1<k && (enemy[p[i]][p[k-1]] || enemy[p[i+1]][p[0]])) ++i;
                reverse(p.begin()+i+1,p.end());
            }
            if(k==N) break;
            int v=1;
            while(used[v]) ++v;
            int i=0;
            while(enemy[v][p[i]]) ++i;
            rotate(p.begin(),p.begin()+i,p.end());
            p.push_back(v);
            // v must adjoin p[0], so reverse the cycle traversal first.
            reverse(p.begin(),p.end()-1);
            used[v]=true;
        }
        for(int i=0;i<N;++i) printf("%d%c",p[i],i+1==N?'\n':' ');
    }
    return 0;
}
