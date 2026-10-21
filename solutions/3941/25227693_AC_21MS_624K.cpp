// POJ 3941 - Expected Allowance
// Model: gpt-6-astra
// Convolve the fair-die distributions, then average max(1, sum-k).
// Two rows hold probabilities; no integer outcome count can overflow.
// The one-bill guarantee applies to every sum at most k+1.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n, m, k;
    while (scanf("%d%d%d", &n, &m, &k) == 3 && n) {
        vector<long double> p(n*m+1, 0), q(n*m+1, 0);
        p[0] = 1;
        for (int i = 1; i <= n; ++i) {
            fill(q.begin(), q.end(), 0);
            for (int s = i-1; s <= (i-1)*m; ++s)
                for (int d = 1; d <= m; ++d)
                    q[s+d] += p[s]/m;
            p.swap(q);
        }
        long double answer = 0;
        for (int s = n; s <= n*m; ++s)
            answer += p[s] * max(1, s-k);
        printf("%.8f\n", (double)answer);
    }
    return 0;
}
