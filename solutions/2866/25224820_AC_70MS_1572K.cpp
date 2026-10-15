// POJ 2866 - Optical Fiber
// Model: gpt-6.1-sol
// Root the prescribed tree. For each candidate router site, sum the
// minimum child-subtree cost plus the Euclidean link to that child.
// A city uses one shared router site for all its incident links.
#include <cstdio>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { double x,y; };
vector<Point> site[1000];
vector<int> edge[1000];
double dp[1000][50];
int parentNode[1000], order[1000];
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        map<string,int> id;
        char name[20], other[20];
        for (int u=0;u<n;++u) {
            int c; scanf("%s%d",name,&c); id[name]=u;
            site[u].resize(c); edge[u].clear();
            for(int j=0;j<c;++j) scanf("%lf%lf",&site[u][j].x,&site[u][j].y);
        }
        for(int i=1;i<n;++i) {
            scanf("%s%s",name,other);
            int u=id[name],v=id[other];
            edge[u].push_back(v); edge[v].push_back(u);
        }
        int size=1; order[0]=0; parentNode[0]=-1;
        for(int k=0;k<size;++k) {
            int u=order[k];
            for(int j=0;j<(int)edge[u].size();++j) {
                int v=edge[u][j]; if(v==parentNode[u]) continue;
                parentNode[v]=u; order[size++]=v;
            }
        }
        for(int k=n-1;k>=0;--k) {
            int u=order[k];
            for(int i=0;i<(int)site[u].size();++i) {
                dp[u][i]=0;
                for(int e=0;e<(int)edge[u].size();++e) {
                    int v=edge[u][e]; if(v==parentNode[u]) continue;
                    double best=1e100;
                    for(int j=0;j<(int)site[v].size();++j) {
                        double dx=site[u][i].x-site[v][j].x;
                        double dy=site[u][i].y-site[v][j].y;
                        best=min(best,dp[v][j]+sqrt(dx*dx+dy*dy));
                    }
                    dp[u][i]+=best;
                }
            }
        }
        printf("%.1f\n",*min_element(dp[0],dp[0]+site[0].size()));
    }
    return 0;
}
