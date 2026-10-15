// POJ 2616 - Origami
// Model: gpt-6.1-sol
// Undo each fold: a point strictly on the left has an unchanged and a
// reflected preimage. Points on the right or within 1e-6 of a crease
// contribute nothing. Original square borders use the same tolerance.
#include <cstdio>
#include <cmath>
const double EPS = 1e-6;
struct Fold { double x, y, dx, dy; } fold[8];
int layers(int n, double x, double y) {
    if (!n) return x > EPS && y > EPS && x < 100-EPS && y < 100-EPS;
    const Fold &f = fold[n-1];
    double d = f.dx*(y-f.y)-f.dy*(x-f.x);
    if (d <= EPS) return 0;
    return layers(n-1,x,y) + layers(n-1,x+2*f.dy*d,y-2*f.dx*d);
}
int main() {
    int n, q;
    if (scanf("%d", &n) != 1) return 0;
    for (int i=0; i<n; ++i) {
        double x2,y2;
        Fold &f=fold[i];
        scanf("%lf%lf%lf%lf",&f.x,&f.y,&x2,&y2);
        f.dx=x2-f.x; f.dy=y2-f.y;
        double len=sqrt(f.dx*f.dx+f.dy*f.dy);
        f.dx/=len; f.dy/=len;
    }
    scanf("%d",&q);
    while(q--) {
        double x,y; scanf("%lf%lf",&x,&y);
        printf("%d\n",layers(n,x,y));
    }
    return 0;
}
