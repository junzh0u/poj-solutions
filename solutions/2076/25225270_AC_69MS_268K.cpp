// POJ 2076 - All Roads Lead to Albuquerque, er, Rome
// Model: gpt-6.1-sol
// Floyd-Warshall, then enumerate hub pairs. With k nodes at the first
// hub, minimize the sum of hub legs by taking the k-1 smallest distance
// differences. Total unordered-pair cost is (n-1)*legs+k*(n-k)*hubdist.
// The hint overrides the arbitrary-answer prose: choose the smallest
// output sequence on every tie, including equal distance differences.
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;
int d[105][105];
int main() {
    int cases; if (scanf("%d", &cases)!=1) return 0;
    while(cases--) {
        int n,m; scanf("%d%d",&n,&m);
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) d[i][j]=i==j?0:100000000;
        for(int z=0;z<m;++z) {int a,b,w; scanf("%d%d%d",&a,&b,&w); --a;--b; d[a][b]=d[b][a]=min(d[a][b],w);}
        for(int k=0;k<n;++k) for(int i=0;i<n;++i) for(int j=0;j<n;++j) d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
        int best=2000000000;
        vector<int> answer;
        for(int a=0;a<n;++a) for(int b=a+1;b<n;++b) {
            vector<pair<int,int> > diff;
            int legs=0;
            vector<int> now(n,b+1); now[a]=now[b]=0;
            for(int i=0;i<n;++i) if(i!=a && i!=b) {legs+=d[i][b]; diff.push_back(make_pair(d[i][a]-d[i][b],i));}
            sort(diff.begin(),diff.end());
            for(int k=1;k<n;++k) {
                int cost=(n-1)*legs+k*(n-k)*d[a][b];
                if(cost<best || (cost==best && now<answer)) {best=cost; answer=now;}
                if(k<n-1) {legs+=diff[k-1].first; now[diff[k-1].second]=a+1;}
            }
        }
        for(int i=0;i<n;++i) printf("%s%d",i?" ":"",answer[i]);
        printf("\n");
    }
    return 0;
}
