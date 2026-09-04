// POJ 3001 - Gallup
// Model: claude-sonnet-5
//
// For a candidate total N (1..9999) and a percentage value P (given as an
// exact integer in units of 10^-d, d = number of decimals in the set), the
// set of raw counts c in [0,N] whose rounded percentage round(c*100/N, d
// decimals, half-up) equals P forms a contiguous integer interval. Deriving
// that interval algebraically (all integer arithmetic, no floating point)
// and summing the per-percentage min/max bounds gives the feasible range for
// the total count N: N is achievable iff sum(min_i) <= N <= sum(max_i),
// since each interval contributes every integer in its range independently.
// Try N from 1 upward and report the first that works, else "error".
//
// Ambiguity/notes: statement says numbers "add up to about 100%" with small
// rounding error, and gives no example proving the interval-sum argument;
// the discuss board thread (message 74855/80225) works the sample "2 50 51"
// -> 200 by hand (counts 99 and 101 rounding to 50% and 51%), confirming the
// half-up rounding direction and that per-percentage feasible counts need
// not individually sum to N before rounding. n=0 on its own line terminates
// input (not an empty data set).

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

// ceiling division, b > 0, a may be negative
static long long ceilDiv(long long a, long long b) {
    if (a >= 0) return (a + b - 1) / b;
    else return -((-a) / b);
}

int main() {
    int n;
    int caseNo = 0;
    while (cin >> n) {
        if (n == 0) break;
        caseNo++;
        long long P[25];
        int d = 0;
        for (int i = 0; i < n; i++) {
            string tok;
            cin >> tok;
            size_t dot = tok.find('.');
            if (dot == string::npos) {
                d = 0;
                long long v = 0;
                for (size_t k = 0; k < tok.size(); k++) v = v * 10 + (tok[k] - '0');
                P[i] = v;
            } else {
                string digits = tok.substr(0, dot) + tok.substr(dot + 1);
                d = (int)(tok.size() - dot - 1);
                long long v = 0;
                for (size_t k = 0; k < digits.size(); k++) v = v * 10 + (digits[k] - '0');
                P[i] = v;
            }
        }
        long long K = 100;
        for (int i = 0; i < d; i++) K *= 10; // K = 100 * 10^d
        long long twoK = 2 * K;

        int answer = -1;
        for (int N = 1; N <= 9999; N++) {
            long long summin = 0, summax = 0;
            bool ok = true;
            for (int i = 0; i < n; i++) {
                long long lowA = (long long)N * (2 * P[i] - 1);
                long long highA = (long long)N * (2 * P[i] + 1);
                long long minc = ceilDiv(lowA, twoK);
                long long maxc = ceilDiv(highA, twoK) - 1;
                if (minc < 0) minc = 0;
                if (maxc > N) maxc = N;
                if (minc > maxc) { ok = false; break; }
                summin += minc;
                summax += maxc;
            }
            if (!ok) continue;
            if (summin <= N && N <= summax) {
                answer = N;
                break;
            }
        }
        if (answer == -1) {
            printf("Case %d: error\n", caseNo);
        } else {
            printf("Case %d: %d\n", caseNo, answer);
        }
    }
    return 0;
}
