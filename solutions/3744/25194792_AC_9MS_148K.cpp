// POJ 3744 - Scout YYF I
// Model: gpt-5.6-terra
// Keep the probability mass of paths which reach the current and previous
// positions without already hitting a mine.  Between mines this follows
// f[i] = p*f[i-1] + (1-p)*f[i-2], so a 2x2 matrix jumps across large gaps.
// A mine removes the mass that reaches it; the remaining mass is the answer.
// Mines are unordered, and a mine at position 1 is immediately fatal.
#include <algorithm>
#include <cstdio>

struct Matrix {
    long double a00, a01, a10, a11;
};

static Matrix multiply(const Matrix &a, const Matrix &b) {
    Matrix c;
    c.a00 = a.a00 * b.a00 + a.a01 * b.a10;
    c.a01 = a.a00 * b.a01 + a.a01 * b.a11;
    c.a10 = a.a10 * b.a00 + a.a11 * b.a10;
    c.a11 = a.a10 * b.a01 + a.a11 * b.a11;
    return c;
}

static Matrix power(Matrix base, int exponent) {
    Matrix result;
    result.a00 = 1.0L;
    result.a01 = 0.0L;
    result.a10 = 0.0L;
    result.a11 = 1.0L;
    while (exponent > 0) {
        if (exponent & 1) result = multiply(result, base);
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

int main() {
    int n;
    long double p;
    while (std::scanf("%d%Lf", &n, &p) == 2) {
        int mine[10];
        int i;
        for (i = 0; i < n; ++i) std::scanf("%d", &mine[i]);
        std::sort(mine, mine + n);

        Matrix step;
        step.a00 = p;
        step.a01 = 1.0L - p;
        step.a10 = 1.0L;
        step.a11 = 0.0L;

        long double current = 1.0L;
        long double previous = 0.0L;
        long double safe = 1.0L;
        int position = 1;
        for (i = 0; i < n; ++i) {
            Matrix jump = power(step, mine[i] - position);
            long double next_current = jump.a00 * current + jump.a01 * previous;
            long double next_previous = jump.a10 * current + jump.a11 * previous;
            safe -= next_current;
            current = 0.0L;
            previous = next_previous;
            position = mine[i];
        }
        if (safe < 0.0L) safe = 0.0L;
        if (safe > 1.0L) safe = 1.0L;
        std::printf("%.7Lf\n", safe);
    }
    return 0;
}
