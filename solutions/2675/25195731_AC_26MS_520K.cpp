// POJ 2675 - Songs
// Model: claude-opus-5
//
// Minimize sum_{i=1..n} f_{s(i)} * sum_{j=1..i} l_{s(j)}, then report the
// identifier sitting at a queried 1-based position of an optimal order.
//
// Adjacent-exchange argument: with a immediately before b and P the length
// already laid down, the two orders cost f_a*(P+l_a) + f_b*(P+l_a+l_b) and
// f_b*(P+l_b) + f_a*(P+l_b+l_a); the difference is f_b*l_a - f_a*l_b, so a
// belongs first exactly when l_a/f_a < l_b/f_b.  Sort ascending by l/f,
// compared as the cross product l_a*f_b < l_b*f_a to avoid a division.
// Zero-frequency songs have infinite ratio and are forced last; they are
// split out first so the comparator stays a strict weak ordering even for a
// degenerate (l=0, f=0) song, whose cross products tie with everything.
// Ties in l/f leave several optimal orders, so the position's identifier is
// only well defined up to them; input order breaks ties.
//
// Input framing (the statement is vague, the discuss board is not): the file
// holds an unknown number of data sets and must be read to EOF, and white
// space is free-form, so everything is read as whitespace-separated tokens.
// Identifiers are read and echoed as raw tokens rather than parsed as ints.
//
// Ambiguity note: the official sample cannot tell ascending from descending
// order - it queries position 3 of 5, the fixed midpoint under reversal, so
// the reversed comparator reproduces the sample byte for byte.  Direction was
// settled by the exchange argument above and by an exhaustive permutation
// reference in exact rational arithmetic.

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

struct Song {
    string id;
    double len;
    double freq;
    int idx;
};

struct Cmp {
    bool operator()(const Song &a, const Song &b) const {
        bool az = (a.freq <= 0.0), bz = (b.freq <= 0.0);
        if (az != bz) return bz;
        if (az && bz) return a.idx < b.idx;
        double lhs = a.len * b.freq;
        double rhs = b.len * a.freq;
        if (lhs != rhs) return lhs < rhs;
        return a.idx < b.idx;
    }
};

int main() {
    long n;
    char buf[256];
    while (scanf("%ld", &n) == 1) {
        vector<Song> v;
        if (n > 0) v.reserve((size_t)n);
        long i;
        for (i = 0; i < n; i++) {
            Song s;
            if (scanf("%255s", buf) != 1) return 0;
            s.id = buf;
            if (scanf("%lf %lf", &s.len, &s.freq) != 2) return 0;
            s.idx = (int)i;
            v.push_back(s);
        }
        long pos;
        if (scanf("%ld", &pos) != 1) return 0;
        sort(v.begin(), v.end(), Cmp());
        if (pos >= 1 && pos <= (long)v.size())
            printf("%s\n", v[pos - 1].id.c_str());
    }
    return 0;
}
