// POJ 2188 - Cow Laundry
// Model: claude-sonnet-5
// Approach: The clothes lines are "proper" exactly when the wire in top
// slot i equals the wire in bottom slot i for every i. Adjacent swaps on
// the top holder and adjacent swaps on the bottom holder are independent
// operations, each an adjacent transposition (bubble-sort move) on its own
// permutation. Reaching a common final arrangement tau minimizes
// dist(top,tau) + dist(bottom,tau), where dist is Kendall-tau (adjacent
// swap) distance. Kendall-tau distance is a metric, so by the triangle
// inequality dist(top,tau)+dist(bottom,tau) >= dist(top,bottom), with
// equality when tau = top (or tau = bottom). So the answer is simply the
// Kendall-tau distance between the top and bottom permutations: the
// number of inversions of the permutation obtained by, for each wire,
// pairing its top slot with its bottom slot. Concretely: botPos[w] = slot
// of wire w on the bottom holder; walk the top holder in slot order and
// count inversions of the sequence botPos[topWire(1..N)] with a Fenwick
// tree. No statement ambiguity beyond the standard "it's just inversions"
// (confirmed by the discuss board, which unanimously reduces this to a
// merge-sort/BIT inversion count and warns the generous 1000ms limit
// still lets O(N^2) pass — N<=1000 makes that moot either way; O(N log N)
// used regardless).
#include <cstdio>
#include <vector>
using namespace std;

int bit_[1005];
int N;

void update(int i) {
    for (; i <= N; i += i & (-i)) bit_[i]++;
}

int query(int i) {
    int s = 0;
    for (; i > 0; i -= i & (-i)) s += bit_[i];
    return s;
}

int main() {
    if (scanf("%d", &N) != 1) return 0;
    vector<int> a(N + 1), b(N + 1), botPos(N + 1);
    for (int i = 1; i <= N; i++) {
        scanf("%d %d", &a[i], &b[i]);
        botPos[b[i]] = i;
    }
    // N <= 1000, so max inversions = N*(N-1)/2 <= 499500: fits in int.
    int inv = 0;
    for (int i = 1; i <= N; i++) {
        int v = botPos[a[i]];
        // count how many already-inserted values are > v
        int cnt = query(N) - query(v);
        inv += cnt;
        update(v);
    }
    printf("%d\n", inv);
    return 0;
}
