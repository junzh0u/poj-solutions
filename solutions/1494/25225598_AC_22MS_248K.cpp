// POJ 1494 - Sunrise
// Model: gpt-6-astra
// Intersect the observer's tangent horizon with the planar solar disc.
// Integrate the visible circular segment; time starts at the upper rim's
// first visibility, including the observer's displacement from planet center.
#include <cstdio>
#include <cmath>
int main() {
    const double pi = acos(-1.0), d = 92900000.0, r = 3950.0, s = 432000.0;
    const double start = asin(r / sqrt(d*d+s*s)) - atan(s/d);
    double t;
    while (scanf("%lf", &t) == 1) {
        double a = start + t * (2*pi/86400.0);
        double x = (r-d*sin(a))/(s*cos(a));
        double answer;
        if (t <= 0 || x >= 1) answer = 0;
        else if (x <= -1) answer = 1;
        else {
            double theta = acos(x);
            if (theta < 0.001) {
                double z = theta*theta;
                answer = theta*z*(2.0/3-z*(2.0/15-z*4.0/315))/pi;
            } else answer = (theta-x*sqrt(1-x*x))/pi;
        }
        printf("%.15g\n", answer);
    }
    return 0;
}
