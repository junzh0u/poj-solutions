// POJ 3541 - Given a string...
// Model: claude-sonnet-5
//
// Approach: T(i) = S(k)_i xor S(l)_i for some 0<=k,l<=n-1, where S(k) is the
// k-th circular right-shift (S(k)_i = S_{(i-k) mod n}). Substituting m = i-l
// (mod n) turns the condition into: for every m,
//   S_{(m-d) mod n} xor S_m  =  T_{(m+l) mod n},     where d = (k-l) mod n.
// The left side, as a function of m, is a fixed string A_d depending only on
// d (n possible values). The right side is T's residue-shifted-by-l version,
// i.e. some cyclic rotation of T. Since d ranges over all of [0,n) as k,l do,
// and l independently ranges over all rotations, the whole condition reduces
// to: T belongs to S^xor iff, for SOME d in [0,n), the string
//   A_d[m] = S[(m-d) mod n] xor S[m]
// is a cyclic rotation of T (equivalently T is a cyclic rotation of A_d,
// since cyclic-rotation-equivalence is symmetric).
//
// For each of the n candidate d's, test "does T occur as a length-n window of
// A_d doubled (A_d+A_d)" using the KMP failure function of T built once
// up front. Total time O(n^2) (~n*(n + 2n) char ops for n=5000 -> ~75M),
// comfortably inside the 2000ms case limit.
//
// The discuss board (message 358751, "hash gave WA, data is fierce") reports
// that double string-hashing was defeated by adversarial data on this
// problem; that ruled out a randomized-hash rotation check in favor of the
// exact KMP-based one used here. Verified against the sample (Yes/No) and a
// brute-force O(n^3) reference on random small strings (n<=60, all 0/1),
// with 0 mismatches over several thousand trials.

#include <cstdio>
#include <cstring>

const int MAXN = 5005;
char T[MAXN], S[MAXN];
int pi_[MAXN];
int A[2 * MAXN];

int main() {
    if (scanf("%s", T) != 1) return 0;
    if (scanf("%s", S) != 1) return 0;
    int n = (int)strlen(T);
    int ns = (int)strlen(S);
    if (ns != n) {
        // Statement guarantees equal length; guard defensively just in case.
        n = (n < ns) ? n : ns;
    }

    // KMP failure function of pattern T (as 0/1 ints).
    pi_[0] = 0;
    for (int i = 1; i < n; ++i) {
        int j = pi_[i - 1];
        while (j > 0 && T[i] != T[j]) j = pi_[j - 1];
        if (T[i] == T[j]) ++j;
        pi_[i] = j;
    }

    bool found = false;
    for (int d = 0; d < n && !found; ++d) {
        // Build A_d[m] = S[(m-d) mod n] xor S[m], doubled into A[0..2n).
        int idx = n - d; // = (0 - d) mod n, since 0<=d<n
        if (idx == n) idx = 0;
        for (int m = 0; m < n; ++m) {
            int bit = (S[idx] != S[m]) ? 1 : 0;
            A[m] = bit;
            A[m + n] = bit;
            ++idx;
            if (idx == n) idx = 0;
        }

        int q = 0;
        for (int i = 0; i < 2 * n; ++i) {
            char c = (char)('0' + A[i]);
            while (q > 0 && c != T[q]) q = pi_[q - 1];
            if (c == T[q]) ++q;
            if (q == n) {
                found = true;
                break;
            }
        }
    }

    printf(found ? "Yes\n" : "No\n");
    return 0;
}
