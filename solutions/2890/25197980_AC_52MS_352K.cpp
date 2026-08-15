// POJ 2890 - Matrix Multiplication
// Model: claude-sonnet-5
//
// The matrix A is boolean (OR/AND semiring) and every diagonal entry is
// forced to 1, so A represents "reach along an edge, or stay put" in one
// step. That makes the sequence of boolean powers A, A^2, A^3, ... monotone
// non-decreasing entrywise (A^n <= A^(n+1) because staying put is always an
// option), and it stabilizes once every reachable pair has been reached by
// some path of length < K (K < 1000 nodes, so the diameter of any simple
// path is at most K-1). Since 2006 > K always (K < 1000), A^2006 equals the
// full transitive closure of the directed graph (i * i is always reachable
// too, regardless of edges, because of the forced diagonal). The answer is
// just the number of (i, j) pairs with j reachable from i.
//
// Verified against the sample by hand: K=3 with edges 1->2, 2->1, 0->1,
// 0->2 (plus self loops) gives reachable-set sizes 3+2+2 = 7, matching the
// expected output.
//
// Computed via a bitset-accelerated Floyd-Warshall transitive closure:
// for k, for i with bit k set in reach[i], reach[i] |= reach[k]. That is
// O(K^3/64), which the discuss board confirms is needed -- a scalar
// O(K^3) closure was reported to TLE.
//
// Input is read to EOF (no test-case count is given); the board confirms
// this (one poster TLE'd from missing the "!=EOF" loop condition) and that
// "K+M elements are 1's" means K diagonal ones plus M extra edges.
#include <cstdio>
#include <bitset>
using namespace std;

const int MAXK = 1000;
bitset<MAXK> reach_[MAXK];

int main() {
    int K, M;
    while (scanf("%d %d", &K, &M) == 2) {
        for (int i = 0; i < K; i++) {
            reach_[i].reset();
            reach_[i].set(i);
        }
        for (int e = 0; e < M; e++) {
            int i, j;
            scanf("%d %d", &i, &j);
            reach_[i].set(j);
        }
        for (int k = 0; k < K; k++) {
            for (int i = 0; i < K; i++) {
                if (reach_[i][k]) reach_[i] |= reach_[k];
            }
        }
        // K < 1000, so the total is at most ~999*999, well within int range.
        long total = 0;
        for (int i = 0; i < K; i++) total += (long)reach_[i].count();
        printf("%ld\n", total);
    }
    return 0;
}
