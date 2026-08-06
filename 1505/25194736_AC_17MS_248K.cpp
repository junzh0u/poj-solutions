// POJ 1505 - Copying Books
// Model: claude-sonnet-5
// Approach: binary search the minimal possible maximum load M over [max(p), sum(p)].
// Feasibility check: greedy left-to-right packing counts the minimum number of
// scribes needed for a given cap M; feasible iff that count <= k.
// Once the minimal M is found, construct the actual split that additionally
// minimizes scriber 1's load, then scriber 2's, etc. (the required tie-break):
// process scribes from the LAST one backwards, each time greedily pulling as
// many trailing books as fit under M while still leaving at least one book per
// remaining (earlier) scriber. Pushing as much work as possible onto later
// scribes first is exactly what minimizes the earlier scribes' loads in
// lexicographic order. Verified by hand against both sample cases.
// Ambiguity: statement doesn't say whether a trailing space after the last
// number is required; the discuss board has both claims but the sample output
// has none, so this solution emits none. All sums fit in a 64-bit integer
// (m <= 500, p_i < 1e7 => total < 5e9, overflows 32-bit).
#include <cstdio>
using namespace std;

typedef long long ll;

int m, k;
ll p[505];

int minParts(ll cap) {
    int parts = 1;
    ll cur = 0;
    for (int i = 1; i <= m; ++i) {
        if (cur + p[i] > cap) {
            ++parts;
            cur = p[i];
        } else {
            cur += p[i];
        }
    }
    return parts;
}

int boundary[505]; // boundary[s] = right end (index) of scribe s's range

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        scanf("%d %d", &m, &k);
        ll lo = 0, hi = 0;
        for (int i = 1; i <= m; ++i) {
            scanf("%lld", &p[i]);
            if (p[i] > lo) lo = p[i];
            hi += p[i];
        }
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (minParts(mid) <= k) hi = mid;
            else lo = mid + 1;
        }
        ll M = lo;

        boundary[k] = m;
        int cur = m;
        for (int s = k; s >= 2; --s) {
            ll sum = 0;
            while (cur >= s && sum + p[cur] <= M) {
                sum += p[cur];
                --cur;
            }
            boundary[s - 1] = cur;
        }
        // boundary[0] = 0 implicitly (scribe 1 starts at book 1)

        bool first = true;
        int left = 1;
        for (int s = 1; s <= k; ++s) {
            int right = boundary[s];
            for (int i = left; i <= right; ++i) {
                if (!first) printf(" ");
                printf("%lld", p[i]);
                first = false;
            }
            if (s != k) printf(" /");
            left = right + 1;
        }
        printf("\n");
    }
    return 0;
}
