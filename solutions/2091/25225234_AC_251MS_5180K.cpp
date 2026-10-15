// POJ 2091 - Zing Zhu's Oyster Farm
// Model: gpt-6.1-sol
// Integer-coordinate fences separate unit square cells. Keep fences whose
// height is at least the tide, then flood fill from outside the bounding box.
// Every unreached cell contributes one square meter; equal heights block water.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Fence { int x1,y1,x2,y2,h; };
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        vector<Fence> f(n);
        int loX=500,loY=500,hiX=-500,hiY=-500;
        for(int i=0;i<n;++i) {
            Fence &s=f[i];
            scanf("%d%d%d%d%d", &s.x1,&s.y1,&s.x2,&s.y2,&s.h);
            if(s.x1>s.x2) swap(s.x1,s.x2);
            if(s.y1>s.y2) swap(s.y1,s.y2);
            loX=min(loX,s.x1); hiX=max(hiX,s.x2);
            loY=min(loY,s.y1); hiY=max(hiY,s.y2);
        }
        int tide; scanf("%d", &tide);
        --loX; --loY;
        int w=hiX-loX+1, h=hiY-loY+1;
        vector<unsigned char> a(w*h,0);
        for(int i=0;i<n;++i) if(f[i].h>=tide) {
            Fence s=f[i];
            if(s.x1==s.x2) {
                int x=s.x1-loX;
                for(int y=s.y1-loY;y<s.y2-loY;++y) {
                    a[y*w+x-1]|=1; a[y*w+x]|=4;
                }
            } else {
                int y=s.y1-loY;
                for(int x=s.x1-loX;x<s.x2-loX;++x) {
                    a[(y-1)*w+x]|=2; a[y*w+x]|=8;
                }
            }
        }
        vector<int> q(w*h);
        int head=0,tail=1; q[0]=0; a[0]|=16;
        const int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};
        while(head<tail) {
            int p=q[head++],x=p%w,y=p/w;
            for(int d=0;d<4;++d) {
                int nx=x+dx[d],ny=y+dy[d];
                if(nx<0||nx>=w||ny<0||ny>=h||(a[p]&(1<<d))) continue;
                int np=ny*w+nx;
                if(!(a[np]&16)) { a[np]|=16; q[tail++]=np; }
            }
        }
        printf("%d\n",w*h-tail);
    }
    return 0;
}
