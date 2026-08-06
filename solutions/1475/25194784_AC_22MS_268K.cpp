// POJ 1475 - Pushing Boxes
// Model: gpt-5.6-terra
// Dijkstra searches states after a push: the box cell and the player's side of
// the box.  Its lexicographic cost is (pushes, all moves).  For each state a
// small BFS finds the shortest walk to each side while treating the box as a
// wall, then each reachable side yields one push transition.  This directly
// enforces the stated priority of pushes before total moves.
#include <cstdio>
#include <cstring>
#include <queue>
#include <string>
#include <vector>

using namespace std;

const int MAXN = 400;
const int INF = 1000000000;
int rows, cols, cells;
char board[21][21];
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};
char lowdir[4] = {'n', 's', 'w', 'e'};
char updir[4] = {'N', 'S', 'W', 'E'};

struct Node {
    int pushes, moves, state;
    Node(int p, int m, int s) : pushes(p), moves(m), state(s) {}
};

struct NodeOrder {
    bool operator()(const Node &a, const Node &b) const {
        if (a.pushes != b.pushes) return a.pushes > b.pushes;
        return a.moves > b.moves;
    }
};

bool freeCell(int p) {
    return p >= 0 && p < cells && board[p / cols][p % cols] != '#';
}

string walkPath(int start, int goal, int blocked) {
    int dist[MAXN], prev[MAXN];
    char step[MAXN];
    queue<int> q;
    int i;
    for (i = 0; i < cells; ++i) dist[i] = -1;
    if (start == blocked || goal == blocked || !freeCell(start) || !freeCell(goal)) return "";
    dist[start] = 0;
    prev[start] = -1;
    q.push(start);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        int r = u / cols, c = u % cols;
        int d;
        if (u == goal) break;
        for (d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            int v;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            v = nr * cols + nc;
            if (v == blocked || !freeCell(v) || dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            prev[v] = u;
            step[v] = lowdir[d];
            q.push(v);
        }
    }
    if (dist[goal] == -1) return "";
    string result;
    for (i = goal; i != start; i = prev[i]) result += step[i];
    for (i = 0; i < (int)result.size() / 2; ++i) {
        char t = result[i];
        result[i] = result[result.size() - 1 - i];
        result[result.size() - 1 - i] = t;
    }
    return result;
}

int main() {
    int maze = 0;
    while (scanf("%d%d", &rows, &cols) == 2 && (rows || cols)) {
        int start = -1, box = -1, target = -1;
        int r, c, i;
        ++maze;
        for (r = 0; r < rows; ++r) {
            scanf("%s", board[r]);
            for (c = 0; c < cols; ++c) {
                int p = r * cols + c;
                if (board[r][c] == 'S') start = p;
                if (board[r][c] == 'B') box = p;
                if (board[r][c] == 'T') target = p;
            }
        }
        cells = rows * cols;
        int bestPush[MAXN * 4], bestMove[MAXN * 4], parent[MAXN * 4];
        string part[MAXN * 4];
        for (i = 0; i < cells * 4; ++i) {
            bestPush[i] = INF;
            bestMove[i] = INF;
            parent[i] = -2;
        }
        priority_queue<Node, vector<Node>, NodeOrder> pq;
        for (i = 0; i < 4; ++i) {
            int br = box / cols, bc = box % cols;
            int needr = br - dr[i], needc = bc - dc[i];
            int nr = br + dr[i], nc = bc + dc[i];
            if (needr < 0 || needr >= rows || needc < 0 || needc >= cols ||
                nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            int need = needr * cols + needc, nextBox = nr * cols + nc;
            if (!freeCell(need) || !freeCell(nextBox)) continue;
            string w = walkPath(start, need, box);
            if (start != need && w.empty()) continue;
            int state = nextBox * 4 + (i ^ 1);
            int moves = (int)w.size() + 1;
            if (1 < bestPush[state] || (1 == bestPush[state] && moves < bestMove[state])) {
                bestPush[state] = 1;
                bestMove[state] = moves;
                parent[state] = -1;
                part[state] = w + updir[i];
                pq.push(Node(1, moves, state));
            }
        }
        int answer = -1;
        while (!pq.empty()) {
            Node cur = pq.top(); pq.pop();
            int state = cur.state, b = state / 4, side = state % 4;
            if (cur.pushes != bestPush[state] || cur.moves != bestMove[state]) continue;
            if (b == target) { answer = state; break; }
            int manr = b / cols + dr[side], manc = b % cols + dc[side];
            if (manr < 0 || manr >= rows || manc < 0 || manc >= cols) continue;
            int man = manr * cols + manc;
            for (i = 0; i < 4; ++i) {
                int br = b / cols, bc = b % cols;
                int needr = br - dr[i], needc = bc - dc[i];
                int nbr = br + dr[i], nbc = bc + dc[i];
                if (needr < 0 || needr >= rows || needc < 0 || needc >= cols ||
                    nbr < 0 || nbr >= rows || nbc < 0 || nbc >= cols) continue;
                int need = needr * cols + needc, nextBox = nbr * cols + nbc;
                if (!freeCell(need) || !freeCell(nextBox)) continue;
                string w = walkPath(man, need, b);
                if (man != need && w.empty()) continue;
                int nextState = nextBox * 4 + (i ^ 1);
                int nextPush = cur.pushes + 1;
                int nextMove = cur.moves + (int)w.size() + 1;
                if (nextPush < bestPush[nextState] ||
                    (nextPush == bestPush[nextState] && nextMove < bestMove[nextState])) {
                    bestPush[nextState] = nextPush;
                    bestMove[nextState] = nextMove;
                    parent[nextState] = state;
                    part[nextState] = w + updir[i];
                    pq.push(Node(nextPush, nextMove, nextState));
                }
            }
        }
        printf("Maze #%d\n", maze);
        if (answer == -1) {
            printf("Impossible.\n\n");
        } else {
            vector<string> pieces;
            for (i = answer; i != -1; i = parent[i]) pieces.push_back(part[i]);
            for (i = (int)pieces.size() - 1; i >= 0; --i) printf("%s", pieces[i].c_str());
            printf("\n\n");
        }
    }
    return 0;
}
