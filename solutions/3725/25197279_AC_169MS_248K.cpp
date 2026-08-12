// POJ 3725 - I know the k-th integer
// Model: gpt-5.6-terra
// Count the integers not greater than n that are lexicographically before m,
// then binary-search the first n whose rank of m reaches k.
#include <climits>
#include <iostream>
#include <string>

using namespace std;

typedef long long int64;

int64 capped_add(int64 a, int64 b, int64 cap) {
    if (a >= cap - b) return cap;
    return a + b;
}

// Number of positive integers x <= n whose decimal representation starts with p.
int64 subtree_count(int64 p, int64 n, int64 cap) {
    int64 ans = 0;
    int64 next = p + 1;
    while (p <= n) {
        int64 upper = next < n + 1 ? next : n + 1;
        ans = capped_add(ans, upper - p, cap);
        if (ans == cap || p > n / 10) break;
        p *= 10;
        if (next > n / 10) next = n + 1;
        else next *= 10;
    }
    return ans;
}

// The 1-based lexicographic rank of m in [1,n], capped above cap.
int64 rank_of(int64 n, const string& s, int64 cap) {
    int64 rank = 1; // m itself
    int64 prefix = 0;
    int64 i;
    for (i = 0; i < (int64)s.size(); ++i) {
        int digit = s[(size_t)i] - '0';
        int first = i == 0 ? 1 : 0;
        int d;
        for (d = first; d < digit; ++d) {
            int64 q = prefix * 10 + d;
            rank = capped_add(rank, subtree_count(q, n, cap), cap);
            if (rank == cap) return rank;
        }
        if (i != 0) {
            rank = capped_add(rank, 1, cap); // the proper prefix itself
            if (rank == cap) return rank;
        }
        prefix = prefix * 10 + digit;
    }
    return rank;
}

int main() {
    int64 k, m;
    const int64 INF = 4000000000000000000LL;
    while (cin >> k >> m) {
        string s;
        int64 x = m;
        do {
            s = char('0' + x % 10) + s;
            x /= 10;
        } while (x != 0);
        int64 cap = k == LLONG_MAX ? LLONG_MAX : k + 1;
        if (rank_of(m, s, cap) > k || rank_of(INF, s, cap) < k) {
            cout << 0 << '\n';
            continue;
        }
        int64 lo = m, hi = INF;
        while (lo < hi) {
            int64 mid = lo + (hi - lo) / 2;
            if (rank_of(mid, s, cap) >= k) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << '\n';
    }
    return 0;
}
