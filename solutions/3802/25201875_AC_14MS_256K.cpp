// POJ 3802 - Cubist Artwork
// Model: qwen3.8-27b-mlx@4bit
// Minimum cubes = sum(front) + sum(side) - sum over v of v * min(countFront[v], countSide[v]).
// Every column j must contain one cell of height exactly h_j (its front-view maximum),
// placed in a row whose side-view maximum is at least h_j; every row i must contain one
// cell of height exactly h'_i in a column whose front-view maximum is at least h'_i.
// Any other cell only adds cost, so an optimum has exactly one non-zero cell per column.
// A single cell can serve both requirements only when h_j == h'_i, saving v cubes per
// such shared cell; pairing equal values greedily is optimal because all values are
// positive and a column (or row) can share with at most one partner. Feasibility,
// guaranteed by the statement (max(front) == max(side)), makes every unpaired
// requirement placeable in a line of the global maximum. The formula is symmetric in
// the two views, so the front/side orientation of the figures cannot affect it.
#include <cstdio>

int main() {
    int w, d;
    while (scanf("%d %d", &w, &d) == 2 && (w || d)) {
        int cnt[101] = {0};
        int cntp[101] = {0};
        int sum = 0;
        for (int i = 0; i < w; ++i) {
            int h;
            scanf("%d", &h);
            if (h >= 0 && h < 101) cnt[h]++;
            sum += h;
        }
        for (int i = 0; i < d; ++i) {
            int h;
            scanf("%d", &h);
            if (h >= 0 && h < 101) cntp[h]++;
            sum += h;
        }
        int save = 0;
        for (int v = 1; v < 101; ++v) {
            int m = cnt[v] < cntp[v] ? cnt[v] : cntp[v];
            save += v * m;
        }
        printf("%d\n", sum - save);
    }
    return 0;
}
