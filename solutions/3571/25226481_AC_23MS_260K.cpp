// POJ 3571 - Game
// Model: gpt-6-astra
// Before n is visited, the walk lies on the path 1..n-1. Winning
// means visiting both ends of that path before stepping onto n.
// Split by the first end reached, then compute the probability of
// crossing to the other end before absorption. Harmonic-function
// differences are positive products q_i/p_i; sum each tail directly
// rather than subtracting probabilities close to one. O(n) time/space.
#include <cstdio>

double p[51];

double hit(int a, int b, int x, bool right) {
    double weight = 1.0, total = 0.0, part = 0.0;
    for (int j = a; j < b; ++j) {
        if (j > a) weight *= (1.0 - p[j]) / p[j];
        total += weight;
        if (right ? j < x : j >= x) part += weight;
    }
    return part / total;
}

int main() {
    int n, k;
    if (std::scanf("%d%d", &n, &k) != 2) return 0;
    for (int i = 1; i < n; ++i) std::scanf("%lf", &p[i]);
    double answer = 1.0;
    if (n > 2) {
        answer = hit(1, n-1, k, false) * hit(0, n-1, 1, true)
               + hit(1, n-1, k, true) * hit(1, n, n-1, false);
    }
    std::printf("%.10f\n", answer);
    return 0;
}
