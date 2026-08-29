// POJ 3959 - Alignment of Code
// Model: claude-sonnet-5
// Approach: split each line into words (runs of non-space ASCII 33-126
// separated by runs of the space character). For column i (1-indexed),
// its starting position p_i is p_1 = 1, and for i >= 2, p_i is the
// minimal position such that every line that has a word at column i-1
// has that word ending at or before p_i - 2; i.e.
//   p_i = 2 + max over lines with >= i-1 words of (p_{i-1} + len(word_{i-1}) - 1)
// This only ever depends on lines that actually have a word in the
// previous column, matching the statement's wording exactly (a line
// missing that column does not constrain the next column's position).
// Output each line's words at their column positions with no trailing
// spaces after the last word on the line (a common PE trap noted on the
// discuss board) and build the whole output in one buffer to avoid the
// per-character cout/printf slowdown other submitters hit (also reported
// on the board as causing a spurious TLE).
// No statement ambiguity beyond the PE trap above, which the board's own
// sample fragment ("s: string; / c: char; // temp") makes explicit.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

int main() {
    vector<vector<string> > lines;
    static char buf[512];
    while (fgets(buf, sizeof(buf), stdin)) {
        size_t len = strlen(buf);
        while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r')) {
            buf[--len] = '\0';
        }
        vector<string> words;
        size_t i = 0;
        while (i < len) {
            while (i < len && buf[i] == ' ') ++i;
            if (i >= len) break;
            size_t start = i;
            while (i < len && buf[i] != ' ') ++i;
            words.push_back(string(buf + start, buf + i));
        }
        // Statement guarantees each line has at least one word, but guard
        // anyway against a stray blank line rather than crash on it.
        if (!words.empty()) lines.push_back(words);
    }

    size_t maxCols = 0;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].size() > maxCols) maxCols = lines[i].size();
    }

    vector<int> pos(maxCols + 1, 0);
    if (maxCols >= 1) pos[1] = 1;
    for (size_t col = 2; col <= maxCols; ++col) {
        int best = 0;
        for (size_t i = 0; i < lines.size(); ++i) {
            if (lines[i].size() >= col - 1) {
                int end = pos[col - 1] + (int)lines[i][col - 2].size() - 1;
                if (end > best) best = end;
            }
        }
        pos[col] = best + 2;
    }

    string out;
    out.reserve(lines.size() * 200 + 16);
    for (size_t i = 0; i < lines.size(); ++i) {
        int cur = 1;
        for (size_t j = 0; j < lines[i].size(); ++j) {
            int target = pos[j + 1];
            for (int k = cur; k < target; ++k) out.push_back(' ');
            out += lines[i][j];
            cur = target + (int)lines[i][j].size();
        }
        out.push_back('\n');
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
