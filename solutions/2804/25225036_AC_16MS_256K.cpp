// POJ 2804 - Jetpack Sniper 3000 Fragfest Extreme
// Model: gpt-6.1-sol
// Intersect each sight segment with every positive-height building using
// parameter intervals for its three coordinate slabs. Tangencies that could
// change visibility are excluded by the statement; zero-height blocks are empty.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double v[3]; };
bool blocked(const Point &a, const Point &b, int x, int y, int h) {
    if (!h) return false;
    double low[3] = {double(x), double(y), 0.0};
    double high[3] = {double(x+1), double(y+1), double(h)};
    double enter = 0.0, leave = 1.0;
    for (int k=0; k<3; ++k) {
        double d = b.v[k]-a.v[k];
        if (fabs(d)<1e-12) {
            if (a.v[k]<low[k] || a.v[k]>high[k]) return false;
        } else {
            double t1=(low[k]-a.v[k])/d, t2=(high[k]-a.v[k])/d;
            if (t1>t2) swap(t1,t2);
            enter=max(enter,t1); leave=min(leave,t2);
            if (enter>leave+1e-12) return false;
        }
    }
    return enter<=leave+1e-12;
}
int main() {
    int n; if (scanf("%d", &n)!=1) return 0;
    for (int c=1; c<=n; ++c) {
        int h[10][10];
        for (int y=0;y<10;++y) for(int x=0;x<10;++x) scanf("%1d", &h[y][x]);
        Point p[4];
        for (int i=0;i<4;++i) scanf(" ( %lf , %lf , %lf )", &p[i].v[0], &p[i].v[1], &p[i].v[2]);
        printf("Fragfest City #%d\n",c);
        for (int i=1;i<=3;++i) {
            bool hide=false;
            for(int y=0;y<10;++y) for(int x=0;x<10;++x)
                if(blocked(p[0],p[i],x,y,h[y][x])) hide=true;
            printf("Player %c is %s\n",'A'+i-1,hide?"hiding":"in sight");
        }
    }
    return 0;
}
