// POJ 3998 - Land Division
// Model: gpt-6.1-sol
// Group equal coordinates on each axis, then partition their prefix sums.
// Scale each region's unfairness by K to keep all arithmetic integral.
// The absolute-value transition is split at K*(prefix[i]-prefix[j])=N;
// a running minimum and a monotone queue evaluate each DP layer linearly.
// Empty strips are allowed: several cuts can lie in the same city-free gap.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
const int INF=100000000;
int solve(vector<int> a,int n,int k) {
    sort(a.begin(),a.end());
    vector<int> p(1,0);
    for(int i=0;i<n;) {
        int j=i+1; while(j<n && a[j]==a[i]) ++j;
        p.push_back(j); i=j;
    }
    int m=(int)p.size()-1;
    vector<int> prev(m+1,INF),cur(m+1),q(m+1);
    prev[0]=0;
    for(int t=1;t<=k;++t) {
        int left=0,head=0,tail=0,best=INF;
        for(int i=0;i<=m;++i) {
            int value=prev[i]+k*p[i];
            while(tail>head && prev[q[tail-1]]+k*p[q[tail-1]]>=value) --tail;
            q[tail++]=i;
            while(left<=i && k*(p[i]-p[left])>=n) {
                best=min(best,prev[left]-k*p[left]); ++left;
            }
            while(head<tail && q[head]<left) ++head;
            cur[i]=INF;
            if(best<INF) cur[i]=best+k*p[i]-n;
            if(head<tail) cur[i]=min(cur[i],prev[q[head]]+k*p[q[head]]+n-k*p[i]);
        }
        prev.swap(cur);
    }
    return prev[m];
}
int gcd(int a,int b) {return b?gcd(b,a%b):a;}
int main() {
    int n,k,tc=0;
    while(scanf("%d%d",&n,&k)==2 && n) {
        vector<int>x(n),y(n);
        for(int i=0;i<n;++i) scanf("%d%d",&x[i],&y[i]);
        int a=min(solve(x,n,k),solve(y,n,k)),b=k*k,g=gcd(a,b);
        printf("%d. %d/%d\n",++tc,a/g,b/g);
    }
    return 0;
}
