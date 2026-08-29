// POJ 3360 - Take a Walk
// Model: claude-sonnet-5
//
// Classic Euler trail/circuit test on an undirected multigraph (no self
// loops, parallel edges allowed). Only edges matter: vertices with degree 0
// (declared in the vertex set but touched by no edge) are ignored entirely,
// both for connectivity and for parity counting -- confirmed by the
// problem's discuss board (message 98429/170465): "isolated points, even
// though not part of a connected block, are still fine" is exactly the
// pitfall of treating them as breaking connectivity.
//
// Let C = number of connected components formed by the edges alone (i.e.
// components restricted to vertices of positive degree), and O = number of
// odd-degree vertices.
//   - An open trail (v0 != vk) covering every edge exists iff C == 1 and
//     O == 2 (the degenerate C == 0, O == 0 case can't occur since the
//     problem guarantees at least 2 edges, but is handled the same way the
//     accepted reference solution on the board handles it).
//   - A tour (closed walk covering every edge exactly once, i.e. an Euler
//     circuit) exists iff C <= 1 and O == 0.
//
// Input framing: the discuss board says a case need not be on a single
// line, and one case's raw line can run to ~13000 characters, so the whole
// input is read at once and processed as a character stream rather than by
// line or with scanf("%s") on a fixed buffer. The grammar is
//   Case <n>: { <vertices> } { (v,w), (v,w), ... }
// repeated to EOF. Parsing only relies on structural characters ('{' '}'
// '(' ')' and letters) and ignores everything else (case numbers, colons,
// commas, arbitrary whitespace/newlines), so it is insensitive to exact
// spacing. The vertex-list brace group's content is skipped outright since
// it never affects the answer once degree-0 vertices are ignored. Vertices
// are single letters, upper and lower case treated as distinct (sample
// case 3 uses both 'A'/'B' and 'c'/'d').
#include <cstdio>
#include <cctype>
#include <string>
#include <iostream>
#include <iterator>
#include <vector>

static int parent_[52];

static int find(int x) {
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];
        x = parent_[x];
    }
    return x;
}

static int idOf(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    return 26 + (c - 'A');
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string data((std::istreambuf_iterator<char>(std::cin)),
                      std::istreambuf_iterator<char>());
    size_t n = data.size();
    size_t pos = 0;

    std::string out;
    for (;;) {
        // Find the opening brace of the vertex list; if none remains, EOF.
        while (pos < n && data[pos] != '{') pos++;
        if (pos >= n) break;
        pos++; // skip '{'

        // Skip the vertex-list content entirely (never affects the answer).
        while (pos < n && data[pos] != '}') pos++;
        if (pos >= n) break; // malformed / truncated, stop
        pos++; // skip '}'

        // Find the opening brace of the edge list.
        while (pos < n && data[pos] != '{') pos++;
        if (pos >= n) break;
        pos++; // skip '{'

        int degree[52];
        bool used[52];
        for (int i = 0; i < 52; i++) { degree[i] = 0; used[i] = false; parent_[i] = i; }

        // Parse edges until the closing brace at the top level (outside any
        // parenthesis).
        while (pos < n && data[pos] != '}') {
            if (data[pos] == '(') {
                pos++;
                char v[2];
                int cnt = 0;
                while (pos < n && data[pos] != ')') {
                    if (isalpha((unsigned char)data[pos]) && cnt < 2) {
                        v[cnt++] = data[pos];
                    }
                    pos++;
                }
                if (pos < n) pos++; // skip ')'
                if (cnt == 2) {
                    int a = idOf(v[0]);
                    int b = idOf(v[1]);
                    degree[a]++;
                    degree[b]++;
                    used[a] = used[b] = true;
                    int ra = find(a), rb = find(b);
                    if (ra != rb) parent_[ra] = rb;
                }
            } else {
                pos++;
            }
        }
        if (pos < n) pos++; // skip '}'

        int comp = 0, odd = 0;
        for (int i = 0; i < 52; i++) {
            if (used[i] && find(i) == i) comp++;
            if (degree[i] % 2 == 1) odd++;
        }

        bool openTrail = (comp == 1 && odd == 2) || (comp == 0 && odd == 0);
        bool tour = (comp <= 1 && odd == 0);

        out += openTrail ? "Yes" : "No";
        out += ' ';
        out += tour ? "Yes" : "No";
        out += '\n';
    }

    std::fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
