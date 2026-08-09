// POJ 3590 - The shuffle Problem
// Model: gpt-5.6-terra
// A permutation's order is the LCM of its cycle lengths.  Choose at most one
// power of each prime by grouped knapsack, then print the lexicographically
// smallest permutation having those cycle lengths.

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Big {
    vector<int> a;
    Big(int x = 0) { if (x) a.push_back(x); }
    void mul(int x) {
        int carry = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            int z = a[i] * x + carry;
            a[i] = z % 10000;
            carry = z / 10000;
        }
        while (carry) { a.push_back(carry % 10000); carry /= 10000; }
    }
};
int cmp(const Big &x, const Big &y) {
    if (x.a.size() != y.a.size()) return x.a.size() < y.a.size() ? -1 : 1;
    for (int i = (int)x.a.size() - 1; i >= 0; --i) if (x.a[i] != y.a[i]) return x.a[i] < y.a[i] ? -1 : 1;
    return 0;
}
void printBig(const Big &x) {
    if (x.a.empty()) { cout << 1; return; }
    cout << x.a.back();
    for (int i = (int)x.a.size() - 2; i >= 0; --i) cout << setw(4) << setfill('0') << x.a[i];
    cout << setfill(' ');
}
struct State { bool ok; Big value; vector<int> lengths; State(): ok(false) {} };
bool better(const State &x, const State &y, int sx, int sy) {
    int c = cmp(x.value, y.value);
    if (c != 0) return c > 0;
    if (sx != sy) return sx < sy;
    return x.lengths < y.lengths;
}

int main() {
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<int> primes;
        for (int p = 2; p <= n; ++p) {
            bool prime = true;
            for (int d = 2; d * d <= p; ++d) if (p % d == 0) prime = false;
            if (prime) primes.push_back(p);
        }
        State dp[101], next[101]; dp[0].ok = true; dp[0].value = Big(1);
        for (size_t g = 0; g < primes.size(); ++g) {
            for (int s = 0; s <= n; ++s) next[s] = dp[s];
            for (int s = 0; s <= n; ++s) if (dp[s].ok) {
                for (int q = primes[g]; s + q <= n; q *= primes[g]) {
                    State z = dp[s]; z.ok = true; z.value.mul(q); z.lengths.push_back(q);
                    sort(z.lengths.begin(), z.lengths.end());
                    if (!next[s + q].ok || better(z, next[s + q], s + q, s + q)) next[s + q] = z;
                    if (q > n / primes[g]) break;
                }
            }
            for (int s = 0; s <= n; ++s) dp[s] = next[s];
        }
        State best; int bestSum = 0;
        for (int s = 0; s <= n; ++s) if (dp[s].ok && (!best.ok || better(dp[s], best, s, bestSum))) { best = dp[s]; bestSum = s; }
        vector<int> p; int at = 1;
        for (int i = 0; i < n - bestSum; ++i) { p.push_back(at); ++at; }
        for (size_t c = 0; c < best.lengths.size(); ++c) {
            int len = best.lengths[c];
            for (int j = 1; j < len; ++j) p.push_back(at + j);
            p.push_back(at);
            at += len;
        }
        printBig(best.value);
        for (size_t i = 0; i < p.size(); ++i) cout << ' ' << p[i];
        cout << '\n';
    }
}
