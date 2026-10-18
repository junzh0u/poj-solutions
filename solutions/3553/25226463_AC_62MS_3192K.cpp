// POJ 3553 - Task schedule
// Model: gpt-6-astra
// Build the sequence backwards, choosing the largest deadline among sinks.
// Moving this sink to the end cannot increase the maximum lateness:
// displaced jobs finish earlier, and its deadline is at least the old last
// job's deadline. Repeat on the remaining DAG. Processing times are read
// but do not affect this exchange argument. Output job IDs, not lateness.
// Complexity: O(m + n log n) time and O(n + m) memory.
#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
using namespace std;
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> deadline(n), out(n, 0), answer(n);
    vector< vector<int> > pred(n);
    for (int i = 0; i < n; ++i) {
        int p;
        scanf("%d%d", &p, &deadline[i]);
    }
    int m;
    scanf("%d", &m);
    for (int k = 0; k < m; ++k) {
        int u, v;
        scanf("%d%d", &u, &v);
        --u; --v;
        pred[v].push_back(u);
        ++out[u];
    }
    priority_queue< pair<int,int> > ready;
    for (int i = 0; i < n; ++i)
        if (!out[i]) ready.push(make_pair(deadline[i], i));
    for (int pos = n - 1; pos >= 0; --pos) {
        int v = ready.top().second;
        ready.pop();
        answer[pos] = v + 1;
        for (size_t k = 0; k < pred[v].size(); ++k) {
            int u = pred[v][k];
            if (--out[u] == 0) ready.push(make_pair(deadline[u], u));
        }
    }
    for (int i = 0; i < n; ++i) printf("%d\n", answer[i]);
    return 0;
}
