// POJ 2322 - PLANKS
// Model: gpt-6-astra
// Search stump positions and remaining multiplicities of plank lengths.
// Mixed-radix memoization merges interchangeable planks; retain original IDs
// for the witness. Crossings are permitted, and each move uses a full plank
// between stumps. One grid is followed by plank lists until EOF.
#include <cstdio>
#include <vector>
#include <cstdlib>
using namespace std;
char grid[10][11];
vector<int> ids[10];
int order[9], groups, used[10], weight[10];
int dest[100][10][4];
vector<unsigned char> dead;
int pathId[20], pathPos[20], answer;
bool search(int pos, int state, int total, int depth) {
    if (pos == 99) { answer = depth; return true; }
    if (total < 18 - pos / 10 - pos % 10) return false;
    int key = state * 100 + pos;
    if (dead[key]) return false;
    for (int g = 0; g < groups; ++g) {
        int len = order[g];
        if (used[len] == (int)ids[len].size()) continue;
        int id = ids[len][used[len]++];
        for (int d = 0; d < 4; ++d) {
            int next = dest[pos][len][d];
            if (next < 0) continue;
            pathId[depth] = id; pathPos[depth] = next;
            if (search(next, state + weight[len], total-len, depth+1)) return true;
        }
        --used[len];
    }
    dead[key] = 1;
    return false;
}
int main() {
    for (int r = 0; r < 10; ++r) if (scanf("%10s", grid[r]) != 1) return 0;
    const int dr[4] = {0,1,0,-1}, dc[4] = {1,0,-1,0};
    for (int p = 0; p < 100; ++p) for (int len = 1; len <= 9; ++len)
        for (int d = 0; d < 4; ++d) {
            int r = p/10 + dr[d]*len, c = p%10 + dc[d]*len;
            dest[p][len][d] = r >= 0 && r < 10 && c >= 0 && c < 10 && grid[r][c] == '*' ? r*10+c : -1;
        }
    int n;
    while (scanf("%d", &n) == 1) {
        groups = 0;
        for (int len = 1; len <= 9; ++len) { ids[len].clear(); used[len] = 0; }
        int total = 0;
        for (int i = 1; i <= n; ++i) {
            int len; scanf("%d", &len);
            if (len < 1 || len > 9) continue;
            if (ids[len].empty()) order[groups++] = len;
            ids[len].push_back(i); total += len;
        }
        int states = 1;
        for (int g = 0; g < groups; ++g) {
            int len = order[g]; weight[len] = states;
            states *= (int)ids[len].size()+1;
        }
        dead.assign(states*100, 0);
        if (search(0,0,total,0)) {
            for (int i = 0; i < answer; ++i)
                printf("place plank %d to stump (%d,%d)\n", pathId[i], pathPos[i]/10+1, pathPos[i]%10+1);
        } else puts("no solution possible");
        puts("");
    }
    return 0;
}
