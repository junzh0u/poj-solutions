// POJ 2317 - SHAKE
// Model: claude-sonnet-5
// Simulate the Shake/Rattle/Roll cipher on an NxN grid.
// Shake: odd columns (1-indexed) shift up (wrap top->bottom), even columns shift down.
// Rattle: odd rows shift right (wrap right->left end), even rows shift left.
// Roll: for each concentric ring (loop k, topmost row = k, 1-indexed), walk the ring
// clockwise starting at its top-left corner; odd loops rotate one step clockwise
// (new[i] = old[i-1]), even loops rotate one step counterclockwise (new[i] = old[i+1]).
// Directions/wrap conventions for Shake/Rattle and the Roll rotation sense (which the
// statement leaves to figures) were reverse-engineered by hand-tracing both sample cases.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

static char grid[105][105];
int n;

void shake() {
    for (int c = 0; c < n; ++c) {
        int colnum = c + 1;
        char tmp[105];
        for (int r = 0; r < n; ++r) tmp[r] = grid[r][c];
        if (colnum % 2 == 1) {
            // shift up: new[r] = old[(r+1)%n]
            for (int r = 0; r < n; ++r) grid[r][c] = tmp[(r + 1) % n];
        } else {
            // shift down: new[r] = old[(r-1+n)%n]
            for (int r = 0; r < n; ++r) grid[r][c] = tmp[(r - 1 + n) % n];
        }
    }
}

void rattle() {
    for (int r = 0; r < n; ++r) {
        int rownum = r + 1;
        char tmp[105];
        for (int c = 0; c < n; ++c) tmp[c] = grid[r][c];
        if (rownum % 2 == 1) {
            // shift right: new[c] = old[(c-1+n)%n]
            for (int c = 0; c < n; ++c) grid[r][c] = tmp[(c - 1 + n) % n];
        } else {
            // shift left: new[c] = old[(c+1)%n]
            for (int c = 0; c < n; ++c) grid[r][c] = tmp[(c + 1) % n];
        }
    }
}

void roll() {
    int maxk = n / 2;
    for (int k = 1; k <= maxk; ++k) {
        int top = k - 1, left = k - 1, bottom = n - k, right = n - k;
        int size = bottom - top + 1; // ring side length
        if (size < 2) continue;
        int L = 4 * (size - 1);
        static int rr[400], cc[400];
        int idx = 0;
        for (int c = left; c <= right; ++c) { rr[idx] = top; cc[idx] = c; ++idx; }
        for (int r = top + 1; r <= bottom; ++r) { rr[idx] = r; cc[idx] = right; ++idx; }
        for (int c = right - 1; c >= left; --c) { rr[idx] = bottom; cc[idx] = c; ++idx; }
        for (int r = bottom - 1; r >= top + 1; --r) { rr[idx] = r; cc[idx] = left; ++idx; }
        // idx should equal L
        static char oldv[400];
        for (int i = 0; i < L; ++i) oldv[i] = grid[rr[i]][cc[i]];
        static char newv[400];
        if (k % 2 == 1) {
            for (int i = 0; i < L; ++i) newv[i] = oldv[(i - 1 + L) % L];
        } else {
            for (int i = 0; i < L; ++i) newv[i] = oldv[(i + 1) % L];
        }
        for (int i = 0; i < L; ++i) grid[rr[i]][cc[i]] = newv[i];
    }
}

int main() {
    string keyLine, msgLine;
    while (true) {
        if (!getline(cin, keyLine)) break;
        // strip trailing \r
        while (!keyLine.empty() && (keyLine[keyLine.size()-1] == '\r' || keyLine[keyLine.size()-1] == '\n'))
            keyLine.erase(keyLine.size()-1);
        if (keyLine.empty()) continue; // skip stray blank lines
        if (!getline(cin, msgLine)) break;
        while (!msgLine.empty() && (msgLine[msgLine.size()-1] == '\r' || msgLine[msgLine.size()-1] == '\n'))
            msgLine.erase(msgLine.size()-1);

        int d1 = keyLine[0] - '0';
        int d2 = keyLine[1] - '0';
        n = d1 * 10 + d2;
        if (n == 0) n = 100;
        string ops = keyLine.substr(2);

        int total = n * n;
        int mi = 0;
        int filler = 0; // index into A..Z repeating
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (mi < (int)msgLine.size()) {
                    char ch = msgLine[mi++];
                    grid[r][c] = toupper((unsigned char)ch);
                } else {
                    grid[r][c] = 'A' + (filler % 26);
                    filler++;
                }
            }
        }

        for (size_t i = 0; i < ops.size(); ++i) {
            char op = ops[i];
            if (op == 'S') shake();
            else if (op == 'R') rattle();
            else if (op == 'L') roll();
        }

        string out;
        out.reserve(total);
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < n; ++c)
                out += grid[r][c];
        printf("%s\n", out.c_str());
    }
    return 0;
}
