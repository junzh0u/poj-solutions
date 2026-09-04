// POJ 3201 - Little Quilt
// Model: claude-sonnet-5
// Approach: recursive-descent parse of the QUILT grammar over the whole
// input with whitespace stripped and expressions split on ';'. Each quilt
// is a rectangular grid of chars plus an error flag. turn(x) rotates the
// grid 90 degrees clockwise: besides moving cells, each individual glyph
// is itself rotated ('/'<->'\\', '-'<->'|', '+' fixed) since the samples
// show turn(A) swaps every '/' to '\\' rather than merely repositioning
// them (verified against all three sample quilts by hand). sew(x,y)
// concatenates columns and requires equal row counts, else error; error
// propagates through further turn()/sew() calls. Sizes are kept as
// std::vector<std::string> (never fixed char buffers) because nested
// sew() calls can double width repeatedly and the discuss board reports
// several RE reports traced to undersized fixed arrays.
#include <cstdio>
#include <string>
#include <vector>
using namespace std;

struct Quilt {
    bool err;
    vector<string> g; // rows of equal length
    Quilt() : err(false) {}
};

static char mapChar(char c) {
    switch (c) {
        case '/': return '\\';
        case '\\': return '/';
        case '-': return '|';
        case '|': return '-';
        default: return c; // '+'
    }
}

Quilt makePrimitive(char which) {
    Quilt q;
    if (which == 'A') {
        q.g.push_back("//");
        q.g.push_back("/+");
    } else {
        q.g.push_back("--");
        q.g.push_back("--");
    }
    return q;
}

Quilt turnQuilt(const Quilt &x) {
    Quilt r;
    if (x.err) { r.err = true; return r; }
    int R = (int)x.g.size();
    int C = R > 0 ? (int)x.g[0].size() : 0;
    r.g.assign(C, string(R, ' '));
    for (int i = 0; i < C; ++i) {
        for (int j = 0; j < R; ++j) {
            r.g[i][j] = mapChar(x.g[R - 1 - j][i]);
        }
    }
    return r;
}

Quilt sewQuilt(const Quilt &a, const Quilt &b) {
    Quilt r;
    if (a.err || b.err) { r.err = true; return r; }
    if (a.g.size() != b.g.size()) { r.err = true; return r; }
    int rows = (int)a.g.size();
    r.g.resize(rows);
    for (int i = 0; i < rows; ++i) {
        r.g[i] = a.g[i] + b.g[i];
    }
    return r;
}

// Recursive descent parser over a whitespace-stripped expression string.
Quilt parseQuilt(const string &s, size_t &pos) {
    if (pos < s.size() && (s[pos] == 'A' || s[pos] == 'B')) {
        char c = s[pos++];
        return makePrimitive(c);
    }
    if (s.compare(pos, 4, "turn") == 0) {
        pos += 4;
        // expect '('
        pos++; // '('
        Quilt x = parseQuilt(s, pos);
        pos++; // ')'
        return turnQuilt(x);
    }
    if (s.compare(pos, 3, "sew") == 0) {
        pos += 3;
        pos++; // '('
        Quilt a = parseQuilt(s, pos);
        pos++; // ','
        Quilt b = parseQuilt(s, pos);
        pos++; // ')'
        return sewQuilt(a, b);
    }
    // Malformed input shouldn't occur per problem statement; return an
    // error quilt defensively rather than reading out of bounds.
    Quilt r;
    r.err = true;
    return r;
}

int main() {
    // Read entire input, stripping whitespace (space, tab, CR, LF).
    string all;
    {
        string line;
        int c;
        while ((c = getchar()) != EOF) {
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
            all.push_back((char)c);
        }
    }

    int quiltNo = 0;
    size_t start = 0;
    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i] == ';') {
            string expr = all.substr(start, i - start);
            start = i + 1;
            if (expr.empty()) continue;
            ++quiltNo;
            size_t pos = 0;
            Quilt q = parseQuilt(expr, pos);
            printf("Quilt %d:\n", quiltNo);
            if (q.err) {
                printf("error\n");
            } else {
                for (size_t r = 0; r < q.g.size(); ++r) {
                    printf("%s\n", q.g[r].c_str());
                }
            }
        }
    }
    return 0;
}
