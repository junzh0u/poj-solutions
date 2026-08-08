// POJ 1914 - Cramer's Rule
// Model: claude-opus-5
//
// Straight transcription of Cramer's rule for a 3x3 system, using the
// cofactor expansion the statement pictures in images/1914_2.jpg:
//   det = a11(a22a33 - a23a32) - a12(a21a33 - a23a31) + a13(a21a32 - a22a31)
// det(A_i) is that same expansion over the columns of A with column i
// replaced by b, so one helper parameterised by three column indices covers
// all four determinants.
//
// The determinants must be 64-bit. With |a_ij| <= 1000 the extreme value is
// 1000^3 times the largest 3x3 determinant of a +-1 matrix, which is 4, so
// |det| reaches 4000000000 -- past INT_MAX. That case cannot arise in the
// official sample (its entries are 0..4), which is why the discuss board is
// full of Wrong Answers fixed only by switching to int64/__int64. Submitted
// under G++ (language 0) rather than C++ (language 4) for long long/%lld.
//
// Output detail from the statement: x_i strictly inside (-0.0005, 0.0005)
// prints "0.000", never "-0.000". This fires even on the sample, where
// det(A2)=det(A3)=0 with det(A)=-1 makes the quotient negative zero.

#include <cstdio>

static long long m[3][4];

/* Determinant of the 3x3 matrix formed by columns c0, c1, c2 of m. */
static long long det3(int c0, int c1, int c2) {
    return m[0][c0] * (m[1][c1] * m[2][c2] - m[1][c2] * m[2][c1])
         - m[0][c1] * (m[1][c0] * m[2][c2] - m[1][c2] * m[2][c0])
         + m[0][c2] * (m[1][c0] * m[2][c1] - m[1][c1] * m[2][c0]);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n-- > 0) {
        int i, j;
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 4; ++j)
                scanf("%lld", &m[i][j]);

        long long dA = det3(0, 1, 2);
        long long d[3];
        d[0] = det3(3, 1, 2);   /* A1: column 1 replaced by b */
        d[1] = det3(0, 3, 2);   /* A2: column 2 replaced by b */
        d[2] = det3(0, 1, 3);   /* A3: column 3 replaced by b */

        printf("%lld %lld %lld %lld\n", d[0], d[1], d[2], dA);
        if (dA == 0) {
            printf("No unique solution\n");
        } else {
            printf("Unique solution:");
            for (i = 0; i < 3; ++i) {
                double x = (double)d[i] / (double)dA;
                if (x > -0.0005 && x < 0.0005) x = 0.0;
                printf(" %.3f", x);
            }
            printf("\n");
        }
        printf("\n");            /* the statement's empty third line */
    }
    return 0;
}
