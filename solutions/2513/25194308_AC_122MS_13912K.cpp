// POJ 2513 - Colored Sticks
//
// Model each color as a graph vertex and each stick as an edge between the
// colors of its two endpoints. Aligning all sticks in one line is exactly
// finding an Eulerian path through this multigraph, which exists iff:
//   1. every edge lies in a single connected component (ignoring colors that
//      never appear), and
//   2. at most two vertices have odd degree.
// A self-colored stick ("red red") is a self-loop: it adds 2 to that
// vertex's degree and is trivially placeable on its own.
//
// With up to 250000 sticks (500000 endpoint words), a std::map<string,int>
// is too slow. Each word is <=10 lowercase letters, so it is encoded as a
// bijective base-27 integer (digits 1..26, no padding) which is a unique,
// collision-free id computable in O(length). All codes are then sorted and
// uniqued once to assign compact vertex ids via binary search, and
// connectivity/degree are tracked with a union-find plus a degree array.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXM = 250005;
static long long Ucode[MAXM], Vcode[MAXM];
static long long allCodes[2*MAXM];
static int parent_[2*MAXM];
static int rank_[2*MAXM];
static int degree[2*MAXM];

int find(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

void unite(int x, int y) {
    x = find(x); y = find(y);
    if (x == y) return;
    if (rank_[x] < rank_[y]) swap(x, y);
    parent_[y] = x;
    if (rank_[x] == rank_[y]) rank_[x]++;
}

long long encode(const char *s) {
    long long code = 0;
    for (int i = 0; s[i]; ++i) {
        code = code * 27 + (s[i] - 'a' + 1);
    }
    return code;
}

int main() {
    static char bufu[16], bufv[16];
    int m = 0;
    while (scanf("%15s %15s", bufu, bufv) == 2) {
        Ucode[m] = encode(bufu);
        Vcode[m] = encode(bufv);
        m++;
    }

    int total = 0;
    for (int i = 0; i < m; i++) {
        allCodes[total++] = Ucode[i];
        allCodes[total++] = Vcode[i];
    }
    sort(allCodes, allCodes + total);
    int n = (int)(unique(allCodes, allCodes + total) - allCodes);

    for (int i = 0; i < n; i++) {
        parent_[i] = i;
        rank_[i] = 0;
        degree[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        int a = (int)(lower_bound(allCodes, allCodes + n, Ucode[i]) - allCodes);
        int b = (int)(lower_bound(allCodes, allCodes + n, Vcode[i]) - allCodes);
        unite(a, b);
        degree[a]++;
        degree[b]++;
    }

    bool possible = true;
    if (n > 0) {
        int root0 = find(0);
        int oddCount = 0;
        for (int i = 0; i < n; i++) {
            if (find(i) != root0) possible = false;
            if (degree[i] % 2 == 1) oddCount++;
        }
        if (oddCount > 2) possible = false;
    }

    printf(possible ? "Possible\n" : "Impossible\n");
    return 0;
}
