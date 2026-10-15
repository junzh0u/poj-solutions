// POJ 3902 - The Bad Number
// Model: gpt-6.1-sol
// Start at the capacity lower bound and test feasible summand counts.
// For N >= 3 and at least two terms, all sums in [t,t*L] are
// reachable except t*L-1 when L mod N is 1. Here L is the largest
// allowed term. N=2 instead allows only sums with the count's parity.
#include <iostream>
using namespace std;
typedef int Int;
Int solve(Int n, Int m, Int k) {
    if (n == 1) return -1;
    Int l = k - (k % n == 0);
    if (l == 1) return m % n ? m : -1;
    if (n == 2 && m % 2 == 0) return -1;
    Int first = (m + l - 1) / l;
    for (Int t = first; t <= m && t <= first + 4; ++t) {
        if (t % n == 0) continue;
        if (t == 1) {
            if (m % n != 0) return t;
        } else if (n == 2) {
            if (m % 2 == t % 2) return t;
        } else if (!(l % n == 1 && (m + 1) % l == 0 && t == (m + 1) / l)) {
            return t;
        }
    }
    return -1;
}
int main() {
    int tests; cin >> tests;
    while (tests--) {
        Int n,m,k; cin >> n >> m >> k;
        cout << solve(n,m,k) << '\n';
    }
    return 0;
}
