// POJ 3109 - Inner Vertices
// Model: gpt-5.6-terra
// The one-step closure is the intersection of original row and column spans.
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>

struct Point {
    int x, y, xi;
};

struct Fenwick {
    int n;
    std::vector<int> tree;
    Fenwick(int size) : n(size), tree(size + 1, 0) {}
    void add(int x, int value) {
        while (x <= n) { tree[x] += value; x += x & -x; }
    }
    int sum(int x) const {
        int result = 0;
        while (x > 0) { result += tree[x]; x -= x & -x; }
        return result;
    }
    int range(int left, int right) const {
        return left > right ? 0 : sum(right) - sum(left - 1);
    }
};

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    std::vector<Point> points(n);
    std::map<int, std::pair<int, int> > columns;
    std::map<int, std::pair<int, int> > rows;
    std::vector<int> xs;
    int i;
    for (i = 0; i < n; ++i) {
        std::scanf("%d%d", &points[i].x, &points[i].y);
        xs.push_back(points[i].x);
        if (columns.find(points[i].x) == columns.end()) columns[points[i].x] = std::make_pair(points[i].y, points[i].y);
        else {
            columns[points[i].x].first = std::min(columns[points[i].x].first, points[i].y);
            columns[points[i].x].second = std::max(columns[points[i].x].second, points[i].y);
        }
        if (rows.find(points[i].y) == rows.end()) rows[points[i].y] = std::make_pair(points[i].x, points[i].x);
        else {
            rows[points[i].y].first = std::min(rows[points[i].y].first, points[i].x);
            rows[points[i].y].second = std::max(rows[points[i].y].second, points[i].x);
        }
    }
    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    for (i = 0; i < n; ++i)
        points[i].xi = (int)(std::lower_bound(xs.begin(), xs.end(), points[i].x) - xs.begin()) + 1;

    std::map<int, std::vector<int> > add_at, remove_at;
    std::map<int, std::pair<int, int> >::iterator it;
    for (it = columns.begin(); it != columns.end(); ++it) {
        int xi = (int)(std::lower_bound(xs.begin(), xs.end(), it->first) - xs.begin()) + 1;
        if (it->second.first < it->second.second) {
            add_at[it->second.first].push_back(xi);
            remove_at[it->second.second].push_back(xi);
        }
    }
    Fenwick active((int)xs.size());
    long long candidates = 0;
    for (it = rows.begin(); it != rows.end(); ++it) {
        int y = it->first;
        std::vector<int> &ending = remove_at[y];
        for (i = 0; i < (int)ending.size(); ++i) active.add(ending[i], -1);
        int left = (int)(std::upper_bound(xs.begin(), xs.end(), it->second.first) - xs.begin()) + 1;
        int right = (int)(std::lower_bound(xs.begin(), xs.end(), it->second.second) - xs.begin());
        candidates += active.range(left, right);
        std::vector<int> &starting = add_at[y];
        for (i = 0; i < (int)starting.size(); ++i) active.add(starting[i], 1);
    }

    long long already_black = 0;
    for (i = 0; i < n; ++i) {
        const std::pair<int, int> &r = rows[points[i].y];
        const std::pair<int, int> &c = columns[points[i].x];
        if (r.first < points[i].x && points[i].x < r.second && c.first < points[i].y && points[i].y < c.second)
            ++already_black;
    }
    std::printf("%lld\n", (long long)n + candidates - already_black);
    return 0;
}
