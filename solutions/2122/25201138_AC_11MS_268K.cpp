// POJ 2122 - Japan Plotter Driver
// Model: claude-sonnet-5
// Approach: direct simulation of a character grid (X columns x Y rows).
// POINT/TEXT/LINE draw characters into cells via a combine() merge rule;
// CLEAR unconditionally blanks its bounding rectangle. LINE direction is
// determined from the two endpoints (horizontal/vertical/diagonal), and the
// diagonal picks '\' when dx and dy have the same sign (both increasing,
// i.e. moving down-right) and '/' otherwise, since y grows downward.
// Overlap rule from the statement, modeled as a pairwise combine(old,new):
//   - empty cell -> new character
//   - same character drawn again -> unchanged
//   - both characters drawn so far belong to {'-','|','+'} -> '+'
//   - both belong to {'/','\\','x'} -> 'x'
//   - otherwise -> '*'
// '+' and 'x' are themselves treated as members of their family so a third
// overlapping stroke of the same family stays consolidated. No statement
// ambiguity found; the sample's overlap cases (diagonal crossing a point,
// vertical crossing a horizontal line, text crossing lines) all match this
// rule directly.
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

static char grid[80][80]; // grid[y][x], 1-indexed
int X, Y;

char combineChar(char c, char n) {
    if (c == ' ') return n;
    if (c == n) return c;
    bool cPM = (c == '-' || c == '|' || c == '+');
    bool nPM = (n == '-' || n == '|' || n == '+');
    if (cPM && nPM) return '+';
    bool cSL = (c == '/' || c == '\\' || c == 'x');
    bool nSL = (n == '/' || n == '\\' || n == 'x');
    if (cSL && nSL) return 'x';
    return '*';
}

void setChar(int x, int y, char n) {
    grid[y][x] = combineChar(grid[y][x], n);
}

int main() {
    while (cin >> X >> Y) {
        if (X == 0 && Y == 0) break;
        for (int y = 1; y <= Y; y++)
            for (int x = 1; x <= X; x++)
                grid[y][x] = ' ';
        string cmd;
        while (cin >> cmd) {
            if (cmd == "PRINT") break;
            if (cmd == "POINT") {
                int x, y;
                cin >> x >> y;
                setChar(x, y, 'o');
            } else if (cmd == "TEXT") {
                int x, y;
                string txt;
                cin >> x >> y >> txt;
                for (size_t i = 0; i < txt.size(); i++)
                    setChar(x + (int)i, y, txt[i]);
            } else if (cmd == "LINE") {
                int x1, y1, x2, y2;
                cin >> x1 >> y1 >> x2 >> y2;
                char ch;
                if (y1 == y2) ch = '-';
                else if (x1 == x2) ch = '|';
                else if ((x2 - x1) * (y2 - y1) > 0) ch = '\\';
                else ch = '/';
                int dx = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
                int dy = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);
                int cx = x1, cy = y1;
                while (true) {
                    setChar(cx, cy, ch);
                    if (cx == x2 && cy == y2) break;
                    cx += dx;
                    cy += dy;
                }
            } else if (cmd == "CLEAR") {
                int x1, y1, x2, y2;
                cin >> x1 >> y1 >> x2 >> y2;
                int xa = min(x1, x2), xb = max(x1, x2);
                int ya = min(y1, y2), yb = max(y1, y2);
                for (int y = ya; y <= yb; y++)
                    for (int x = xa; x <= xb; x++)
                        grid[y][x] = ' ';
            }
        }
        cout << '+';
        for (int x = 1; x <= X; x++) cout << '-';
        cout << '+' << "\n";
        for (int y = 1; y <= Y; y++) {
            cout << '|';
            for (int x = 1; x <= X; x++) cout << grid[y][x];
            cout << '|' << "\n";
        }
        cout << '+';
        for (int x = 1; x <= X; x++) cout << '-';
        cout << '+' << "\n";
        cout << "\n";
    }
    return 0;
}
