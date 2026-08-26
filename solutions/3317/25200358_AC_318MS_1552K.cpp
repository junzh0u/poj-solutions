// POJ 3317 - Stake Your Claim
// Model: gpt-5.6-terra
// Exhaustively minimax the at most ten unfilled squares.  A state is the
// sequence of choices on those squares; memoization is safe without
// alpha-beta bounds because each state has one exact zero-sum value.
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int n, emptyCount;
vector<string> board;
vector<int> er, ec;
map<string, int> memo;
int dr[4] = {-1, 1, 0, 0};
int dc[4] = {0, 0, -1, 1};

int terminalValue() {
    bool used[8][8] = {};
    int best[2] = {0, 0};
    int qr[64], qc[64];
    for (int r = 0; r < n; ++r) for (int c = 0; c < n; ++c) {
        if (used[r][c]) continue;
        char who = board[r][c];
        int head = 0, tail = 0;
        qr[tail] = r; qc[tail++] = c; used[r][c] = true;
        while (head < tail) {
            int x = qr[head], y = qc[head++];
            for (int d = 0; d < 4; ++d) {
                int nx = x + dr[d], ny = y + dc[d];
                if (nx >= 0 && nx < n && ny >= 0 && ny < n && !used[nx][ny] && board[nx][ny] == who) {
                    used[nx][ny] = true;
                    qr[tail] = nx; qc[tail++] = ny;
                }
            }
        }
        int v = tail;
        if (v > best[who - '0']) best[who - '0'] = v;
    }
    return best[0] - best[1];
}

int solve(string &state, int filled, int zeros, int ones) {
    if (filled == emptyCount) return terminalValue();
    map<string, int>::iterator it = memo.find(state);
    if (it != memo.end()) return it->second;
    int player = (zeros == ones ? 0 : 1);
    int ans = player == 0 ? -1000 : 1000;
    for (int i = 0; i < emptyCount; ++i) if (state[i] == '.') {
        state[i] = char('0' + player);
        board[er[i]][ec[i]] = state[i];
        int got = solve(state, filled + 1, zeros + (player == 0), ones + (player == 1));
        if (player == 0) { if (got > ans) ans = got; }
        else { if (got < ans) ans = got; }
        board[er[i]][ec[i]] = '.';
        state[i] = '.';
    }
    memo[state] = ans;
    return ans;
}

int main() {
    while (cin >> n && n) {
        board.assign(n, string());
        int zeros = 0, ones = 0;
        er.clear(); ec.clear();
        for (int r = 0; r < n; ++r) {
            cin >> board[r];
            for (int c = 0; c < n; ++c) {
                if (board[r][c] == '.') { er.push_back(r); ec.push_back(c); }
                else if (board[r][c] == '0') ++zeros;
                else ++ones;
            }
        }
        emptyCount = (int)er.size();
        int player = (zeros == ones ? 0 : 1);
        string state(emptyCount, '.');
        int best = player == 0 ? -1000 : 1000, move = -1;
        memo.clear();
        for (int i = 0; i < emptyCount; ++i) {
            state[i] = char('0' + player);
            board[er[i]][ec[i]] = state[i];
            int got = solve(state, 1, zeros + (player == 0), ones + (player == 1));
            if ((player == 0 && got > best) || (player == 1 && got < best)) {
                best = got;
                move = i;
            }
            board[er[i]][ec[i]] = '.';
            state[i] = '.';
        }
        int score = player == 0 ? best : -best;
        cout << '(' << er[move] << ',' << ec[move] << ") " << score << '\n';
    }
    return 0;
}
