// POJ 3997 - Stock Chase
// Model: claude-sonnet-5
// Approach: maintain the transitive closure of the "can (indirectly) control
// shares in" relation as it grows, one accepted transaction at a time.
// reach[x] is a bitset over companies: bit y set means x can already reach y.
// A pending transaction A->B must be rejected iff B can already reach A
// (reach[B] has bit A set), since accepting it would then close a cycle
// A->B->...->A, letting every company on that cycle indirectly control
// itself. A self-transaction (A == B) is a length-1 cycle and is always
// rejected outright.
// When a transaction is accepted, the new edge A->B extends reachability:
// every company that could already reach A (plus A itself) can now reach
// everything B could already reach (plus B itself). This is applied with a
// bitset OR over at most N rows, N <= 234, so with T <= 100000 transactions
// per case the whole update is O(N^2/64 * T), comfortably within the limit.
// Statement/board note: the board only mentions that the official data is
// weak enough for cubic recomputation to pass; no input-framing pitfalls
// were reported, and A==B is not excluded by the stated bounds, so it is
// handled explicitly as an immediate rejection.
#include <cstdio>
#include <bitset>
using namespace std;

const int MAXN = 235;
bitset<MAXN> reach_[MAXN];

int main() {
    int n, t;
    int caseNum = 1;
    while (scanf("%d %d", &n, &t) == 2 && (n || t)) {
        for (int i = 1; i <= n; ++i) reach_[i].reset();
        int rejected = 0;
        for (int i = 0; i < t; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            if (a == b) {
                ++rejected;
                continue;
            }
            if (reach_[b].test(a)) {
                ++rejected;
                continue;
            }
            bitset<MAXN> tb = reach_[b];
            tb.set(b);
            for (int x = 1; x <= n; ++x) {
                if (x == a || reach_[x].test(a)) {
                    reach_[x] |= tb;
                }
            }
        }
        printf("%d. %d\n", caseNum++, rejected);
    }
    return 0;
}
