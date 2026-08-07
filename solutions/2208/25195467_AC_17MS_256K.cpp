// POJ 2208 - Pyramids
// Model: claude-sonnet-5
// Approach: Cayley-Menger determinant. The 5x5 matrix is built directly
// from the squared edge lengths in the input's own order (AB, AC, AD,
// BC, BD, CD), mapped onto pairwise distances between points 0=A,1=B,
// 2=C,3=D. This avoids relabeling against a memorized closed-form
// formula variable convention -- the discuss board explicitly flags a
// mismatch between formula-variable names and problem edge order as a
// common bug (message 65980: "AD=a,BD=b,CD=c,AB=l,BC=m,AC=n must match
// the problem's edges"). Here the mapping is the identity, so there is
// nothing to mismatch. 288*V^2 = det(CM). The determinant is computed
// by Gaussian elimination with partial pivoting in long double; the
// squared edge lengths (<=10^6) are exact in long double so no
// precision is lost constructing the matrix, and pivoting keeps the
// elimination itself well conditioned even for the sample's very flat
// tetrahedron. Special Judge (confirmed on the problem page and by the
// board threads about differing last digits), so no unusual tolerance
// handling is needed; read datasets to EOF in case the judge feeds more
// than one despite the statement's singular phrasing.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    double ab, ac, ad, bc, bd, cd;
    while (scanf("%lf %lf %lf %lf %lf %lf", &ab, &ac, &ad, &bc, &bd, &cd) == 6) {
        long double d[4][4];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                d[i][j] = 0;
        d[0][1] = d[1][0] = (long double)ab * ab;
        d[0][2] = d[2][0] = (long double)ac * ac;
        d[0][3] = d[3][0] = (long double)ad * ad;
        d[1][2] = d[2][1] = (long double)bc * bc;
        d[1][3] = d[3][1] = (long double)bd * bd;
        d[2][3] = d[3][2] = (long double)cd * cd;

        long double m[5][5];
        m[0][0] = 0;
        for (int j = 1; j < 5; j++) m[0][j] = 1;
        for (int i = 1; i < 5; i++) {
            m[i][0] = 1;
            for (int j = 1; j < 5; j++) m[i][j] = d[i - 1][j - 1];
        }

        long double det = 1;
        for (int col = 0; col < 5; col++) {
            int piv = -1;
            long double best = 0;
            for (int row = col; row < 5; row++) {
                long double v = m[row][col] < 0 ? -m[row][col] : m[row][col];
                if (v > best) { best = v; piv = row; }
            }
            if (piv < 0 || best < 1e-15L) { det = 0; break; }
            if (piv != col) {
                for (int k = 0; k < 5; k++) swap(m[piv][k], m[col][k]);
                det = -det;
            }
            det *= m[col][col];
            for (int row = col + 1; row < 5; row++) {
                long double factor = m[row][col] / m[col][col];
                for (int k = col; k < 5; k++) m[row][k] -= factor * m[col][k];
            }
        }

        long double vol2 = det / 288.0L;
        if (vol2 < 0) vol2 = 0;
        double V = (double)sqrtl(vol2);
        printf("%.4f\n", V);
    }
    return 0;
}
