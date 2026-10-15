// POJ 3738 - Card Game
// Model: gpt-6.1-sol
// Dynamic programming on removed-card masks, base rank and refreshes left.
// A card is exposed after both children are removed. Refreshes are uniform
// over all 13 ranks, and are forced when no move exists and chances remain.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
int main() {
    int n,m;
    while (scanf("%d%d",&n,&m)==2 && n) {
        int k=n*(n+1)/2, full=(1<<k)-1;
        int a[15],child[15]={0};
        for(int i=0;i<k;++i) {scanf("%d",&a[i]); --a[i];}
        int p,q,x; scanf("%d%d%d",&p,&q,&x); --x;
        for(int r=0;r<n-1;++r) for(int c=0;c<=r;++c) {
            int i=r*(r+1)/2+c, j=(r+1)*(r+2)/2+c;
            child[i]=(1<<j)|(1<<(j+1));
        }
        vector<int> states, exposed(1<<k,0);
        vector<char> seen(1<<k,0); seen[0]=1;
        for(int s=0;s<=full;++s) if(seen[s]) {
            states.push_back(s);
            for(int i=0;i<k;++i) if(!(s&(1<<i)) && (s&child[i])==child[i]) {
                exposed[s]|=1<<i; seen[s|(1<<i)]=1;
            }
        }
        vector<double> prev((1<<k)*13,0),cur((1<<k)*13,0);
        for(int t=0;t<=m;++t) {
            for(int z=(int)states.size()-1;z>=0;--z) {
                int s=states[z];
                double refresh=0;
                if(t && s!=full) {
                    for(int b=0;b<13;++b) refresh+=prev[s*13+b];
                    refresh=refresh/13-q;
                }
                for(int b=0;b<13;++b) {
                    double best=t ? refresh : 0; bool move=false;
                    for(int i=0;i<k;++i) if(exposed[s]&(1<<i)) {
                        int d=abs(a[i]-b);
                        if(d==1 || d==12) {
                            double v=p+cur[(s|(1<<i))*13+a[i]];
                            if(!move && !t) best=v;
                            else best=max(best,v);
                            move=true;
                        }
                    }
                    cur[s*13+b]=best;
                }
            }
            if(t==m) printf("%.2f\n",cur[x]+0.0);
            prev.swap(cur);
        }
    }
    return 0;
}
