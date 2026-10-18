// POJ 3609 - Reset Sequence
// Model: gpt-6-astra
// BFS over sets of possible states finds a shortest command sequence.
// The goal is precisely {0}, not an arbitrary singleton. Commands use
// lowercase hexadecimal digits; the input contains exactly one case.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int n, m, go[8][16];
    if (!(cin >> n >> m)) return 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) cin >> go[i][j];
    int parent[256], command[256], queue[256];
    fill(parent, parent + 256, -1);
    int start = (1 << n) - 1, head = 0, tail = 0;
    parent[start] = start;
    queue[tail++] = start;
    while (head < tail && parent[1] == -1) {
        int mask = queue[head++];
        for (int j = 0; j < m; ++j) {
            int next = 0;
            for (int i = 0; i < n; ++i)
                if (mask & (1 << i)) next |= 1 << go[i][j];
            if (parent[next] == -1) {
                parent[next] = mask;
                command[next] = j;
                queue[tail++] = next;
            }
        }
    }
    if (parent[1] == -1) cout << "*\n";
    else {
        string answer;
        for (int mask = 1; mask != start; mask = parent[mask])
            answer += "0123456789abcdef"[command[mask]];
        reverse(answer.begin(), answer.end());
        cout << answer << '\n';
    }
    return 0;
}
