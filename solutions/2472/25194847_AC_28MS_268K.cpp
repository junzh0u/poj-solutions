// POJ 2472 - 106 miles to Chicago
// Model: gpt-5.6-terra
// Dijkstra's selection rule still applies when path values are multiplied:
// repeatedly finalize the unvisited intersection with greatest survival probability.
// Edge probabilities are read as doubles before division, and the final value is converted to percent.
#include <cstdio>

int main() {
    int n;
    while (std::scanf("%d", &n) == 1 && n != 0) {
        int m;
        std::scanf("%d", &m);
        double probability[101][101] = {{0.0}};
        int i;
        for (i = 1; i <= n; ++i) probability[i][i] = 1.0;
        for (i = 0; i < m; ++i) {
            int a, b, percent;
            std::scanf("%d%d%d", &a, &b, &percent);
            probability[a][b] = probability[b][a] = percent / 100.0;
        }

        double best[101] = {0.0};
        bool used[101] = {false};
        best[1] = 1.0;
        for (i = 1; i <= n; ++i) {
            int current = -1;
            int node;
            for (node = 1; node <= n; ++node) {
                if (!used[node] && (current == -1 || best[node] > best[current])) current = node;
            }
            if (current == -1 || best[current] == 0.0) break;
            used[current] = true;
            for (node = 1; node <= n; ++node) {
                double candidate = best[current] * probability[current][node];
                if (!used[node] && candidate > best[node]) best[node] = candidate;
            }
        }
        std::printf("%.6f percent\n", best[n] * 100.0);
    }
    return 0;
}
