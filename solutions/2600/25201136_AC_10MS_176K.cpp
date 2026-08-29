// POJ 2600 - Geometrical dreams
// Model: claude-sonnet-5
//
// On each side A_i A_{i+1} of a clockwise polygon, an isosceles triangle
// A_i M_i A_{i+1} is erected outward with apex angle alpha_i at M_i. Given
// the M_i and alpha_i, recover the A_i.
//
// Since |M_i A_i| = |M_i A_{i+1}| and the apex angle is alpha_i, A_{i+1}
// is obtained from A_i by rotating about M_i by +alpha_i (counter-clockwise,
// standard math convention) -- verified against the sample on all three
// edges. Represent points as complex numbers: z_{i+1} = c_i*z_i + m_i*(1-c_i)
// where c_i = e^{i*alpha_i}. Composing all n such affine maps starting from
// the unknown z_1 = x must return to x (A_{n+1} = A_1), giving a single
// linear equation in x: x = (A[n]*x + B[n] == x) => x = B[n]/(1-A[n]).
// The "no subset of angles sums to a multiple of 360" condition guarantees
// the total rotation A[n] != 1, so this is always solvable and unique.
//
// Output: the board's discussion confirms the expected output is the
// coordinates rounded to the nearest integer (printf "%.0f %.0f").
#include <cstdio>
#include <cstring>
#include <cmath>
#include <complex>
#include <vector>
using namespace std;
typedef complex<double> C;

// printf("%.0f", v) renders a small negative value that rounds to zero as
// "-0"; normalize that away before printing.
void printCoord(double v) {
    char buf[64];
    sprintf(buf, "%.0f", v);
    if (strcmp(buf, "-0") == 0) {
        buf[0] = '0';
        buf[1] = '\0';
    }
    printf("%s", buf);
}

int main(){
    int n;
    while (scanf("%d", &n) == 1) {
        vector<C> m(n);
        for (int i = 0; i < n; i++) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            m[i] = C(x, y);
        }
        vector<C> c(n);
        for (int i = 0; i < n; i++) {
            double a;
            scanf("%lf", &a);
            double theta = a * acos(-1.0) / 180.0;
            c[i] = C(cos(theta), sin(theta));
        }
        vector<C> A(n + 1), B(n + 1);
        A[0] = C(1, 0);
        B[0] = C(0, 0);
        for (int i = 0; i < n; i++) {
            A[i + 1] = c[i] * A[i];
            B[i + 1] = c[i] * B[i] + m[i] * (C(1, 0) - c[i]);
        }
        C x = B[n] / (C(1, 0) - A[n]);
        C z = x;
        vector<C> verts(n);
        for (int i = 0; i < n; i++) {
            verts[i] = z;
            z = c[i] * z + m[i] * (C(1, 0) - c[i]);
        }
        for (int i = 0; i < n; i++) {
            printCoord(verts[i].real());
            printf(" ");
            printCoord(verts[i].imag());
            printf("\n");
        }
    }
    return 0;
}
