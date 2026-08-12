// POJ 1291 - This Sentence is False
// Model: gpt-5.6-terra
// Each sentence gives an XOR constraint between its truth value and the
// referenced sentence.  Bipartite-colour every signed connected component;
// either colour may be the true value, so take the larger colour class.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int to, different;
    Edge(int t, int d) : to(t), different(d) {}
};

int main() {
    int n;
    char line[64];
    while (scanf("%d", &n) == 1 && n) {
        fgets(line, sizeof(line), stdin);
        vector< vector<Edge> > graph(n);
        for (int i = 0; i < n; ++i) {
            fgets(line, sizeof(line), stdin);
            int x;
            char word[8];
            sscanf(line, "Sentence %d is %7s", &x, word);
            --x;
            int different = (word[0] == 'f');
            graph[i].push_back(Edge(x, different));
            graph[x].push_back(Edge(i, different));
        }

        vector<int> colour(n, -1);
        bool consistent = true;
        int answer = 0;
        for (int start = 0; start < n; ++start) {
            if (colour[start] != -1) continue;
            int count[2] = {0, 0};
            vector<int> stack(1, start);
            colour[start] = 0;
            while (!stack.empty()) {
                int u = stack.back();
                stack.pop_back();
                ++count[colour[u]];
                for (size_t j = 0; j < graph[u].size(); ++j) {
                    int v = graph[u][j].to;
                    int need = colour[u] ^ graph[u][j].different;
                    if (colour[v] == -1) {
                        colour[v] = need;
                        stack.push_back(v);
                    } else if (colour[v] != need) {
                        consistent = false;
                    }
                }
            }
            answer += max(count[0], count[1]);
        }
        if (consistent) printf("%d\n", answer);
        else printf("Inconsistent\n");
    }
    return 0;
}
