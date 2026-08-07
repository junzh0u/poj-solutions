// POJ 3226 - String's Puzzle
// Model: claude-sonnet-5
// Approach: given an n-length string of distinct uppercase letters, compute
// its 0-indexed rank among all P(26,n) permutations of n distinct letters
// chosen from the 26-letter alphabet, listed in lexicographic order.
// Standard "permutation rank from a larger pool" formula: at position i,
// count how many still-unused letters are smaller than s[i]; each such
// letter would start a block of P(25-i, n-1-i) permutations that sort
// before position i's actual choice. Sum these blocks. Since P(26,26) =
// 26! ~= 4e26 overflows 64-bit integers, the accumulator and the block
// sizes are kept in a small base-10 bignum (digit vector, multiply-by-
// small-int and add -- all this problem needs). The block sizes for a
// fixed n form a simple running product as i decreases (block(i-1) =
// block(i) * (26-i), block(n-1) = 1), so they're precomputed once per
// test case in O(n) bignum multiplications instead of recomputed from
// scratch at each position, comfortably covering the judge's slower
// hardware even on adversarial-size input.
// No statement ambiguity: the worked example in the description (ABC=0,
// ZYX=15599 for n=3) pins down both the direction (ascending) and the
// 0-index directly, and matches this formula exactly. The discuss
// board's claimed n=26 answer for the fully-reversed string (403291461
// 126605635583999981) was checked against the closed form sum_{j=0}^{25}
// j*j! = 26!-1 and found to be off by 18 -- an unresolved dispute
// visible in the thread's own back-and-forth replies -- so it was not
// used; 26!-1 is the value this program computes and was verified
// against an independent unranking algorithm for random and exhaustive
// small-n cases instead.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

typedef vector<int> Big; // little-endian decimal digits

void mulSmall(Big &a, int m) {
    if (m == 0) { a.assign(1, 0); return; }
    long long carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        long long cur = (long long)a[i] * m + carry;
        a[i] = (int)(cur % 10);
        carry = cur / 10;
    }
    while (carry) {
        a.push_back((int)(carry % 10));
        carry /= 10;
    }
}

void addBig(Big &a, const Big &b) {
    size_t n = a.size() > b.size() ? a.size() : b.size();
    a.resize(n, 0);
    int carry = 0;
    for (size_t i = 0; i < n; ++i) {
        int s = a[i] + (i < b.size() ? b[i] : 0) + carry;
        a[i] = s % 10;
        carry = s / 10;
    }
    if (carry) a.push_back(carry);
}

void printBig(const Big &a) {
    int i = (int)a.size() - 1;
    while (i > 0 && a[i] == 0) --i;
    for (; i >= 0; --i) putchar('0' + a[i]);
    putchar('\n');
}

int main() {
    int n;
    char buf[40];
    Big block[26];
    while (scanf("%d", &n) == 1 && n != 0) {
        scanf("%s", buf);
        bool used[26];
        memset(used, 0, sizeof(used));
        // block[i] = P(25-i, n-1-i), built from i=n-1 down to i=0:
        // block[n-1] = 1 (empty product), block[i-1] = block[i]*(26-i)
        block[n - 1].assign(1, 1);
        for (int i = n - 1; i >= 1; --i) {
            block[i - 1] = block[i];
            mulSmall(block[i - 1], 26 - i);
        }
        Big rank(1, 0);
        for (int i = 0; i < n; ++i) {
            int c = buf[i] - 'A';
            int cnt = 0;
            for (int j = 0; j < c; ++j) if (!used[j]) ++cnt;
            Big contrib = block[i];
            mulSmall(contrib, cnt);
            addBig(rank, contrib);
            used[c] = true;
        }
        printBig(rank);
    }
    return 0;
}
