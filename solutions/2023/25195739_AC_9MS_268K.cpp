// POJ 2023 - Choose Your Own Adventure
// Model: claude-opus-5
//
// Each story is a directed graph on X <= 99 pages: a 'C' page has two out-edges,
// an 'E' page is a sink, exactly one of which is HAPPY. Print the page texts along
// the path from page 1 to that happy ending; the statement guarantees exactly one
// such story per data set.
//
// Depth-first search from page 1 with a permanent visited mark, keeping the
// recursion stack as the path. The mark is load-bearing rather than an
// optimisation: the judge data contains cycles (self-loops and back-edges), which
// the discuss board reports as the usual cause of RE/WA here, and the official
// sample is acyclic so it cannot expose the omission -- a mutant with the mark
// removed reproduces the sample byte-for-byte and blows the stack on a cyclic
// case. The mark is also safe under the uniqueness guarantee: DFS never leaves a
// node on the answer path unexplored, because reaching such a node by any other
// route would itself yield a second simple page-1-to-HAPPY path (an end page is a
// sink, so it can never be an interior node of the alternate prefix).
//
// Parsing is token-based rather than line-based: read the type character, then
// getline twice with '"' as the delimiter to skip to the opening quote and capture
// the text verbatim, then read the two choices or the ending word with >>. Text
// may contain spaces, digits and the words HAPPY/GRISLY, and this ignores line
// structure entirely, so CRLF endings and trailing spaces cost nothing.
//
// The leading count n is consumed but not trusted: data sets are read to EOF and
// numbered as they come, a superset of honouring n.

#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

static const int MAXP = 128;

int X;
char kind[MAXP];
string body[MAXP];
int nxt[MAXP][2];
bool happyEnd[MAXP];
bool seen[MAXP];
int path[MAXP];
int plen;

bool dfs(int u) {
    if (u < 1 || u > X) return false;
    if (seen[u]) return false;
    seen[u] = true;
    path[plen++] = u;
    if (kind[u] == 'E') {
        if (happyEnd[u]) return true;
    } else {
        if (dfs(nxt[u][0])) return true;
        if (dfs(nxt[u][1])) return true;
    }
    --plen;
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    if (!(cin >> n)) return 0;
    int story = 0;
    while (cin >> X) {
        if (X < 1 || X >= MAXP) return 0;
        for (int i = 1; i <= X; ++i) {
            char c;
            if (!(cin >> c)) return 0;
            kind[i] = c;
            string s;
            getline(cin, s, '"');
            getline(cin, s, '"');
            body[i] = s;
            nxt[i][0] = nxt[i][1] = 0;
            happyEnd[i] = false;
            if (c == 'C') {
                cin >> nxt[i][0] >> nxt[i][1];
            } else {
                string e;
                cin >> e;
                happyEnd[i] = (e == "HAPPY");
            }
        }
        for (int i = 1; i <= X; ++i) seen[i] = false;
        plen = 0;
        dfs(1);
        printf("STORY %d\n", ++story);
        for (int i = 0; i < plen; ++i)
            printf("%s\n", body[path[i]].c_str());
    }
    return 0;
}
