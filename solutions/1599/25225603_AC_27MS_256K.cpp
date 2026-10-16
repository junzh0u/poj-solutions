// POJ 1599 - Station Balance
// Model: gpt-6-astra
// Pad with zero-mass empty slots and enumerate every perfect matching.
// Compare C times the imbalance using exact integers; at most 945 matchings.
// Follow the sample and discussion: leading space, and no '=' in IMBALANCE.
#include <cstdio>
#include <cstdlib>
#include <climits>
int c,s,a[10],total,best,cur[5][2],ans[5][2];
void search(int mask,int k,int cost) {
    if(k==c) {
        if(cost<best) {
            best=cost;
            for(int i=0;i<c;++i) for(int j=0;j<2;++j) ans[i][j]=cur[i][j];
        }
        return;
    }
    int i=0; while(mask&(1<<i)) ++i;
    for(int j=i+1;j<2*c;++j) if(!(mask&(1<<j))) {
        cur[k][0]=a[i]; cur[k][1]=a[j];
        search(mask|(1<<i)|(1<<j),k+1,cost+abs(c*(a[i]+a[j])-total));
    }
}
int main() {
    int tc=0;
    while(scanf("%d%d",&c,&s)==2) {
        total=0;
        for(int i=0;i<2*c;++i) a[i]=0;
        for(int i=0;i<s;++i) { scanf("%d",&a[i]); total+=a[i]; }
        best=INT_MAX; search(0,0,0);
        printf("Set #%d\n",++tc);
        for(int i=0;i<c;++i) {
            printf(" %d:",i);
            for(int j=0;j<2;++j) if(ans[i][j]) printf(" %d",ans[i][j]);
            printf("\n");
        }
        printf("IMBALANCE %.5f\n\n",double(best)/c);
    }
    return 0;
}
