// POJ 3757 - Simple Distributed storage system
// Model: claude-sonnet-5
// Approach: for any chosen set S of exactly K servers, define
// r_i = p_i*b_i/(p_i+b_i) (server i's combined processing+transfer rate).
// If all chosen servers finish at the same time T, then f_i = T*r_i and
// F = T * sum_{i in S} r_i, so cost(S) = F * sum(c_i*r_i) / sum(r_i).
// Minimizing cost is exactly minimizing the weighted average
// q(S) = sum(c_i r_i)/sum(r_i) over K-subsets: a classic 0-1 fractional
// programming problem. Binary search lambda in [min c_i, max c_i]: for
// fixed lambda, min_{|S|=K} sum_{i in S} r_i*(c_i-lambda) is achieved by
// taking the K servers with the smallest values of r_i*(c_i-lambda) (the
// terms are independent, only the count is constrained), and that minimum
// is monotonically decreasing in lambda, crossing zero exactly at the
// optimal ratio q*. Final answer = F * q*.
// The discuss board confirms "0-1 fractional programming + binary search"
// is the intended method, and separately documents that under POJ's G++,
// scanf("%lf", &double) misreads values (needs %f there) while plain C++
// (this submission's language) reads %lf correctly, so staying on C++
// avoids that trap. long double is deliberately avoided: on POJ's old
// MSVC-based C++ compiler it is the same 8 bytes as double with no extra
// precision, while "%Lf" is exactly the kind of ancient-runtime format
// specifier this judge's own board shows misbehaving -- plain double and
// "%lf"/"%f" sidestep that risk entirely. 200 bisection iterations converge
// to double's full precision well before they run out.

#include <cstdio>
#include <algorithm>
using namespace std;

static const int MAXN = 20005;
int n, k;
double F;
double r[MAXN], c[MAXN];
double val[MAXN];

int main() {
    scanf("%d %d %lf", &n, &k, &F);
    double lo = 1e18, hi = -1e18;
    for (int i = 0; i < n; i++) {
        double p, b, cc;
        scanf("%lf %lf %lf", &p, &b, &cc);
        r[i] = p * b / (p + b);
        c[i] = cc;
        if (c[i] < lo) lo = c[i];
        if (c[i] > hi) hi = c[i];
    }
    for (int iter = 0; iter < 200; iter++) {
        double mid = (lo + hi) / 2.0;
        for (int i = 0; i < n; i++) val[i] = r[i] * (c[i] - mid);
        nth_element(val, val + (k - 1), val + n);
        double sum = 0.0;
        for (int i = 0; i < k; i++) sum += val[i];
        if (sum > 0.0) lo = mid; else hi = mid;
    }
    double ans = F * (lo + hi) / 2.0;
    printf("%.4f\n", ans);
    return 0;
}
