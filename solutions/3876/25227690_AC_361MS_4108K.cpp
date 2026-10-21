// POJ 3876 - Darts
// Model: gpt-6-astra
// A-turn and B-turn winning probabilities for A at each pair of scores.
// All successful throws reduce a score; eliminate the two-turn overshoot
// cycle algebraically for each B target, then choose B's optimal target.
// Board adjacency follows the figure. Each turn consists of one dart.
#include <cstdio>
#include <algorithm>
using namespace std;
double aturn[502][502], bturn[502][502];
const int board[20]={20,1,18,4,13,6,10,15,2,17,3,19,7,16,8,11,14,9,12,5};
int main(){
    for(int b=1;b<=501;++b) aturn[0][b]=bturn[0][b]=1;
    for(int a=1;a<=501;++a) for(int b=1;b<=501;++b){
        double c=0, q=max(20-a,0)/20.0;
        for(int s=1;s<=20 && s<=a;++s) c+=bturn[a-s][b]/20.0;
        double best=1;
        for(int k=0;k<20;++k){
            double d=0,r=0;
            for(int t=-1;t<=1;++t){
                int s=board[(k+t+20)%20];
                if(s>b) r+=1.0/3;
                else d+=aturn[a][b-s]/3;
            }
            best=min(best,(c+q*d)/(1-q*r));
        }
        aturn[a][b]=best;
        best=1;
        for(int k=0;k<20;++k){
            double p=0;
            for(int t=-1;t<=1;++t){
                int s=board[(k+t+20)%20];
                p+=(s>b ? aturn[a][b] : aturn[a][b-s])/3;
            }
            best=min(best,p);
        }
        bturn[a][b]=best;
    }
    int n;
    while(scanf("%d",&n)==1 && n) printf("%.12f %.12f\n",aturn[n][n],1-bturn[n][n]);
    return 0;
}
