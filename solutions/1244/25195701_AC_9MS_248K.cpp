// POJ 1244 - Slots of Fun
// Model: claude-opus-5
//
// The display is a triangular packing of touching unit circles: row a (0-based)
// holds a+1 circles, rows offset by half a diameter.  Cell (a,b) therefore sits
// at cartesian (b - a/2, -a*sqrt(3)/2), which makes the squared distance an
// exact integer form on the triangular lattice:
//
//     d^2 = da^2 + db^2 - da*db
//
// Three cells are the vertices of an equilateral triangle iff their three
// pairwise d^2 agree.  Using this instead of floating-point coordinates removes
// the only pitfall the discuss board reports (every WA post there is an == on
// doubles, or float instead of double); no epsilon is involved at all.
//
// The statement guarantees no letter appears more than 3 times, so per letter
// there is at most one triple; the code still scans all triples so that input
// violating the guarantee degrades gracefully rather than silently.
//
// Not an ambiguity but worth stating: "equilateral" includes triangles tilted
// with respect to the lattice axes, not just the upright/inverted ones - both
// sample cases contain a tilted winner ('c' in case 1, 'c' and 'z' in case 2),
// which is what fixes the reading.  The near-miss 'd' of case 1 (two cells in a
// row plus one below) is isosceles, not equilateral, and is correctly excluded.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static int distSq(int a1, int b1, int a2, int b2) {
    int da = a1 - a2, db = b1 - b2;
    return da * da + db * db - da * db;
}

int main() {
    int n;
    while (cin >> n && n != 0) {
        int total = n * (n + 1) / 2;
        string s;
        while ((int)s.size() < total) {
            string chunk;
            if (!(cin >> chunk)) break;
            s += chunk;
        }

        vector<int> ra[26], ca[26];
        int idx = 0;
        for (int a = 0; a < n; a++) {
            for (int b = 0; b <= a; b++) {
                int c = s[idx++] - 'a';
                if (c >= 0 && c < 26) {
                    ra[c].push_back(a);
                    ca[c].push_back(b);
                }
            }
        }

        string out;
        for (int c = 0; c < 26; c++) {
            int m = (int)ra[c].size();
            bool win = false;
            for (int i = 0; i < m && !win; i++)
                for (int j = i + 1; j < m && !win; j++)
                    for (int k = j + 1; k < m && !win; k++) {
                        int d1 = distSq(ra[c][i], ca[c][i], ra[c][j], ca[c][j]);
                        int d2 = distSq(ra[c][i], ca[c][i], ra[c][k], ca[c][k]);
                        int d3 = distSq(ra[c][j], ca[c][j], ra[c][k], ca[c][k]);
                        if (d1 == d2 && d2 == d3) win = true;
                    }
            if (win) out += (char)('a' + c);
        }

        if (out.empty()) out = "LOOOOOOOOSER!";
        cout << out << "\n";
    }
    return 0;
}
