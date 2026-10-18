// POJ 3552 - Snatch!
// Model: gpt-6-astra
// Place N values evenly around the M-position circle. Rotation sums then
// span exactly M-gcd(N,M), attaining the modular-residue lower bound on
// their maximum: (N*(M+1)+M-gcd(N,M))/2. The first value is always 1.
// Cylinder order beyond the first is unrestricted; any optimum is valid.
#include <cstdio>
int main() {
    int n, m;
    if (std::scanf("%d%d", &n, &m) != 2) return 0;
    for (int i = 0; i < n; ++i)
        std::printf("%d%c", i * m / n + 1, i + 1 == n ? '\n' : ' ');
    return 0;
}
