// POJ 3893 - Hard-working Student
// Model: claude-sonnet-5
//
// The base graph is fixed and fully determined by arithmetic on the vertex
// key: vertices are grouped into rows of two, row = x/2, col = x%2. Forward
// edges cross columns into the next row (F(x) = 2*row+3-col), backward edges
// stay in the same column but go to the previous row (B(x) = 2*row-2+col).
// Both formulas were derived from, and match, every worked example in the
// statement (F(0)=3, F(1)=2, B(3)=1, B(4)=2, and the fully walked "4 <kff 0"
// example). Because the target key is a pure function of the current key,
// "follow the edge if it exists or create it" needs no existence tracking
// at all -- the key is the same either way, so f/b reduce to arithmetic.
//
// v[] is a sparse map from vertex key to assigned node (only 0..3 are
// pre-assigned); commands read a leading index0, a string of actions
// processed right-to-left, and a trailing index1 whose v[] entry seeds the
// running "argument". Each command's string can be very long (the problem's
// own discuss board reports needing a buffer far bigger than 100 chars, up
// to ~1,000,000), so it is read into a std::string rather than a fixed
// buffer, and std::map<int,int> is used for v[] since indices are not
// bounded by the stated N<=10000 in practice. Keys stay well inside 32-bit
// range for any input the time limit permits, so plain int (and %d) is used
// throughout instead of long long/%lld, keeping this C++98-friendly for
// POJ's old MSVC-based "C++" compiler (language 4), which lacks both.
#include <cstdio>
#include <string>
#include <map>
#include <iostream>

using namespace std;

static inline int doF(int x) {
    int row = x / 2, col = x % 2;
    return 2 * row + 3 - col;
}
static inline int doB(int x) {
    int row = x / 2, col = x % 2;
    return 2 * row - 2 + col;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    map<int, int> v;
    v[0] = 0; v[1] = 1; v[2] = 2; v[3] = 3;

    int idx0, idx1;
    string s;
    string out;
    out.reserve(1 << 20);
    char buf[32];

    while (cin >> idx0) {
        if (!(cin >> s)) break;
        if (!(cin >> idx1)) break;
        int arg = v[idx1];
        for (int i = (int)s.size() - 1; i >= 0; --i) {
            char c = s[i];
            if (c == 'f') {
                arg = doF(arg);
            } else if (c == 'b') {
                arg = doB(arg);
            } else if (c == 'k') {
                int len = sprintf(buf, "%d\n", arg);
                out.append(buf, len);
            } else if (c == '<') {
                v[idx0] = arg;
            } else if (c == '=') {
                out += (v[idx0] == arg) ? "=\n" : "#\n";
            }
        }
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
