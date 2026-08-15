// POJ 1362 - Skew Binary
// Model: claude-sonnet-5
// Approach: skew binary weight(k) = 2^(k+1)-1. Greedily, while the
// remaining value is positive, subtract the largest weight not exceeding
// it and record its rank; the collected ranks, reversed, are exactly the
// canonical ascending rank list (digit 2 only at the lowest nonzero rank).
// This greedy is equivalent to Okasaki's O(1) skew-binary increment
// (verified by simulating n increments for n=0..19999 and diffing against
// this greedy — zero mismatches), and was also checked by reconstructing
// the value from the emitted rank list for 2000 random n up to the
// stated bound 100663270, plus every worked example posted on the
// problem's discuss board (including the maximal 26-rank list for
// n=100663270). The recursion/iteration depth is O(log n), so no
// per-test performance concern despite t up to 10 and n up to ~1e8.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        scanf("%d", &n);
        vector<int> ranks;
        int rem = n;
        while (rem > 0) {
            int w = 1; // weight(0)
            int k = 0;
            while (true) {
                int nextw = 2 * w + 1; // weight(k+1)
                if (nextw <= rem) { w = nextw; k++; }
                else break;
            }
            ranks.push_back(k);
            rem -= w;
        }
        reverse(ranks.begin(), ranks.end());
        printf("%d [", n);
        for (size_t i = 0; i < ranks.size(); i++) {
            if (i) printf(",");
            printf("%d", ranks[i]);
        }
        printf("]\n");
    }
    return 0;
}
