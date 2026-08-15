// POJ 1754 - Buffer Manager
// Model: claude-sonnet-5
// Approach: read N buffer states (digits 0-9 or '*' for locked). Build
// prefix sums of worthiness (locked/free both contribute 0) and, for each
// position, the index of the most recent locked buffer at or before it.
// A window [L, L+K-1] is a valid candidate iff no locked buffer falls in
// it, i.e. lastLocked[L+K-1] < L. Slide L from 1..N-K+1 in O(1) per step
// using the prefix sum, tracking the minimum total worthiness and the
// smallest L achieving it. If N<K or no valid window exists, print 0.
// Statement note: board post 92377 flags N<K as a common missed case
// (already explicit in the statement: "The latter can also happen if
// total number of buffers is less than K").
#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    vector<char> buf(n + 1);
    for (int i = 1; i <= n; ++i) {
        char c;
        scanf(" %c", &c);
        buf[i] = c;
    }

    vector<int> prefix(n + 1, 0);
    vector<int> lastLocked(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        bool locked = (buf[i] == '*');
        int val = locked ? 0 : (buf[i] - '0');
        prefix[i] = prefix[i - 1] + val;
        lastLocked[i] = locked ? i : lastLocked[i - 1];
    }

    if (n < k) {
        printf("0\n");
        return 0;
    }

    int bestSum = -1;
    int bestL = 0;
    for (int L = 1; L + k - 1 <= n; ++L) {
        int R = L + k - 1;
        if (lastLocked[R] >= L) continue; // window contains a locked buffer
        int sum = prefix[R] - prefix[L - 1];
        if (bestSum == -1 || sum < bestSum) {
            bestSum = sum;
            bestL = L;
        }
    }

    printf("%d\n", bestL);
    return 0;
}
