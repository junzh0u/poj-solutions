// POJ 3416 - Crossing
// Model: gpt-5.6-terra
// Offline dominance counting: for every query, sweep points with px < x and
// use a Fenwick tree to count py < y.  The three prefix counts determine the
// signed difference between the diagonal pairs of quadrants.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
};

struct Query {
    int x;
    int y;
    int id;
};

struct Fenwick {
    vector<int> tree;

    Fenwick(int n) : tree(n + 1, 0) {}

    void add(int pos) {
        ++pos;
        while (pos < (int)tree.size()) {
            ++tree[pos];
            pos += pos & -pos;
        }
    }

    int sum(int pos) const {
        int result = 0;
        while (pos > 0) {
            result += tree[pos];
            pos -= pos & -pos;
        }
        return result;
    }
};

bool byXPoint(const Point &a, const Point &b) {
    return a.x < b.x;
}

bool byYPoint(const Point &a, const Point &b) {
    return a.y < b.y;
}

bool byXQuery(const Query &a, const Query &b) {
    return a.x < b.x;
}

int main() {
    int testCases;
    if (scanf("%d", &testCases) != 1) return 0;

    for (int tc = 0; tc < testCases; ++tc) {
        int n, m;
        scanf("%d%d", &n, &m);
        vector<Point> points(n);
        for (int i = 0; i < n; ++i) scanf("%d%d", &points[i].x, &points[i].y);

        vector<Query> queries(m);
        for (int i = 0; i < m; ++i) {
            scanf("%d%d", &queries[i].x, &queries[i].y);
            queries[i].id = i;
        }

        vector<Point> byX = points;
        vector<Point> byY = points;
        vector<Query> ordered = queries;
        sort(byX.begin(), byX.end(), byXPoint);
        sort(byY.begin(), byY.end(), byYPoint);
        sort(ordered.begin(), ordered.end(), byXQuery);

        vector<int> answer(m);
        Fenwick bit(500001);
        int left = 0;
        int pointIndex = 0;
        for (int i = 0; i < m; ++i) {
            const Query &q = ordered[i];
            while (left < n && byX[left].x < q.x) ++left;
            while (pointIndex < n && byX[pointIndex].x < q.x) {
                bit.add(byX[pointIndex].y);
                ++pointIndex;
            }
            int lowerLeft = bit.sum(q.y);
            Point yKey;
            yKey.x = 0;
            yKey.y = q.y;
            int below = lower_bound(byY.begin(), byY.end(), yKey, byYPoint) - byY.begin();
            int signedDifference = n + 4 * lowerLeft - 2 * left - 2 * below;
            answer[q.id] = signedDifference < 0 ? -signedDifference : signedDifference;
        }

        if (tc != 0) putchar('\n');
        for (int i = 0; i < m; ++i) printf("%d\n", answer[i]);
    }
    return 0;
}
