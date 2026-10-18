// POJ 2477 - Help the problem setter
// Model: gpt-6-astra
// Assign each node one plus the total frequency of its descendants.
// Rotating this node toward the root strictly improves any competing tree:
// its saved frequency exceeds the total possible increase for other nodes.
// Induction then gives the prescribed tree as the unique optimum.
// A subtree of k nodes has total frequency at most 2^k - 1, so n <= 50
// keeps both individual frequencies and their sum within signed 64 bits.
#include <iostream>
using namespace std;
int child[51][2];
long long frequency[51];
long long assignFrequency(int u) {
    if (u == -1) return 0;
    long long descendants = assignFrequency(child[u][0])
                          + assignFrequency(child[u][1]);
    frequency[u] = descendants + 1;
    return descendants + frequency[u];
}
int main() {
    int n;
    while (cin >> n && n) {
        bool hasParent[51] = {false};
        for (int i = 1; i <= n; ++i) {
            cin >> child[i][0] >> child[i][1];
            for (int j = 0; j < 2; ++j)
                if (child[i][j] != -1) hasParent[child[i][j]] = true;
        }
        int root = 1;
        while (hasParent[root]) ++root;
        assignFrequency(root);
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << frequency[i];
        }
        cout << '\n';
    }
    return 0;
}
