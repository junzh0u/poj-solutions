// POJ 3503 - Summits
// Model: gpt-5.6-terra
// Process every cell at the elevation h-d just below its allowed descent.
// A disjoint-set component then contains exactly the points reachable without
// stepping to height at most h-d; the cell is a d-summit iff that component's
// maximum height is h.  Only four orthogonal neighbours are adjacent.
#include <cstdio>
#include <algorithm>

const int MAXN = 250000 + 5;

struct Cell {
    int height;
    int id;
};

int height_[MAXN], parent_[MAXN], rank_[MAXN], maximum_[MAXN];
Cell byHeight[MAXN], queries[MAXN];
int rows, cols, descent;

bool higher(const Cell &a, const Cell &b) {
    return a.height > b.height;
}

int findRoot(int x) {
    int r = x;
    while (parent_[r] != r) r = parent_[r];
    while (parent_[x] != x) {
        int next = parent_[x];
        parent_[x] = r;
        x = next;
    }
    return r;
}

void unite(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    if (a == b) return;
    if (rank_[a] < rank_[b]) std::swap(a, b);
    parent_[b] = a;
    if (rank_[a] == rank_[b]) ++rank_[a];
    if (maximum_[b] > maximum_[a]) maximum_[a] = maximum_[b];
}

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        std::scanf("%d%d%d", &rows, &cols, &descent);
        int n = rows * cols;
        int i;
        for (i = 0; i < n; ++i) {
            std::scanf("%d", &height_[i]);
            byHeight[i].height = height_[i];
            byHeight[i].id = i;
            queries[i].height = height_[i] - descent;
            queries[i].id = i;
            parent_[i] = -1;
        }
        std::sort(byHeight, byHeight + n, higher);
        std::sort(queries, queries + n, higher);

        int next = 0;
        int answer = 0;
        for (i = 0; i < n; ++i) {
            int threshold = queries[i].height;
            while (next < n && byHeight[next].height > threshold) {
                int v = byHeight[next].id;
                parent_[v] = v;
                rank_[v] = 0;
                maximum_[v] = height_[v];
                int r = v / cols;
                int c = v - r * cols;
                if (r > 0 && parent_[v - cols] != -1) unite(v, v - cols);
                if (r + 1 < rows && parent_[v + cols] != -1) unite(v, v + cols);
                if (c > 0 && parent_[v - 1] != -1) unite(v, v - 1);
                if (c + 1 < cols && parent_[v + 1] != -1) unite(v, v + 1);
                ++next;
            }
            int v = queries[i].id;
            if (maximum_[findRoot(v)] == height_[v]) ++answer;
        }
        std::printf("%d\n", answer);
    }
    return 0;
}
