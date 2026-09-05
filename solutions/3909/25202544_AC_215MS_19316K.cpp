// POJ 3909 - Lucky numbers
// Model: claude-sonnet-5
//
// A "lucky number" uses only digits 4 and 7. A "very lucky number" is any
// product of one or more lucky numbers (a lucky number itself counts).
// Count how many very lucky numbers lie in [A, B], A,B <= 1e12.
//
// Approach: enumerate all lucky numbers with 1..12 digits (2^1+...+2^12-2
// = 8190 of them; 13-digit lucky numbers already exceed 1e12). Then build
// the multiplicative closure of this set intersected with [1, 1e12] via a
// DFS that only ever extends a product by factors at index >= the index
// used to reach it (non-decreasing factor sequence), and memoizes on the
// *value* reached (a std::set) rather than on (index, value): once a value
// is first reached via some starting index idx0, every extension reachable
// from a larger starting index idx1 >= idx0 is already a subset of what
// idx0's continuation will explore, so re-descending from a later idx is
// redundant and safe to skip. This keeps the search to ~3.5e5 nodes
// instead of blowing up combinatorially, verified against the sample
// (counts 0,0,1,10) and the hint's explicit list {4,7,16,28,44,47,49,64,
// 74,77} for [1,100].
//
// Overflow: values can reach ~1e12 and products before the bound check
// could reach ~1e24 if multiplied blindly, so before multiplying we check
// lucky[i] > LIMIT/prod (safe because LIMIT/prod is a floor division: if
// lucky[i] <= floor(LIMIT/prod) then prod*lucky[i] <= LIMIT; otherwise it
// strictly exceeds LIMIT). Needs long long, hence G++ rather than POJ's
// old MSVC (language 4), which has no 64-bit integer type at all.
//
// The resulting closure has 346370 elements; answering each query is a
// pair of binary searches over the sorted result.
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
typedef long long ll;

static const ll LIMIT = 1000000000000LL; // 1e12

vector<ll> lucky;
set<ll> resultSet;

void genLucky() {
    for (int len = 1; len <= 12; len++) {
        int total = 1 << len;
        for (int mask = 0; mask < total; mask++) {
            ll v = 0;
            for (int b = len - 1; b >= 0; b--) {
                int digit = ((mask >> b) & 1) ? 7 : 4;
                v = v * 10 + digit;
            }
            if (v <= LIMIT) lucky.push_back(v);
        }
    }
    sort(lucky.begin(), lucky.end());
    lucky.erase(unique(lucky.begin(), lucky.end()), lucky.end());
}

void rec(int idx, ll prod) {
    for (size_t i = idx; i < lucky.size(); i++) {
        if (lucky[i] > LIMIT / prod) break; // prod * lucky[i] would exceed LIMIT
        ll p = prod * lucky[i];
        if (resultSet.find(p) == resultSet.end()) {
            resultSet.insert(p);
            rec((int)i, p);
        }
    }
}

int main() {
    genLucky();
    rec(0, 1);

    vector<ll> result(resultSet.begin(), resultSet.end()); // already sorted

    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 0; tc < T; tc++) {
        ll A, B;
        scanf("%lld %lld", &A, &B);
        size_t lo = lower_bound(result.begin(), result.end(), A) - result.begin();
        size_t hi = upper_bound(result.begin(), result.end(), B) - result.begin();
        printf("%lld\n", (ll)(hi - lo));
    }
    return 0;
}
