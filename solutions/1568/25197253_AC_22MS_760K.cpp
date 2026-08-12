// POJ 1568 - Find the Winning Move
// Model: gpt-5.6-terra
// Minimax searches every continuation after each candidate x move.  The board
// contains at least four stones, so ternary memo states over the empty cells
// fit in 3^12 entries and make exhaustive search practical.
#include <cstdio>
#include <cstring>

int baseX, baseO, pos[16], n, pw[17];
unsigned char memo[531441];
int lines[10] = {
    0x000f, 0x00f0, 0x0f00, 0xf000,
    0x1111, 0x2222, 0x4444, 0x8888,
    0x8421, 0x1248
};

bool won(int mask) {
    int i;
    for (i = 0; i < 10; ++i)
        if ((mask & lines[i]) == lines[i]) return true;
    return false;
}

bool solve(int state, bool xturn) {
    int i, t = state, xm = baseX, om = baseO, d;
    unsigned char &saved = memo[state];
    if (saved) return saved == 2;
    for (i = 0; i < n; ++i) {
        d = t % 3;
        t /= 3;
        if (d == 1) xm |= 1 << pos[i];
        else if (d == 2) om |= 1 << pos[i];
    }
    if (won(xm)) { saved = 2; return true; }
    if (won(om) || state == pw[n] - 1) { saved = 1; return false; }
    if (xturn) {
        for (i = 0; i < n; ++i)
            if ((state / pw[i]) % 3 == 0 && solve(state + pw[i], false)) {
                saved = 2;
                return true;
            }
        saved = 1;
        return false;
    }
    for (i = 0; i < n; ++i)
        if ((state / pw[i]) % 3 == 0 && !solve(state + 2 * pw[i], true)) {
            saved = 1;
            return false;
        }
    saved = 2;
    return true;
}

int main() {
    char row[32];
    int r, c, answer;
    pw[0] = 1;
    for (r = 1; r <= 16; ++r) pw[r] = pw[r - 1] * 3;
    while (std::fgets(row, sizeof(row), stdin) && row[0] != '$') {
        baseX = baseO = 0;
        for (r = 0; r < 4; ++r) {
            std::fgets(row, sizeof(row), stdin);
            for (c = 0; c < 4; ++c) {
                if (row[c] == 'x') baseX |= 1 << (r * 4 + c);
                else if (row[c] == 'o') baseO |= 1 << (r * 4 + c);
            }
        }
        answer = -1;
        n = 0;
        for (r = 0; r < 16; ++r)
            if (!(baseX & (1 << r)) && !(baseO & (1 << r))) pos[n++] = r;
        for (r = 0; r < 16; ++r) {
            int index = -1, i;
            if ((baseX & (1 << r)) || (baseO & (1 << r))) continue;
            for (i = 0; i < n; ++i) if (pos[i] == r) index = i;
            std::memset(memo, 0, pw[n]);
            if (solve(pw[index], false)) { answer = r; break; }
        }
        if (answer < 0) std::puts("#####");
        else std::printf("(%d,%d)\n", answer / 4, answer % 4);
    }
    return 0;
}
