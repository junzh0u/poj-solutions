// POJ 3575 - Kingdom Partitioning
// Model: gpt-6-astra
// Moving knife: each remaining kingdom requests a vertical cut enclosing
// one n-th of its original value after the current left boundary. Give the
// leftmost requested strip to its requester. Every other kingdom loses at
// most one n-th, so the invariant guarantees enough value for all survivors.
// Circle segment areas are evaluated analytically and cuts by bisection.
// Full-height rectangles give convex, pairwise interior-disjoint witnesses.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Circle { double x, r; } c[30][30];
int n, m[30];
bool used[30];
double total[30], leftedge[30], rightedge[30];
const double PI = acos(-1.0);
double area(int i, double x) {
    double ans = 0;
    for (int j=0; j<m[i]; ++j) {
        double r=c[i][j].r, d=x-c[i][j].x;
        if (d<=-r) continue;
        if (d>=r) ans+=PI*r*r;
        else ans+=r*r*(asin(d/r)+PI/2)+d*sqrt(max(0.0,r*r-d*d));
    }
    return ans;
}
int main() {
    if (scanf("%d", &n)!=1) return 0;
    for (int i=0;i<n;++i) {
        scanf("%d", &m[i]);
        for (int j=0;j<m[i];++j) {
            double y;
            scanf("%lf%lf%lf", &c[i][j].x, &y, &c[i][j].r);
            total[i]+=PI*c[i][j].r*c[i][j].r;
        }
    }
    double left=-2500;
    for (int step=0;step<n;++step) {
        double best=2500;
        int who=-1;
        for (int i=0;i<n;++i) if (!used[i]) {
            double lo=left, hi=2500;
            double target=area(i,left)+total[i]/n;
            for (int k=0;k<70;++k) {
                double mid=(lo+hi)/2;
                if (area(i,mid)<target) lo=mid; else hi=mid;
            }
            if (who==-1 || hi<best) { best=hi; who=i; }
        }
        if (step==n-1) best=2500;
        used[who]=true;
        leftedge[who]=left; rightedge[who]=best;
        left=best;
    }
    for (int i=0;i<n;++i) {
        printf("4\n%.10f -2500.0000000000\n%.10f -2500.0000000000\n",leftedge[i],rightedge[i]);
        printf("%.10f 2500.0000000000\n%.10f 2500.0000000000\n",rightedge[i],leftedge[i]);
    }
    return 0;
}
