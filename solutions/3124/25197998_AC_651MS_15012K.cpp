// POJ 3124 - The Bookcase
// Model: claude-sonnet-5
//
// Approach: Sort books by height descending. WLOG the globally tallest book
// (index 0 after sorting) belongs to shelf A; its height h0 is automatically
// the max of whatever else ends up in A, so A's height contribution is fixed.
// For the remaining n-1 books (processed in decreasing height order), do a
// 2-dimensional 0/1 "knapsack" DP over (widthB, widthC): f[j][k] = minimum
// possible hB+hC (heights of shelf B and shelf C) achievable when the books
// assigned so far to B total width j and to C total width k (books not
// assigned to B or C fall through to shelf A implicitly, since f[j][k] simply
// keeps its old value when a book isn't used for a transition). Because books
// are processed in decreasing height order, the first book ever placed into
// an empty shelf (width 0 -> width t_i) is necessarily that shelf's tallest
// book, so the height cost h_i is added exactly on that empty->t_i
// transition (f[0][k] -> f[t_i][k], and symmetrically for k). The j (resp. k)
// loop runs downward so each book is used at most once per shelf (standard
// 0/1 knapsack in-place trick), and a book can update at most one of B/C in
// a given transition (never both), matching "each book goes to exactly one
// shelf". Finally for every reachable (j,k) with j>=1,k>=1 (B,C non-empty),
// answer candidate = (f[j][k]+h0) * max(S-j-k+t0, max(j,k)), where S is the
// total thickness of the n-1 non-A-forced books and t0 the tallest book's
// thickness (shelf A's width = t0 + everything not sent to B or C).
//
// No statement ambiguity found; sample matches directly. N<=70, thickness
// sum S<=70*30=2100 fits comfortably in the O(n*S^2) DP within the 3000ms
// limit (verified locally on the worst case: N=70, all thickness=30).
//
// f[j][k] is symmetric under swapping j<->k (the DP treats the B and C
// dimensions identically), so only the upper triangle j<=k is stored and
// updated; any read that would fall in the lower triangle is mirrored via
// getF(). This halves the DP's inner-loop work.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXS = 2101;
int f[MAXS][MAXS];

inline int getF(int a, int b) {
    return (a <= b) ? f[a][b] : f[b][a];
}

struct Book {
    int h, t;
};

bool cmpDesc(const Book &a, const Book &b) {
    return a.h > b.h;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        Book books[70];
        for (int i = 0; i < n; i++) scanf("%d %d", &books[i].h, &books[i].t);
        sort(books, books + n, cmpDesc);

        int h0 = books[0].h, t0 = books[0].t;
        int S = 0;
        for (int i = 1; i < n; i++) S += books[i].t;

        for (int j = 0; j <= S; j++)
            for (int k = j; k <= S; k++)
                f[j][k] = 0x3f3f3f3f;
        f[0][0] = 0;

        int sum = 0;
        for (int i = 1; i < n; i++) {
            int ti = books[i].t, hi = books[i].h;
            sum += ti;
            for (int j = sum; j >= 0; j--) {
                for (int k = sum; k >= j; k--) {
                    int &cell = f[j][k];
                    if (j > ti) {
                        int cand = getF(j - ti, k);
                        if (cand < cell) cell = cand;
                    } else if (j == ti) {
                        int cand = getF(0, k) + hi;
                        if (cand < cell) cell = cand;
                    }
                    if (k > ti) {
                        int cand = getF(j, k - ti);
                        if (cand < cell) cell = cand;
                    } else if (k == ti) {
                        int cand = getF(j, 0) + hi;
                        if (cand < cell) cell = cand;
                    }
                }
            }
        }

        int best = -1;
        for (int j = 1; j <= S; j++) {
            for (int k = j; k <= S; k++) {
                if (f[j][k] >= 0x3f3f3f3f) continue;
                int height = f[j][k] + h0;
                int width = S - j - k + t0;
                if (j > width) width = j;
                if (k > width) width = k;
                int area = height * width;
                if (best == -1 || area < best) best = area;
            }
        }
        printf("%d\n", best);
    }
    return 0;
}
