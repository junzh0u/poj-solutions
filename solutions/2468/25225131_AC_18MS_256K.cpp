// POJ 2468 - Adventures in Moving - Part V
// Model: gpt-6.1-sol
// Try all final axis assignments, then straight, forward, and sideways loading.
// A tilt must clear the ceiling throughout the rotation back to upright.
// Forward loading constrains the door's slice, not the full projected length.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double EPS = 1e-9;
bool fits(double w, double h, double l, double door,
          double a, double b, double c) {
    if (a > w || b > h || c > l) return false;
    if (b <= door) return true;
    if (c <= door && b*b+c*c <= h*h+EPS && b*c <= l*door+EPS)
        return true;
    double diagonal = sqrt(a*a+b*b);
    if (a <= door && diagonal <= h+EPS) {
        double angle = atan2(a,b) + acos(door/diagonal);
        if (a*cos(angle)+b*sin(angle) <= w+EPS) return true;
    }
    return false;
}
int main() {
    double w,h,l,door,d[3];
    while (scanf("%lf%lf%lf%lf", &w,&h,&l,&door)==4) {
        if (scanf("%lf%lf%lf", &d[0],&d[1],&d[2])!=3) break;
        sort(d,d+3);
        bool ok=false;
        do { if(fits(w,h,l,door,d[0],d[1],d[2])) ok=true; }
        while(next_permutation(d,d+3));
        puts(ok ? "The box goes on the truck." : "The box will not go on the truck.");
    }
    return 0;
}
