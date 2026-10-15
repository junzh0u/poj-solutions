// POJ 3792 - Area of Polycubes
// Model: gpt-6.1-sol
// Check each placement against all preceding cubes. Each shared face removes
// two faces from 6P; this counts enclosed cavity walls as required.
// Duplicate placements and a non-origin first cube are invalid as well.
#include <cstdio>
#include <cstdlib>
struct Point { int x,y,z; };
int main() {
    int cases;
    if (scanf("%d", &cases)!=1) return 0;
    for (int tc=1; tc<=cases; ++tc) {
        int n; scanf("%d", &n);
        Point a[105]; int bad=0, area=0;
        for (int i=0; i<n; ++i) {
            scanf("%d,%d,%d", &a[i].x, &a[i].y, &a[i].z);
            int shared=0; bool duplicate=false;
            for (int j=0; j<i; ++j) {
                double dx=(double)a[i].x-a[j].x;
                double dy=(double)a[i].y-a[j].y;
                double dz=(double)a[i].z-a[j].z;
                double d=(dx<0?-dx:dx)+(dy<0?-dy:dy)+(dz<0?-dz:dz);
                if (d==0) duplicate=true;
                if (d==1) ++shared;
            }
            bool invalid=(i==0 ? (a[i].x!=0 || a[i].y!=0 || a[i].z!=0) : shared==0);
            if (!bad && (invalid || duplicate)) bad=i+1;
            area+=6-2*shared;
        }
        if (bad) printf("%d NO %d\n",tc,bad);
        else printf("%d %d\n",tc,area);
    }
    return 0;
}
