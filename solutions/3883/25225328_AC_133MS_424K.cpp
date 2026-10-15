// POJ 3883 - Penguin Bashing
// Model: gpt-6.1-sol
// Enumerate integral angles, then simulate sorted ground obstacles.
// Sliding loses 5 m/s per meter; mine jumps skip the next 2 meters.
// Compare rounded centimeter scores, retaining the lowest angle on ties.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Obstacle {
    double x; bool mine;
    bool operator<(const Obstacle& b) const { return x < b.x; }
};
int main() {
    int cases; scanf("%d", &cases);
    const double pi = acos(-1.0);
    while (cases--) {
        int n; scanf("%d", &n);
        vector<Obstacle> a(n);
        for (int i=0;i<n;++i) {
            char type[16]; scanf("%lf%s", &a[i].x, type);
            a[i].mine = type[0]=='m';
        }
        sort(a.begin(), a.end());
        int best=-101, angle=-90;
        for (int d=-90;d<=90;++d) {
            double rad=d*pi/180.0, v=25*cos(rad), vy=25*sin(rad);
            double t=(vy+sqrt(vy*vy+2*9.81))/9.81;
            double x=v*t;
            bool dead=false;
            for (int i=0;i<n;++i) {
                if (a[i].x < x) continue;
                double loss=5*(a[i].x-x);
                if (loss > v) break;
                v-=loss; x=a[i].x;
                if (!a[i].mine) { dead=true; break; }
                v+=4; x+=2;
            }
            int score=dead ? -100 : int(floor((x+v/5)*100+0.5));
            if (score>best) {best=score; angle=d;}
        }
        printf("%d %d\n",best,angle);
    }
    return 0;
}
