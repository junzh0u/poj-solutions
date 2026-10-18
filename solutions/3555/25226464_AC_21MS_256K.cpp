// POJ 3555 - Cocktail
// Model: gpt-6-astra
// Invert the vessel's available volume for each liquid boundary at a fixed
// cube-bottom height. Buoyancy decreases as that height rises, so bisection
// finds equilibrium, with the floor enforcing a nonnegative bottom height.
// Calculate without overflow, then cap at H: lowering the cube monotonically
// raises the surface until equilibrium or the floor, so overflow leaves it full.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double A, B, n;
double level(double v, double z) {
    if (v <= A*z) return v/A;
    if (v <= A*z+(A-B)*n) return z+(v-A*z)/(A-B);
    return (v+B*n)/A;
}
double immersed(double y, double z) {
    return max(0.0, min(n, y-z));
}
int main() {
    double H,R,d1,h1,d2,h2,d;
    if (scanf("%lf%lf%lf%lf%lf%lf%lf%lf", &H,&R,&d1,&h1,&d2,&h2,&d,&n)!=8) return 0;
    A=acos(-1.0)*R*R; B=n*n;
    double lo=0,hi=h1+h2;
    for (int k=0;k<100;++k) {
        double z=(lo+hi)/2;
        double a=immersed(level(A*h1,z),z);
        double b=immersed(level(A*(h1+h2),z),z);
        if (d1*a+d2*(b-a)>d*n) lo=z;
        else hi=z;
    }
    printf("%.8f\n", min(H,level(A*(h1+h2),(lo+hi)/2)));
    return 0;
}
