// POJ 1626 - Chapayev
// Model: gpt-6.1-sol
// Enumerate angular cells and their tangent boundaries for every shooter.
// Memoized minimax uses the surviving-piece mask and the side to move.
// A shot removes its own piece even if it hits nothing; touching counts.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
double x[16], y[16];
vector<int> shot[16];
signed char memo[2][65536];
const double PI=acos(-1.0);
double norm(double a) { while(a<0) a+=2*PI; while(a>=2*PI) a-=2*PI; return a; }
void prepare() {
    for(int i=0;i<16;++i) {
        vector<double> a;
        for(int j=0;j<16;++j) if(i!=j) {
            double dx=x[j]-x[i], dy=y[j]-y[i];
            double t=atan2(dy,dx), w=asin(0.8/sqrt(dx*dx+dy*dy));
            a.push_back(norm(t-w)); a.push_back(norm(t+w));
        }
        sort(a.begin(),a.end());
        for(int k=0;k<(int)a.size();++k) for(int edge=0;edge<2;++edge) {
            double b=a[k];
            if(!edge) b=(b+(k+1==(int)a.size()?a[0]+2*PI:a[k+1]))/2;
            double ux=cos(b), uy=sin(b);
            int kill=1<<i;
            for(int j=0;j<16;++j) if(i!=j) {
                double dx=x[j]-x[i], dy=y[j]-y[i];
                if(dx*ux+dy*uy>0 && fabs(dx*uy-dy*ux)<=0.8+1e-10) kill|=1<<j;
            }
            shot[i].push_back(kill);
        }
        sort(shot[i].begin(),shot[i].end());
        shot[i].erase(unique(shot[i].begin(),shot[i].end()),shot[i].end());
    }
}
int win(int mask,int side) {
    signed char &v=memo[side][mask];
    if(v!=-1) return v;
    int own=mask&(side?65280:255);
    if(!own) return v=0;
    for(int i=side*8;i<side*8+8;++i) if(own&(1<<i))
        for(int j=0;j<(int)shot[i].size();++j)
            if(!win(mask&~shot[i][j],side^1)) return v=1;
    return v=0;
}
int main() {
    for(int i=0;i<16;++i) if(scanf("%lf%lf",&x[i],&y[i])!=2) return 0;
    prepare(); memset(memo,-1,sizeof(memo));
    puts(win(65535,0)?"RED":"WHITE");
    return 0;
}
