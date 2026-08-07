// POJ 3445 - Elementary Additions
// Model: claude-opus-5
//
// Von Neumann ordinals: n is the set of the representations of all smaller
// non-negative integers, so the integer a string denotes is exactly the
// cardinality of its outermost set -- the number of top-level elements. Parsing
// therefore needs no recursion: track brace depth and count every '{' seen while
// the depth is already 1. The nested contents are redundant for a well-formed
// input.
//
// Reading is a depth-driven scan of the whole stream rather than line-by-line, so
// it does not care how the two sets of a case are split across lines, nor about
// blank or trailing lines.
//
// Output: rep(0) = "{}", rep(n) = "{" + rep(0) + "," + ... + "," + rep(n-1) + "}",
// built iteratively once for 0..15. |rep(n)| = 5*2^(n-1)-1 for n>=1, so rep(15)
// is 81919 bytes -- std::string throughout, never a fixed char array (undersized
// buffers are the only pitfall the discuss board records for this problem).
//
// The sum is guaranteed to be at most 15, so no big-integer arithmetic is needed.

#include <cstdio>
#include <string>
#include <vector>

using namespace std;

int main() {
    const int MAXN = 15;
    vector<string> rep(MAXN + 1);
    rep[0] = "{}";
    for (int n = 1; n <= MAXN; ++n) {
        string s = "{";
        for (int i = 0; i < n; ++i) {
            if (i) s += ',';
            s += rep[i];
        }
        s += '}';
        rep[n] = s;
    }

    int t;
    if (scanf("%d", &t) != 1) return 0;

    const size_t BUF = 1 << 16;
    vector<char> buf(BUF);
    size_t have = 0, pos = 0;

    int depth = 0;      // current brace nesting depth
    int count = 0;      // top-level elements of the set being scanned
    int pending = -1;   // cardinality of the first set of the current case
    int done = 0;       // cases emitted

    while (done < t) {
        if (pos == have) {
            have = fread(&buf[0], 1, BUF, stdin);
            pos = 0;
            if (have == 0) break;  // input exhausted
        }
        char c = buf[pos++];
        if (c == '{') {
            if (depth == 1) ++count;
            ++depth;
        } else if (c == '}') {
            --depth;
            if (depth == 0) {
                if (pending < 0) {
                    pending = count;
                } else {
                    int sum = pending + count;
                    pending = -1;
                    if (sum < 0) sum = 0;
                    if (sum > MAXN) sum = MAXN;
                    fwrite(rep[sum].data(), 1, rep[sum].size(), stdout);
                    fputc('\n', stdout);
                    ++done;
                }
                count = 0;
            }
        }
    }
    return 0;
}
