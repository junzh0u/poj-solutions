// POJ 1486 - Sorting Slides
// Model: claude-opus-5
//
// Bipartite graph: slide i (letter 'A'+i) -- number j+1 when the number's
// point lies inside the slide's bounding rectangle.  A pair is "uniquely
// determined" exactly when it belongs to every maximum matching, so: build one
// maximum matching M with Kuhn's algorithm, then for each edge of M delete it
// and rematch; if the maximum matching shrinks, that edge is forced and gets
// printed.  A greedy "repeatedly peel a degree-1 vertex" scheme is not
// equivalent and fails on cases like the 5-slide one on the discuss board
// (answer (C,3), greedy says none).
//
// Ambiguity: the statement promises no number lies on a slide boundary, so the
// inside test can be inclusive or strict; inclusive is used here since the two
// agree on legal input.
//
// Output: "Heap k", then the forced pairs ordered by letter separated by single
// spaces (no trailing space) or "none", then a blank line after every heap.

#include <cstdio>
#include <cstring>

const int MAXN = 128;

int n;
int xmin_[MAXN], xmax_[MAXN], ymin_[MAXN], ymax_[MAXN];
bool adj[MAXN][MAXN];     /* adj[slide][number] */
int matchSlide[MAXN];     /* slide -> number, -1 if free */
int matchNum[MAXN];       /* number -> slide, -1 if free */
bool used[MAXN];          /* visited numbers in the current augmentation */
int banSlide, banNum;     /* edge temporarily removed, -1 for none */

static bool augment(int s)
{
    for (int j = 0; j < n; ++j) {
        if (!adj[s][j] || used[j]) continue;
        if (s == banSlide && j == banNum) continue;
        used[j] = true;
        if (matchNum[j] == -1 || augment(matchNum[j])) {
            matchNum[j] = s;
            matchSlide[s] = j;
            return true;
        }
    }
    return false;
}

static int maxMatching()
{
    int res = 0;
    for (int i = 0; i < n; ++i) matchSlide[i] = -1;
    for (int j = 0; j < n; ++j) matchNum[j] = -1;
    for (int i = 0; i < n; ++i) {
        memset(used, 0, sizeof(bool) * n);
        if (augment(i)) ++res;
    }
    return res;
}

int main()
{
    int heap = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        ++heap;
        for (int i = 0; i < n; ++i)
            scanf("%d %d %d %d", &xmin_[i], &xmax_[i], &ymin_[i], &ymax_[i]);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                adj[i][j] = false;
        for (int j = 0; j < n; ++j) {
            int x, y;
            scanf("%d %d", &x, &y);
            for (int i = 0; i < n; ++i)
                if (x >= xmin_[i] && x <= xmax_[i] && y >= ymin_[i] && y <= ymax_[i])
                    adj[i][j] = true;
        }

        banSlide = banNum = -1;
        int best = maxMatching();

        int base[MAXN];
        for (int i = 0; i < n; ++i) base[i] = matchSlide[i];

        printf("Heap %d\n", heap);
        bool any = false;
        for (int i = 0; i < n; ++i) {
            if (base[i] == -1) continue;
            banSlide = i;
            banNum = base[i];
            int without = maxMatching();
            banSlide = banNum = -1;
            if (without < best) {
                if (any) putchar(' ');
                printf("(%c,%d)", (char)('A' + i), base[i] + 1);
                any = true;
            }
        }
        if (!any) printf("none");
        printf("\n\n");
    }
    return 0;
}
