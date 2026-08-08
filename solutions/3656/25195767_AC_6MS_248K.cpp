// POJ 3656 - Rock Skipping
// Model: claude-opus-5
//
// Brute force over every throw (i, d). The map is at most 40 characters, and a
// throw's outcome is unchanged once i + d reaches the end of the map, so the
// whole search space is i in [0, n) and d in [1, n - i] -- at most 820 throws,
// each simulated in O(n).
//
// A throw contacts i, i+d, i+2d, ...  Each water contact ('.') counts as a skip
// and the rock continues; the first non-'.' contact stops it.  "Length" is the
// position of the LAST contact, which is that obstacle when one is hit and the
// last water position when the rock leaves the lake instead.  Note a throw
// landing directly on an obstacle is legal: it has count 0 and length i, which
// is what decides an all-obstacle map (the discuss board's one posted case is
// the single-character map "#", answer "0 1").
//
// Ranking, in order: count highest, length greatest, initial position greatest,
// distance smallest.
//
// The distance tie-break is in fact unreachable except when it is forced.  For
// count >= 1 the pair (count, length) determines d uniquely -- two different d
// would require the position "length" to be both water (rock skipped off the
// end from it) and an obstacle (rock stopped on it).  For count == 0 every d
// ties, but a count-0 throw only wins on an all-obstacle map, where the largest
// length forces i = n-1 and hence d = 1.  It is implemented anyway, as stated.

#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

int main() {
    string line;
    while (getline(cin, line)) {
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);
        if (line == "END") break;
        if (line.empty()) continue;

        int n = (int)line.size();
        int bi = -1, bd = -1, bc = -1, bl = -1;
        for (int i = 0; i < n; ++i) {
            for (int d = 1; i + d <= n; ++d) {
                int count = 0, j = i;
                while (j < n && line[j] == '.') { ++count; j += d; }
                int len = (j < n) ? j : j - d;
                bool better;
                if (bi < 0) better = true;
                else if (count != bc) better = count > bc;
                else if (len != bl) better = len > bl;
                else if (i != bi) better = i > bi;
                else better = d < bd;
                if (better) { bi = i; bd = d; bc = count; bl = len; }
            }
        }
        printf("%d %d\n", bi, bd);
    }
    return 0;
}
