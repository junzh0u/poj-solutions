// POJ 1339 - poker card game
// Model: claude-sonnet-5
// Approach: minimizing sum(card*depth) where cards are placed as leaves of
// an implicit infinite binary tree (START's two branches, each branching further)
// with the constraint that placing a card blocks its whole subtree (antichain
// constraint). This is exactly the classical Huffman coding minimum weighted
// path length problem: repeatedly merge the two smallest remaining values,
// accumulating their sum as cost, until one remains. Verified against all
// three sample cases (43, 34, 110) which independently pin down the model --
// the naive "assign shallow depths freely on both sides" reading gives 33
// instead of 43 for the first sample, so the antichain/blocking rule is load
// bearing.  m==1 is a special case: Huffman's textbook single-leaf tree has
// depth 0, but here a card can never be placed at START itself, so a lone
// card always costs value*1.
//
// The statement bounds the final answer at under 5,000,000, so plain `int`
// (32-bit even under POJ's old C++) suffices throughout; no long long needed.
#include <cstdio>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int m;
        scanf("%d", &m);
        priority_queue<int, vector<int>, greater<int> > pq;
        for (int i = 0; i < m; i++) {
            int v;
            scanf("%d", &v);
            pq.push(v);
        }
        if (m == 0) {
            printf("0\n");
            continue;
        }
        if (m == 1) {
            printf("%d\n", pq.top());
            continue;
        }
        int total = 0;
        while (pq.size() > 1) {
            int a = pq.top(); pq.pop();
            int b = pq.top(); pq.pop();
            int s = a + b;
            total += s;
            pq.push(s);
        }
        printf("%d\n", total);
    }
    return 0;
}
