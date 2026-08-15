// POJ 3688 - Cheat in the Game
// Model: claude-sonnet-5
//
// The card draws are without replacement from a shared box; within a turn a
// player keeps drawing until a card fits the remaining pile (or the box runs
// out, which resets the whole game to the start and is irrelevant to the
// binary "can this happen at all" question we need). Because the pile after
// using cards c_1..c_k in any order equals W - (partial sums), and the last
// card must exactly zero the pile, a candidate sequence is feasible for ANY
// ordering the moment its values are a subset of the N cards summing exactly
// to W: c_i <= W - sum_{j<i} c_j always holds automatically once the total
// equals W (the remaining suffix sum, which includes c_i itself, is >= c_i
// since all values are positive). So order never matters - only whether a
// subset of a given size (parity) sums to W.
//
// Turn k (1-indexed) belongs to Alice iff k is odd. So:
//   Alice can be forced to win W  <=>  some ODD-size subset sums to W
//   Bob can ever win W            <=>  some EVEN-size subset sums to W
// "surely win" means: odd-size subset sums to W, and NO even-size subset
// sums to W. Sample W=3 with cards {1,5,7}: no subset at all sums to 3, so
// the game simply never decides (matches the board's confirmed discussion).
//
// This reduces to a 0/1 subset-sum DP tracking reachability by parity of
// subset size, computed with two bitsets (even-size-reachable state and
// odd-size-reachable state) updated per item from their PRE-item snapshots
// (classic 0/1 knapsack semantics), giving O(N*M/64) per test case.

#include <cstdio>
#include <bitset>
using namespace std;

static const int MAXM = 100001; // W in [0,100000]

int main(){
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        bitset<MAXM> even_r, odd_r, tmp;
        even_r.set(0);
        for (int i = 0; i < n; i++) {
            int a;
            scanf("%d", &a);
            tmp = even_r;
            even_r |= (odd_r << a);
            odd_r  |= (tmp << a);
        }
        int ans = 0;
        for (int w = 1; w <= m; w++) {
            if (odd_r.test(w) && !even_r.test(w)) ans++;
        }
        printf("%d\n", ans);
    }
    return 0;
}
