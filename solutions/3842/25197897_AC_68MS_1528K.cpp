// POJ 3842 - An Industrial Spy
// Model: claude-sonnet-5
// Approach: sieve primality up to 9,999,999 (max value from 7 digits) with a
// bit-packed sieve. For each test case, enumerate every nonempty subset of
// the given digit positions (bitmask over up to 7 positions, so multiplicity
// of repeated digits is respected), generate every distinct permutation of
// that subset's multiset via next_permutation on the sorted digits, parse
// the digit sequence as an integer (leading zeros just fold into a smaller
// value, matching the statement's "identical up to leading zeros" rule),
// and collect the primes found into a std::set to dedupe across subsets/
// permutations before printing its size.
// Ambiguity: none found; empty discuss board. Sample cases (including the
// leading-zero case "011" -> {11, 101}) match by hand trace.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
#include <vector>
using namespace std;

const int MAXV = 10000000; // exclusive upper bound; max 7-digit number is 9999999

vector<bool> isComposite;

void sieve() {
    isComposite.assign(MAXV, false);
    isComposite[0] = true;
    isComposite[1] = true;
    for (int i = 2; i * i < MAXV; ++i) {
        if (!isComposite[i]) {
            for (int j = i * i; j < MAXV; j += i) {
                isComposite[j] = true;
            }
        }
    }
}

int main() {
    sieve();
    int c;
    if (scanf("%d", &c) != 1) return 0;
    char buf[32];
    while (c--) {
        scanf("%s", buf);
        int n = (int)strlen(buf);
        int digits[7];
        for (int i = 0; i < n; ++i) digits[i] = buf[i] - '0';
        set<int> found;
        for (int mask = 1; mask < (1 << n); ++mask) {
            int sub[7], k = 0;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) sub[k++] = digits[i];
            }
            sort(sub, sub + k);
            do {
                int val = 0;
                for (int i = 0; i < k; ++i) val = val * 10 + sub[i];
                if (val >= 2 && !isComposite[val]) found.insert(val);
            } while (next_permutation(sub, sub + k));
        }
        printf("%d\n", (int)found.size());
    }
    return 0;
}
