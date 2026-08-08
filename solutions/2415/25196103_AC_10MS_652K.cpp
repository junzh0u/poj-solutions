// POJ 2415 - Hike on a Graph
// Model: claude-fable-5
//
// BFS over the state space (a,b,c) of the three piece positions, n <= 50 so
// 125000 states. From (a,b,c) one piece may move to any location j where the
// arrow colour to j equals the colour of the arrow between the other two
// pieces (the diagonal loop colour when they share a location). Answer is the
// distance to any state (x,x,x), or "impossible" if unreachable.
//
// Board pitfalls honoured: matrix tokens may be separated by multiple spaces
// (cin >> char skips all whitespace), and all per-case state is reset.
#include <iostream>
#include <cstring>
using namespace std;

int n;
char col[51][51];
short dist[51][51][51];
int qa[130000], qb[130000], qc[130000];

int main() {
    while (cin >> n && n != 0) {
        int p1, p2, p3;
        cin >> p1 >> p2 >> p3;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                cin >> col[i][j];
        memset(dist, -1, sizeof(dist));
        int head = 0, tail = 0;
        dist[p1][p2][p3] = 0;
        qa[tail] = p1; qb[tail] = p2; qc[tail] = p3; tail++;
        int ans = -1;
        if (p1 == p2 && p2 == p3) ans = 0;
        while (head < tail && ans < 0) {
            int a = qa[head], b = qb[head], c = qc[head];
            head++;
            int d = dist[a][b][c] + 1;
            char cbc = col[b][c], cac = col[a][c], cab = col[a][b];
            for (int j = 1; j <= n; j++) {
                if (col[a][j] == cbc && dist[j][b][c] < 0) {
                    dist[j][b][c] = (short)d;
                    if (j == b && b == c) { ans = d; break; }
                    qa[tail] = j; qb[tail] = b; qc[tail] = c; tail++;
                }
                if (col[b][j] == cac && dist[a][j][c] < 0) {
                    dist[a][j][c] = (short)d;
                    if (a == j && j == c) { ans = d; break; }
                    qa[tail] = a; qb[tail] = j; qc[tail] = c; tail++;
                }
                if (col[c][j] == cab && dist[a][b][j] < 0) {
                    dist[a][b][j] = (short)d;
                    if (a == b && b == j) { ans = d; break; }
                    qa[tail] = a; qb[tail] = b; qc[tail] = j; tail++;
                }
            }
        }
        if (ans < 0) cout << "impossible" << endl;
        else cout << ans << endl;
    }
    return 0;
}
