// POJ 1320 - Street Numbers
// Model: claude-sonnet-5
//
// No input. Houses numbered 1..k on one side of a street; the programmer's
// house is number n. Walking one way past all houses before hers and back
// (excluding her own) sums to n(n-1)/2; walking the other way past all
// houses after hers sums to k(k+1)/2 - n(n+1)/2. Setting these equal:
//   n(n-1)/2 = k(k+1)/2 - n(n+1)/2  =>  n^2 = k(k+1)/2  =>  2n^2 = k(k+1)
// Multiply by 4 and complete the square in k: (2k+1)^2 - 8n^2 = 1, a Pell
// equation x^2 - 8y^2 = 1 with x = 2k+1, y = n. Fundamental solution
// (x1,y1) = (3,1) generates all solutions via
//   x_{i+1} = 3*x_i + 8*y_i,  y_{i+1} = x_i + 3*y_i
// starting from (x0,y0) = (1,0). The first nontrivial solution (x1,y1)=(3,1)
// gives (n,k) = (1,1), a degenerate single-house street with no walk at
// all (excluded, matching the sample which starts at (6,8)), so we skip it
// and print the next 10 generated pairs. Values stay well within a plain
// 32-bit int (the 10th pair's k is under 66 million), so no bignum or
// 64-bit type is needed.
#include <cstdio>

int main() {
    long x = 1, y = 0;
    int printed = 0;
    bool first = true;
    while (printed < 10) {
        long nx = 3 * x + 8 * y;
        long ny = x + 3 * y;
        x = nx; y = ny;
        long k = (x - 1) / 2;
        long n = y;
        if (first) { first = false; continue; } // skip degenerate (1,1)
        std::printf("%10ld%10ld\n", n, k);
        printed++;
    }
    return 0;
}
