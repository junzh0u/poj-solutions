// POJ 3156 - Interconnect
// Model: gpt-6-astra
// Memoize expectations by the sorted multiset of component sizes.
// A road within a component consumes a year without changing the state;
// eliminate that self-loop when solving the expectation equation.
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
map<vector<int>, double> memo;
int n, parent[31], sizes[31];
int root(int x) { return parent[x] == x ? x : parent[x] = root(parent[x]); }
double expected(const vector<int>& a) {
    if (a.size() == 1) return 0;
    map<vector<int>, double>::iterator it = memo.find(a);
    if (it != memo.end()) return it->second;
    double numerator = n * (n - 1) / 2.0;
    int cross = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        for (int j = i + 1; j < (int)a.size(); ++j) {
            int weight = a[i] * a[j];
            cross += weight;
            vector<int> b;
            for (int k = 0; k < (int)a.size(); ++k)
                if (k != i && k != j) b.push_back(a[k]);
            b.push_back(a[i] + a[j]);
            sort(b.begin(), b.end());
            numerator += weight * expected(b);
        }
    }
    return memo[a] = numerator / cross;
}
int main() {
    int m;
    if (scanf("%d%d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) parent[i] = i;
    for (int i = 0; i < m; ++i) {
        int u, v; scanf("%d%d", &u, &v);
        parent[root(u)] = root(v);
    }
    for (int i = 1; i <= n; ++i) ++sizes[root(i)];
    vector<int> a;
    for (int i = 1; i <= n; ++i) if (sizes[i]) a.push_back(sizes[i]);
    sort(a.begin(), a.end());
    printf("%.10f\n", expected(a));
    return 0;
}
