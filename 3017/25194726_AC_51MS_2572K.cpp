// POJ 3017 - Cut the Sequence
// Model: claude-sonnet-5
//
// Approach: dp[i] = min cost to optimally cut the first i elements, dp[0] = 0.
// dp[i] = min over j in [L(i), i-1] of dp[j] + max(a[j+1..i]), where L(i) is the
// smallest window start with sum(a[L+1..i]) <= M (two-pointer, valid since a_i >= 0
// makes prefix sums monotone). If a[i] > M the element can never fit in any single
// valid part, so the whole sequence is infeasible -> -1.
//
// Key facts used:
//  - dp[] is non-decreasing in i. Proof sketch: given an optimal cutting of the
//    first i+1 elements whose last part is a[k+1..i+1], splitting it into
//    a[k+1..i] + {a[i+1]} keeps sum(a[k+1..i]) <= sum(a[k+1..i+1]) <= M (a>=0) and
//    max(a[k+1..i]) <= max(a[k+1..i+1]), so dropping the trailing element a[i+1]
//    entirely gives a valid cutting of the first i elements with cost <= dp[i+1].
//    Hence dp[i] <= dp[i+1].
//  - Maintain a monotonic deque of groups (val, pos) covering the current valid
//    j-range [L, i-1], val strictly decreasing from front to back, where each
//    group's pos is the smallest j in its range (so, by dp monotonicity, dp[pos]
//    is the minimal dp[j] over the whole group). On adding a[i], pop back groups
//    with val <= a[i] and merge (keep the smallest pos seen). On the two-pointer
//    advancing L, either drop a group entirely (its whole range < L) or shrink the
//    front group in place by resetting its pos to L.
//  - A multiset of keys (val + dp[pos]) over all groups gives dp[i] = *ms.begin()
//    in O(log n) amortized per operation; total O(n log n).
//
// M needs a 64-bit type per the problem's hint (sums of up to 1e5 * 1e6 values
// already exceed 32-bit range, and M itself may be given as a large 64-bit value).

#include <cstdio>
#include <deque>
#include <set>
#include <algorithm>
using namespace std;

typedef long long ll;
const int MAXN = 100000 + 5;

int n;
ll M;
ll a[MAXN];
ll S[MAXN];
ll dp[MAXN];

struct Node {
    ll val;
    int pos;
};

int main() {
    if (scanf("%d %lld", &n, &M) != 2) return 0;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
        S[i] = S[i - 1] + a[i];
    }

    dp[0] = 0;
    deque<Node> dq;
    multiset<ll> ms;

    int L = 0;
    bool infeasible = false;

    for (int i = 1; i <= n; i++) {
        while (S[i] - S[L] > M) L++;
        if (L > i - 1) { infeasible = true; break; }

        // push candidate group for j = i-1, merging back groups with val <= a[i]
        ll newval = a[i];
        int bestpos = i - 1;
        while (!dq.empty() && dq.back().val <= newval) {
            bestpos = dq.back().pos; // keeps shrinking to the smallest pos merged
            ll oldkey = dq.back().val + dp[dq.back().pos];
            multiset<ll>::iterator it = ms.find(oldkey);
            if (it != ms.end()) ms.erase(it);
            dq.pop_back();
        }
        Node nd;
        nd.val = newval;
        nd.pos = bestpos;
        ms.insert(nd.val + dp[nd.pos]);
        dq.push_back(nd);

        // evict / shrink front groups per current L
        while (!dq.empty()) {
            Node &front = dq.front();
            int rb = (dq.size() >= 2) ? (dq[1].pos - 1) : (i - 1);
            if (rb < L) {
                ll oldkey = front.val + dp[front.pos];
                multiset<ll>::iterator it = ms.find(oldkey);
                if (it != ms.end()) ms.erase(it);
                dq.pop_front();
            } else if (front.pos < L) {
                ll oldkey = front.val + dp[front.pos];
                multiset<ll>::iterator it = ms.find(oldkey);
                if (it != ms.end()) ms.erase(it);
                front.pos = L;
                ms.insert(front.val + dp[front.pos]);
                break;
            } else {
                break;
            }
        }

        dp[i] = *ms.begin();
    }

    if (infeasible) {
        printf("-1\n");
    } else {
        printf("%lld\n", dp[n]);
    }
    return 0;
}
