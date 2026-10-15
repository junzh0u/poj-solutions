// POJ 2344 - Nikifor
// Model: gpt-6.1-sol
// Greedy minimum-weight basis of the vector matroid, ordered by price then id.
// Reduce each candidate against a Gaussian basis; choose its largest residual
// coordinate as pivot. Equal-price id order gives the lexicographically first
// optimum, whose ids are printed sorted. The original Timus statement has n<=50;
// storage also accommodates the larger dimension bound printed on POJ.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct Item { int cost, id; };
bool lessItem(const Item& a,const Item& b) {
    return a.cost!=b.cost ? a.cost<b.cost : a.id<b.id;
}
int main() {
    int m,n;
    if(scanf("%d%d",&m,&n)!=2) return 0;
    vector< vector<double> > a(m,vector<double>(n));
    for(int i=0;i<m;++i) for(int j=0;j<n;++j) scanf("%lf",&a[i][j]);
    vector<Item> order(m);
    for(int i=0;i<m;++i) { scanf("%d",&order[i].cost); order[i].id=i; }
    sort(order.begin(),order.end(),lessItem);
    vector<int> pivots, rows, answer;
    int total=0;
    for(int t=0;t<m && (int)answer.size()<n;++t) {
        int r=order[t].id;
        for(int k=0;k<(int)rows.size();++k) {
            int p=pivots[k]; double factor=a[r][p];
            for(int j=0;j<n;++j) a[r][j]-=factor*a[rows[k]][j];
            a[r][p]=0;
        }
        int p=0;
        for(int j=1;j<n;++j) if(fabs(a[r][j])>fabs(a[r][p])) p=j;
        if(fabs(a[r][p])<1e-8) continue;
        double d=a[r][p];
        for(int j=0;j<n;++j) a[r][j]/=d;
        a[r][p]=1;
        pivots.push_back(p); rows.push_back(r); answer.push_back(r+1);
        total+=order[t].cost;
    }
    if((int)answer.size()!=n) { puts("0"); return 0; }
    sort(answer.begin(),answer.end());
    printf("%d\n",total);
    for(int i=0;i<n;++i) printf("%d\n",answer[i]);
    return 0;
}
