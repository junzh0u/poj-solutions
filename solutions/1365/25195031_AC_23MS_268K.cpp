// POJ 1365 - Prime Land
// Model: claude-opus-5
//
// Each input line holds a number x (2 < x <= 32767) written in "prime base"
// notation: pairs "p e" meaning the factor p^e, listed in decreasing order of
// p and only for e > 0.  Rebuild x by multiplying the p^e, then print x-1 in
// the same notation.  A line whose first token is 0 terminates the input.
//
// Approach: parse a whole line at a time (the number of pairs per line varies,
// so token-at-a-time reading cannot tell where a line ends), multiply out to
// get x, then factor x-1 by trial division.  x <= 32767 so x-1 >= 2 always has
// a non-empty factorization and trial division up to sqrt is instant.
// Factors come out in increasing prime order, so print them reversed.
//
// Ambiguity settled from the discuss board: the pairs are (prime, exponent)
// with the primes given explicitly -- "509 1 59 1" is 509^1 * 59^1, not an
// exponent vector indexed by the sequence of all primes.  The two readings
// disagree on the sample, and only this one reproduces it.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        // Samples/judge data use CRLF line endings; drop the CR.
        while (!line.empty() && (line[line.size() - 1] == '\r' ||
                                 line[line.size() - 1] == '\n')) {
            line.erase(line.size() - 1);
        }

        istringstream in(line.c_str());
        long p;
        if (!(in >> p)) continue;  // blank line: nothing to do
        if (p == 0) break;

        long x = 1;
        long e;
        in >> e;
        for (long i = 0; i < e; ++i) x *= p;
        while (in >> p) {
            in >> e;
            for (long i = 0; i < e; ++i) x *= p;
        }

        long n = x - 1;
        long fp[32], fe[32];
        int cnt = 0;
        for (long d = 2; d * d <= n; ++d) {
            if (n % d) continue;
            long k = 0;
            while (n % d == 0) {
                n /= d;
                ++k;
            }
            fp[cnt] = d;
            fe[cnt] = k;
            ++cnt;
        }
        if (n > 1) {
            fp[cnt] = n;
            fe[cnt] = 1;
            ++cnt;
        }

        for (int i = cnt - 1; i >= 0; --i) {
            if (i != cnt - 1) printf(" ");
            printf("%ld %ld", fp[i], fe[i]);
        }
        printf("\n");
    }
    return 0;
}
