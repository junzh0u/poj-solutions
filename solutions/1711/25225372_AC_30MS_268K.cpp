// POJ 1711 - Puncher
// Model: gpt-6.1-sol
// Normalize translations by the occupied bounding rectangle. Count its
// full-boundary subsets by Burnside and inclusion-exclusion on empty sides.
// Rectangles use four symmetries; squares use all eight. Rotated bounding
// dimensions are counted once, provided either orientation fits the form.
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long Int;
Int countBox(int h,int w) {
    Int sum=0; int groups=(h==w?8:4);
    for(int g=0;g<groups;++g) {
        int p[32], sides[32], nc=0; bool seen[32]={false};
        for(int r=0;r<h;++r) for(int c=0;c<w;++c) {
            int x=r,y=c;
            if(g<4) { if(g&1)x=h-1-x; if(g&2)y=w-1-y; }
            else { x=c;y=r; if(g&1)x=h-1-x; if(g&2)y=w-1-y; }
            p[r*w+c]=x*w+y;
        }
        for(int i=0;i<h*w;++i) if(!seen[i]) {
            int j=i,b=0;
            do { seen[j]=true; int r=j/w,c=j%w;
                if(r==0)b|=1; if(r==h-1)b|=2;
                if(c==0)b|=4; if(c==w-1)b|=8;
                j=p[j];
            } while(j!=i);
            sides[nc++]=b;
        }
        for(int mask=0;mask<16;++mask) {
            int free=0,parity=0;
            for(int i=0;i<nc;++i) if(!(sides[i]&mask))++free;
            for(int k=0;k<4;++k) if(mask&(1<<k))parity^=1;
            sum+=(parity?-1:1)*(1LL<<free);
        }
    }
    return sum/groups;
}
int main(){int m,n;if(!(cin>>m>>n))return 0;Int ans=0;
    for(int a=1;a<=max(m,n);++a)for(int b=a;b<=max(m,n);++b)
        if((a<=m&&b<=n)||(b<=m&&a<=n))ans+=countBox(a,b);
    cout<<ans<<endl;return 0;
}
