// POJ 3816 - Pool Table
// Model: gpt-6.1-sol
// Unfold reflected tables and visit target images by cushion count.
// A reduced directed slope is usable only at its first target image;
// later images on that ray would hit the target prematurely.
// A corner crosses two grid lines, hence contributes two cushions.
#include <cstdio>
#include <cmath>
#include <set>
#include <utility>
#include <cstdlib>
using namespace std;
int gcd(int a,int b) { while(b) { int r=a%b; a=b; b=r; } return a; }
int main() {
    int L,W,cx,cy,tx,ty,n;
    while(scanf("%d%d%d%d%d%d%d",&L,&W,&cx,&cy,&tx,&ty,&n)==7 && L) {
        set<pair<int,int> > seen;
        int best=2147483647;
        for(int k=0;k<=n;++k) for(int i=-k;i<=k;++i) {
            int a=k-abs(i);
            for(int s=0;s<(a?2:1);++s) {
                int j=s?-a:a;
                int x=i*L+(i%2?L-tx:tx)-cx;
                int y=j*W+(j%2?W-ty:ty)-cy;
                int g=gcd(abs(x),abs(y));
                pair<int,int> direction(x/g,y/g);
                if(seen.insert(direction).second && k==n) {
                    int d=x*x+y*y;
                    if(d<best) best=d;
                }
            }
        }
        printf("%.3f\n",sqrt((double)best));
    }
    return 0;
}
