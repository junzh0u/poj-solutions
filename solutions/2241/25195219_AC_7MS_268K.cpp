// POJ 2241 - The Tower of Babylon
// Model: claude-opus-5
//
// Each block type has an unlimited supply and may be reoriented, so a type
// contributes 6 ordered orientations (which dimension is the height, times
// which of the remaining two is the first base dimension).  A tower is a
// sequence of orientations whose base dimensions decrease strictly in BOTH
// corresponding coordinates, so no orientation can repeat and the unlimited
// supply matters only in that several orientations of one type may appear in
// one tower.
//
// Build the 6n orientations, sort by (w, l) ascending, and run the O((6n)^2)
// longest-path DP: best[i] = h_i + max{ best[j] : w_j < w_i && l_j < l_i },
// where best[i] is the tallest tower whose bottom block is orientation i.
// Sorting lexicographically is enough for the DP order because w_j < w_i puts
// every candidate top block j strictly before i (and it avoids the w*l product
// an area sort would need).
//
// Because the base may be rotated freely, comparing ordered pairs over all 6
// orientations is equivalent to comparing sorted pairs over 3; the 6-orientation
// form is used since it transcribes the statement's wording directly.
//
// The statement gives no bound on xi, yi, zi (the discuss board notes this), but
// the answer is a sum of at most 3 dimensions per type over at most 30 types and
// every accepted solution on the board uses int, so int is used here too --
// which also keeps the source clear of long long, which POJ's "C++" compiler and
// strict C++98 both dislike.

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Ori {
    int w, l, h;
};

static bool byBase(const Ori &a, const Ori &b) {
    if (a.w != b.w) return a.w < b.w;
    return a.l < b.l;
}

int main() {
    ios_base::sync_with_stdio(false);

    int n;
    int caseNo = 1;
    while (cin >> n && n != 0) {
        vector<Ori> o;
        o.reserve(6 * n);
        for (int i = 0; i < n; ++i) {
            int d[3];
            cin >> d[0] >> d[1] >> d[2];
            for (int hi = 0; hi < 3; ++hi) {
                int a = (hi + 1) % 3, b = (hi + 2) % 3;
                Ori p;
                p.h = d[hi];
                p.w = d[a]; p.l = d[b];
                o.push_back(p);
                p.w = d[b]; p.l = d[a];
                o.push_back(p);
            }
        }
        sort(o.begin(), o.end(), byBase);

        int m = (int)o.size();
        vector<int> best(m, 0);
        int ans = 0;
        for (int i = 0; i < m; ++i) {
            int top = 0;
            for (int j = 0; j < i; ++j) {
                if (o[j].w < o[i].w && o[j].l < o[i].l && best[j] > top)
                    top = best[j];
            }
            best[i] = top + o[i].h;
            if (best[i] > ans) ans = best[i];
        }

        cout << "Case " << caseNo << ": maximum height = " << ans << "\n";
        ++caseNo;
    }
    return 0;
}
