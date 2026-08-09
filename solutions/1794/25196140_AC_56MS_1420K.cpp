// POJ 1794 - Castle Walls
// Model: gpt-5.6-terra
// Sweep peasants in line order and use two Fenwick trees to count opposite
// colored earlier hooks whose wall position is not smaller.

#include <algorithm>
#include <cstdio>
#include <vector>

struct Hook {
    int line;
    int wall;
    int color;
};

bool by_line(const Hook &a, const Hook &b) {
    return a.line < b.line;
}

struct Fenwick {
    std::vector<int> tree;
    Fenwick(int n) : tree(n + 1, 0) {}
    void add(int x) {
        for (; x < (int)tree.size(); x += x & -x) ++tree[x];
    }
    int sum(int x) const {
        int result = 0;
        for (; x > 0; x -= x & -x) result += tree[x];
        return result;
    }
};

int main() {
    int cases;
    if (std::scanf("%d", &cases) != 1) return 0;
    for (int scenario = 1; scenario <= cases; ++scenario) {
        int n, m;
        std::scanf("%d%d", &n, &m);
        int total = n + m;
        std::vector<Hook> hooks(total);
        for (int i = 0; i < total; ++i) {
            std::scanf("%d%d", &hooks[i].line, &hooks[i].wall);
            hooks[i].color = i < n ? 0 : 1;
        }
        std::sort(hooks.begin(), hooks.end(), by_line);
        Fenwick seen[2] = {Fenwick(total), Fenwick(total)};
        int count[2] = {0, 0};
        long long crossings = 0;
        for (int i = 0; i < total; ++i) {
            int other = 1 - hooks[i].color;
            crossings += count[other] - seen[other].sum(hooks[i].wall - 1);
            seen[hooks[i].color].add(hooks[i].wall);
            ++count[hooks[i].color];
        }
        std::printf("Scenario #%d:\n%lld\n\n", scenario, crossings);
    }
    return 0;
}
