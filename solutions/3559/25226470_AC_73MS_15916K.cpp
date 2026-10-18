// POJ 3559 - Trigonometric optimization
// Model: gpt-6-astra
// Fix t=y+z. Sum-to-product makes the last two terms a coefficient
// times cos((y-z)/2-offset), where offset is 0 or +/-pi/4.
// For each parity, the legal differences grow by two endpoints as t grows.
// Keep their minimum and maximum cosine and evaluate both witnesses.
// Integer-angle sine/cosine tables use a recurrence reanchored every 128
// entries; reevaluate the winning witness with direct trigonometric calls.
// Positive integer variables and radian angles. O(S) time and memory.
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    string f[3], direction;
    int S;
    if (!(cin >> f[0] >> f[1] >> f[2] >> direction >> S)) return 0;
    int k[3];
    for(int i=0;i<3;++i) k[i]=(f[i]=="sin");
    int sign=direction=="max" ? 1 : -1;
    vector<double> sn(S+1),cs(S+1);
    double s1=sin(1.0),c1=cos(1.0);
    for(int i=0;i<=S;++i) {
        if(i%128==0) { sn[i]=sin((double)i); cs[i]=cos((double)i); }
        else { sn[i]=sn[i-1]*c1+cs[i-1]*s1; cs[i]=cs[i-1]*c1-sn[i-1]*s1; }
    }
    double co=1,so=0;
    if(k[1]!=k[2]) { co=sqrt(0.5); so=k[1] ? co : -co; }
    double mn[2]={2,2},mx[2]={-2,-2};
    int dlo[2]={0,0},dhi[2]={0,0};
    double ch=cos(0.5),sh=sin(0.5),best=-4;
    int bx=1,by=1,bz=1;
    for(int t=2;t<S;++t) {
        int p=t%2,n=(t-2)/2,d=t-2;
        double c=cs[n],s=sn[n];
        if(p) { c=cs[n]*ch-sn[n]*sh; s=sn[n]*ch+cs[n]*sh; }
        for(int side=-1;side<=1;side+=2) {
            double v=c*co+side*s*so;
            if(v<mn[p]) { mn[p]=v; dlo[p]=side*d; }
            if(v>mx[p]) { mx[p]=v; dhi[p]=side*d; }
        }
        int ds[2]={dlo[p],dhi[p]},x=S-t;
        for(int j=0;j<2;++j) {
            int y=(t+ds[j])/2,z=t-y;
            double cur=sign*((k[0]?sn[x]:cs[x])+(k[1]?sn[y]:cs[y])+(k[2]?sn[z]:cs[z]));
            if(cur>best) { best=cur; bx=x; by=y; bz=z; }
        }
    }
    double ans=(k[0]?sin((double)bx):cos((double)bx))
              +(k[1]?sin((double)by):cos((double)by))
              +(k[2]?sin((double)bz):cos((double)bz));
    printf("%.15f\n",ans);
    return 0;
}
