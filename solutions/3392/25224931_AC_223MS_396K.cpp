// POJ 3392 - Pitcher Rotation
// Model: gpt-6.1-sol
// Keep the nine best pitchers for each opponent. A day's pitcher conflicts
// with at most eight other days (four before and four after), so any choice
// outside this list can be replaced without decreasing the total score.
// DP records candidate indices on the preceding four calendar days.
// Four full rest days mean starts must be at least five days apart.
// Unlike the top-five heuristic discussed on the board, this pruning is exact.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n,m,g; scanf("%d%d%d",&n,&m,&g);
        vector< vector< pair<int,int> > > p(m+1);
        for(int i=1;i<=m;++i) {
            for(int j=0;j<n;++j) {int x;scanf("%d",&x);p[i].push_back(make_pair(x,j));}
            sort(p[i].begin(),p[i].end(),greater< pair<int,int> >());
            if(n>9) p[i].resize(9);
        }
        p[0].push_back(make_pair(0,-1));
        vector< vector< pair<int,int> > > days(g+14,p[0]);
        for(int i=4;i<g+14;++i) {int x;scanf("%d",&x);days[i]=p[x];}
        int a[6561], b[6561];
        fill(a,a+6561,-1);a[0]=0;
        for(int day=4;day<g+14;++day) {
            fill(b,b+6561,-1);
            for(int s=0;s<6561;++s) if(a[s]>=0) {
                int old[4],q=s;
                for(int k=0;k<4;++k) {old[k]=days[day-1-k][q%9].second;q/=9;}
                for(int k=0;k<(int)days[day].size();++k) {
                    int id=days[day][k].second;
                    bool ok=true;
                    for(int j=0;j<4;++j) if(id>=0 && id==old[j]) ok=false;
                    if(ok) {int ns=(s%729)*9+k;b[ns]=max(b[ns],a[s]+days[day][k].first);}
                }
            }
            copy(b,b+6561,a);
        }
        int ans=*max_element(a,a+6561);
        printf("%d.%02d\n",ans/100,ans%100);
    }
    return 0;
}
