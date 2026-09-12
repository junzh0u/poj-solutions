// POJ 3526 - The Teacher's Side of Math
// Model: claude-sonnet-5
//
// t = a^(1/m) + b^(1/n), a,b distinct primes, m,n>1, mn<=20.
// The field Q(alpha,beta) (alpha=a^(1/m), beta=b^(1/n)) has Q-basis
// {alpha^i beta^j : 0<=i<m, 0<=j<n}, dimension d = m*n, matching the
// problem's guarantee that the minimal polynomial of t has degree d.
// Build the d x d integer matrix A of "multiplication by t" in that
// basis (alpha^i beta^j -> alpha^(i+1) beta^j + alpha^i beta^(j+1),
// wrapping alpha^m=a, beta^n=b). Since t generates the whole degree-d
// extension, its minimal polynomial equals the characteristic
// polynomial of A, which is automatically monic and (being the char.
// poly of an integer matrix) has integer coefficients - and monic
// integer polynomials are trivially primitive. So the answer is just
// charpoly(A), computed exactly via the Faddeev-LeVerrier recurrence
// with arbitrary-precision integers (entries of A are only 0,1,a,b,
// but coefficients along the way need big integers).
//
// Ambiguity: none found in the statement; the board's only relevant
// notes were "the leading coefficient really is always 1 (it's given
// in the statement)" and that some people needed bignum / careful
// Gaussian elimination to pass in time - consistent with this
// approach, whose intermediate numbers stay bounded because every
// eigenvalue of A has modulus <= t <= 4 (a root of unity times
// a^(1/m) or b^(1/n)), so Faddeev-LeVerrier's intermediate matrices
// never blow up the way a naive symbolic-resultant expansion would.
//
// Verified against all 5 sample cases (including the degree-12 one)
// with exact match, and against brute force for tiny synthetic cases.

#include <cstdio>
#include <vector>
#include <cstdlib>
using namespace std;

const int BASE = 10000;

struct Big {
    int sign; // -1, 0, 1
    vector<int> d; // little endian, base BASE, no trailing (high) zero limbs when sign!=0

    Big() : sign(0) {}
};

static void trim(Big &x) {
    while (!x.d.empty() && x.d.back() == 0) x.d.pop_back();
    if (x.d.empty()) x.sign = 0;
}

Big fromInt(int v) {
    Big r;
    if (v == 0) { r.sign = 0; return r; }
    r.sign = (v < 0) ? -1 : 1;
    long av = v < 0 ? -(long)v : (long)v;
    while (av > 0) {
        r.d.push_back((int)(av % BASE));
        av /= BASE;
    }
    return r;
}

int cmpAbs(const Big &a, const Big &b) {
    if (a.d.size() != b.d.size()) return a.d.size() < b.d.size() ? -1 : 1;
    for (int i = (int)a.d.size() - 1; i >= 0; --i) {
        if (a.d[i] != b.d[i]) return a.d[i] < b.d[i] ? -1 : 1;
    }
    return 0;
}

vector<int> addAbsVec(const vector<int> &a, const vector<int> &b) {
    vector<int> r;
    int n = (int)(a.size() > b.size() ? a.size() : b.size());
    int carry = 0;
    for (int i = 0; i < n || carry; ++i) {
        int s = carry;
        if (i < (int)a.size()) s += a[i];
        if (i < (int)b.size()) s += b[i];
        r.push_back(s % BASE);
        carry = s / BASE;
    }
    while (!r.empty() && r.back() == 0) r.pop_back();
    return r;
}

// requires a >= b (abs)
vector<int> subAbsVec(const vector<int> &a, const vector<int> &b) {
    vector<int> r;
    int borrow = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        int s = a[i] - borrow - (i < (int)b.size() ? b[i] : 0);
        if (s < 0) { s += BASE; borrow = 1; } else borrow = 0;
        r.push_back(s);
    }
    while (!r.empty() && r.back() == 0) r.pop_back();
    return r;
}

vector<int> mulAbsVec(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return vector<int>();
    vector<int> r(a.size() + b.size(), 0);
    for (int i = 0; i < (int)a.size(); ++i) {
        long carry = 0;
        long ai = a[i];
        if (ai == 0) continue;
        for (int j = 0; j < (int)b.size() || carry; ++j) {
            long cur = r[i + j] + carry;
            if (j < (int)b.size()) cur += ai * (long)b[j];
            r[i + j] = (int)(cur % BASE);
            carry = cur / BASE;
        }
    }
    while (!r.empty() && r.back() == 0) r.pop_back();
    return r;
}

Big operator+(const Big &a, const Big &b) {
    if (a.sign == 0) return b;
    if (b.sign == 0) return a;
    Big r;
    if (a.sign == b.sign) {
        r.sign = a.sign;
        r.d = addAbsVec(a.d, b.d);
    } else {
        int c = cmpAbs(a, b);
        if (c == 0) { r.sign = 0; return r; }
        if (c > 0) { r.sign = a.sign; r.d = subAbsVec(a.d, b.d); }
        else { r.sign = b.sign; r.d = subAbsVec(b.d, a.d); }
    }
    trim(r);
    return r;
}

Big operator-(const Big &a) {
    Big r = a;
    r.sign = -r.sign;
    return r;
}

Big operator-(const Big &a, const Big &b) {
    return a + (-b);
}

Big operator*(const Big &a, const Big &b) {
    Big r;
    if (a.sign == 0 || b.sign == 0) return r;
    r.sign = a.sign * b.sign;
    r.d = mulAbsVec(a.d, b.d);
    trim(r);
    return r;
}

// exact division by small positive int k
Big divExactSmall(const Big &a, int k) {
    Big r;
    if (a.sign == 0) return r;
    vector<int> q(a.d.size(), 0);
    long rem = 0;
    for (int i = (int)a.d.size() - 1; i >= 0; --i) {
        long cur = rem * BASE + a.d[i];
        q[i] = (int)(cur / k);
        rem = cur % k;
    }
    r.sign = a.sign;
    r.d = q;
    trim(r);
    return r;
}

void printBig(const Big &a) {
    if (a.sign == 0) { printf("0"); return; }
    if (a.sign < 0) printf("-");
    printf("%d", a.d.back());
    for (int i = (int)a.d.size() - 2; i >= 0; --i) {
        printf("%04d", a.d[i]);
    }
}

typedef vector<vector<Big> > Mat;

Mat zeroMat(int d) {
    return Mat(d, vector<Big>(d));
}

Mat matMul(const Mat &A, const Mat &B, int d) {
    Mat C = zeroMat(d);
    for (int i = 0; i < d; ++i) {
        for (int k = 0; k < d; ++k) {
            if (A[i][k].sign == 0) continue;
            const Big &aik = A[i][k];
            for (int j = 0; j < d; ++j) {
                if (B[k][j].sign == 0) continue;
                C[i][j] = C[i][j] + aik * B[k][j];
            }
        }
    }
    return C;
}

int main() {
    int a, m, b, n;
    while (scanf("%d %d %d %d", &a, &m, &b, &n) == 4) {
        if (a == 0 && m == 0 && b == 0 && n == 0) break;
        int d = m * n;

        Mat A = zeroMat(d);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                int src = i * n + j;
                int dst1, coef1;
                if (i + 1 < m) { dst1 = (i + 1) * n + j; coef1 = 1; }
                else { dst1 = 0 * n + j; coef1 = a; }
                A[dst1][src] = A[dst1][src] + fromInt(coef1);

                int dst2, coef2;
                if (j + 1 < n) { dst2 = i * n + (j + 1); coef2 = 1; }
                else { dst2 = i * n + 0; coef2 = b; }
                A[dst2][src] = A[dst2][src] + fromInt(coef2);
            }
        }

        // Faddeev-LeVerrier
        Mat M = zeroMat(d); // M_0 = 0
        vector<Big> c(d + 1);
        c[0] = fromInt(1);
        for (int k = 1; k <= d; ++k) {
            // M_k = A*M_{k-1} + c_{k-1} * I
            Mat AM = matMul(A, M, d);
            for (int i = 0; i < d; ++i) AM[i][i] = AM[i][i] + c[k - 1];
            M = AM;
            // c_k = -(1/k) * trace(A*M_k)
            Mat P = matMul(A, M, d);
            Big tr;
            for (int i = 0; i < d; ++i) tr = tr + P[i][i];
            Big negtr = -tr;
            c[k] = divExactSmall(negtr, k);
        }

        for (int k = 0; k <= d; ++k) {
            if (k) printf(" ");
            printBig(c[k]);
        }
        printf("\n");
    }
    return 0;
}
