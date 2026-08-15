// POJ 2276 - Model Rocket Height
// Model: claude-sonnet-5
//
// Approach: place observers A,B,C at (0,0),(D,0),(2D,0) in the ground
// plane; instruments sit H feet above the field.  If the rocket's
// vertical height above the field is Z, the vertical offset from each
// instrument to the rocket is k = Z - H, and the horizontal distance
// from observer i to the rocket's ground projection is r_i = k*cot(angle_i).
// Writing r_i^2 = k^2*cot(angle_i)^2 and expanding the three circle
// equations (x-x_i)^2+y^2=r_i^2 for the unknown projection (x,y) and
// eliminating x,y via pairwise subtraction gives a single equation in k^2:
//   k^2*(cotA^2 - 2*cotB^2 + cotC^2) = 2*D^2
// solved directly for k, then Z = H + k, rounded to nearest foot
// (x.5 rounds up).
#include <cstdio>
#include <cmath>
using namespace std;

int main(){
    double D,H;
    while(scanf("%lf %lf",&D,&H)==2){
        double a,b,c;
        // read triples until one <=0 encountered (that's the terminator, not preceded by D,H read again)
        while(scanf("%lf %lf %lf",&a,&b,&c)==3){
            if(a<=0||b<=0||c<=0) break;
            double PI = acos(-1.0);
            double A = a*PI/180.0, B = b*PI/180.0, C = c*PI/180.0;
            double cotA = cos(A)/sin(A);
            double cotB = cos(B)/sin(B);
            double cotC = cos(C)/sin(C);
            double denom = cotA*cotA - 2*cotB*cotB + cotC*cotC;
            double k2 = 2*D*D/denom;
            double k = sqrt(k2);
            double Z = H + k;
            long ans = (long)floor(Z+0.5);
            printf("%ld\n", ans);
        }
        break; // only one dataset (D,H) per input per problem statement)
    }
    return 0;
}
