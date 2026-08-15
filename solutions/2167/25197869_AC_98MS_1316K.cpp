// POJ 2167 - Irrelevant Elements
// Model: claude-sonnet-5
//
// After n-1 rounds of pairwise-summing, the final value is
// sum_{i=1..n} C(n-1, i-1) * a_i (mod m). Element i is irrelevant iff
// its coefficient C(n-1, i-1) is divisible by m for every choice of the
// other a_j, i.e. C(n-1, i-1) mod m == 0.
//
// m need not be prime, so factor m = prod p_j^e_j (at most ~9 distinct
// primes since 2*3*5*7*11*13*17*19*23 < 1e9 < that*29) and, by Kummer's
// theorem, v_p(C(N,k)) = (s_p(k) + s_p(N-k) - s_p(N)) / (p-1) where s_p
// is the base-p digit sum. C(N,k) is divisible by m iff for every prime
// factor p_j, v_{p_j}(C(N,k)) >= e_j. This avoids computing C(N,k)
// itself, which can be astronomically large. All quantities involved
// (n, m <= 1e9, and digit sums of numbers <= 1e5) fit comfortably in a
// 32-bit int, so plain int is used throughout and no 64-bit type or
// format specifier is needed.
//
// Statement gives no explicit case count and the page's DP-TLE thread
// shows accepted-style solutions reading "while (cin >> n >> m)", so
// read to EOF; the discuss board also confirms the trailing blank line
// (when the answer count is 0) is not checked by the judge.
#include <cstdio>
#include <vector>
using namespace std;

static int digitSum(int x, int p) {
    int s = 0;
    while (x > 0) {
        s += x % p;
        x /= p;
    }
    return s;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int N = n - 1;

        vector<int> primes;
        vector<int> exps;
        int mm = m;
        for (int p = 2; p * p <= mm; p++) {
            if (mm % p == 0) {
                int e = 0;
                while (mm % p == 0) { mm /= p; e++; }
                primes.push_back(p);
                exps.push_back(e);
            }
        }
        if (mm > 1) {
            primes.push_back(mm);
            exps.push_back(1);
        }

        int t = (int)primes.size();
        vector<int> sN(t);
        for (int j = 0; j < t; j++) sN[j] = digitSum(N, primes[j]);

        vector<int> result;
        for (int k = 0; k <= N; k++) {
            bool irrelevant = true;
            for (int j = 0; j < t; j++) {
                int p = primes[j];
                int sk = digitSum(k, p);
                int sNk = digitSum(N - k, p);
                int val = (sk + sNk - sN[j]) / (p - 1);
                if (val < exps[j]) { irrelevant = false; break; }
            }
            if (irrelevant) result.push_back(k + 1);
        }

        printf("%d\n", (int)result.size());
        for (size_t i = 0; i < result.size(); i++) {
            printf("%d%c", result[i], (i + 1 == result.size()) ? '\n' : ' ');
        }
        if (result.empty()) printf("\n");
    }
    return 0;
}
