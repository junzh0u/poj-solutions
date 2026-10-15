// POJ 3771 - World Islands
// Model: gpt-6.1-sol
// Try every island as Bill's isolated island and run dense Prim on the rest.
// The remaining n-1 islands must form one connected tree; deleting the
// largest edge of a full MST would incorrectly allow two nontrivial groups.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int n, x[50], y[50];
        double w[50][50];
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%d%d", &x[i], &y[i]);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                int dx = x[i]-x[j], dy = y[i]-y[j];
                w[i][j] = sqrt(double(dx*dx+dy*dy));
            }
        double answer = 1e100;
        for (int omit = 0; omit < n; ++omit) {
            double best[50], sum = 0;
            bool used[50];
            for (int i = 0; i < n; ++i) { best[i] = 1e100; used[i] = false; }
            used[omit] = true;
            if (n > 1) best[omit == 0 ? 1 : 0] = 0;
            for (int step = 0; step < n-1; ++step) {
                int v = -1;
                for (int i = 0; i < n; ++i)
                    if (!used[i] && (v == -1 || best[i] < best[v])) v = i;
                used[v] = true;
                sum += best[v];
                for (int i = 0; i < n; ++i)
                    if (!used[i] && w[v][i] < best[i]) best[i] = w[v][i];
            }
            answer = min(answer, sum);
        }
        printf("%.2f\n", answer);
    }
    return 0;
}
