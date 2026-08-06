// POJ 3636 - Nested Dolls
// Model: claude-opus-5
//
// Doll a nests into b iff wa < wb and ha < hb, a strict partial order.  The
// answer is the minimum number of chains covering all dolls, which by
// Dilworth's theorem equals the size of the largest antichain.
//
// Sort by width ascending, height descending on ties.  For i < j in that order
// wi <= wj, so the pair is incomparable exactly when hi >= hj: equal widths are
// always incomparable and the tie-break already forces hi >= hj there, while
// for wi < wj incomparability is precisely hi >= hj.  So an antichain is
// exactly a non-increasing subsequence of h, and the answer is the longest one.
// (The h-descending tie-break is load-bearing: sorting ties the other way would
// let two equal-width dolls be counted as a chain.)
//
// Longest non-increasing subsequence of h = longest non-decreasing subsequence
// of -h, computed in O(m log m) with the usual patience/tails array and
// upper_bound.  m <= 20000 over <= 20 cases with a 1000MS limit, and the
// discuss board reports TLE for O(m^2), hence the O(m log m) pass plus a
// getchar-based reader for the 800000 integers of a worst-case input.

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;

struct Doll { int w, h; };

static bool cmpDoll(const Doll &a, const Doll &b) {
    if (a.w != b.w) return a.w < b.w;
    return a.h > b.h;
}

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return -1;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sign;
}

int main() {
    int t = readInt();
    while (t-- > 0) {
        int m = readInt();
        vector<Doll> d(m);
        for (int i = 0; i < m; i++) { d[i].w = readInt(); d[i].h = readInt(); }
        sort(d.begin(), d.end(), cmpDoll);
        vector<int> tails;
        tails.reserve(m);
        for (int i = 0; i < m; i++) {
            int x = -d[i].h;
            vector<int>::iterator it = upper_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) tails.push_back(x);
            else *it = x;
        }
        printf("%d\n", (int)tails.size());
    }
    return 0;
}
