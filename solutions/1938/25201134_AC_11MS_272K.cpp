// POJ 1938 - California Jones and the Gate to Freedom
// Model: claude-sonnet-5
//
// Approach: The stone board figure shows that, for n stones labeled by
// their input position (1..n), all C(n, n/2) subsets of size n/2 are
// enumerated in standard ascending lexicographic order of their sorted
// position-sequences (e.g. for n=4: {1,2},{1,3},{1,4},{2,3},{2,4},{3,4}),
// and consecutive binary numbers 0,1,2,... (as plain unsigned integers,
// not zero-padded to a fixed width) index into that enumeration.
//
// For each sub-test we are given a bit string b (interpreted as an
// unsigned integer m) and a chosen set of n/2 stone VALUES. We map each
// chosen value back to its 1-indexed input position, sort the positions
// ascending, and compute that combination's rank in the same lexicographic
// order using the standard combinatorial ranking formula:
//   rank = sum_{i=1..k} sum_{v=prev+1}^{p_i-1} C(n-v, k-i)
// where p_1<...<p_k are the sorted chosen positions and prev starts at 0.
// The answer is TRUE iff rank == m.
//
// This was verified by hand against every sub-test of the sample (all 5
// lines reproduce exactly), including cases with n=4 and n=8, k=2 and k=4.
// Binomials up to C(32,16) (~6e8) are precomputed with long long, and the
// bit string (length <= 30) is parsed manually into a long long, so no
// overflow is possible within the stated limits (n <= 32).

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

static long long C[40][40];

int main(){
    for (int i = 0; i < 40; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            C[i][j] = C[i-1][j-1] + (j <= i-1 ? C[i-1][j] : 0);
        }
        for (int j = i+1; j < 40; ++j) C[i][j] = 0;
    }

    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<int> stones(n+1); // 1-indexed
        for (int i = 1; i <= n; ++i) scanf("%d", &stones[i]);
        int k;
        scanf("%d", &k);
        int half = n / 2;
        for (int t = 0; t < k; ++t) {
            char buf[40];
            scanf("%s", buf);
            long long m = 0;
            for (int i = 0; buf[i]; ++i) m = m * 2 + (buf[i] - '0');

            vector<int> pos(half);
            for (int i = 0; i < half; ++i) {
                int v;
                scanf("%d", &v);
                int p = -1;
                for (int j = 1; j <= n; ++j) if (stones[j] == v) { p = j; break; }
                pos[i] = p;
            }
            sort(pos.begin(), pos.end());

            long long rnk = 0;
            int prev = 0;
            for (int i = 1; i <= half; ++i) {
                int pi = pos[i-1];
                for (int v = prev + 1; v <= pi - 1; ++v) {
                    rnk += C[n - v][half - i];
                }
                prev = pi;
            }

            printf(rnk == m ? "TRUE\n" : "FALSE\n");
        }
    }
    return 0;
}
