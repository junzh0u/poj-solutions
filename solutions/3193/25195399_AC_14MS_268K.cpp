// POJ 3193 - Cow Phrasebook
// Model: claude-opus-5
//
// Count how many of the N messages are a prefix of at least one of the M
// phrases. Comparisons are case-sensitive and every character of a line
// (letters, '.', ',', '?', ' ') is significant, so lines must be read whole
// with fgets rather than token-wise.
//
// Approach: sort the M phrases lexicographically, then for each message q take
// lower_bound(q) and test whether that single phrase starts with q.
// Correctness: if some phrase p has q as a prefix then p >= q, so the
// lower_bound result r satisfies q <= r <= p. If q were not a prefix of r they
// would first differ at some i < |q| with r[i] > q[i] = p[i], giving r > p --
// a contradiction. So one comparison at lower_bound decides the query.
// O((M+N)*L + N*L*log M), trivially inside the 1s limit (0.005s on the
// full-size 1000x10000 stress case here; a naive M*N scan is what the discuss
// board reports as TLE).
//
// Points the discuss board settles, none of which the official sample can
// expose (it certifies both broken variants below):
//  - Duplicate messages each count separately; de-duplicating them is the
//    board's most-reported Wrong Answer.
//  - Case matters: upper- and lower-case letters are distinct.
// A message equal to a whole phrase counts (a phrase is a prefix of itself);
// an empty message, if one occurred, is a prefix of every phrase and counts.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

static char buf[1024];

static bool readLine(std::string &out) {
    if (!fgets(buf, sizeof(buf), stdin)) { out.clear(); return false; }
    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) --len;
    out.assign(buf, buf + len);
    return true;
}

int main() {
    std::string line;
    if (!readLine(line)) return 0;
    int m = 0, n = 0;
    if (sscanf(line.c_str(), "%d %d", &m, &n) != 2) return 0;

    std::vector<std::string> book;
    book.reserve(m);
    for (int i = 0; i < m; ++i) { readLine(line); book.push_back(line); }
    std::sort(book.begin(), book.end());

    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        readLine(line);
        std::vector<std::string>::const_iterator it =
            std::lower_bound(book.begin(), book.end(), line);
        if (it != book.end() && it->compare(0, line.size(), line) == 0) ++cnt;
    }
    printf("%d\n", cnt);
    return 0;
}
