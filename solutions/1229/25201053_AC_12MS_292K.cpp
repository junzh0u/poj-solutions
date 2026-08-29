// POJ 1229 - Wild Domains
// Model: claude-sonnet-5
//
// Each domain pattern is a sequence of tokens separated by dots. A token is
// either a literal word, or one of the wildcards *, ?, ! matching a run of
// consecutive domain parts: * >=1, ? in [1,3], ! >=3.  Two patterns "match"
// if some domain name (sequence of >=1 parts; the input guarantees each
// pattern already has >=2 tokens, so any valid alignment has length >=2) can
// be partitioned simultaneously according to both patterns' token sequences.
//
// A shared position can only create a *constraint* when both patterns place
// a literal token exactly on that single position (literal tokens always
// occupy exactly one part) -- then the two literal words must be equal.  If
// either side's token covering that position is a wildcard, there is no
// constraint, because wildcard content is free.  So we can do a linear scan
// over the (unknown) sequence of domain parts, tracking how far into the
// "current" token of each pattern we are, and require token ranges [min,max]
// to be respected; because min<=3 and max is either <=3 or infinite for all
// token kinds, the "amount consumed so far in the current token" only needs
// to be tracked up to a cap of 3 (3 meaning ">=3"). This gives a small
// reachability graph on states (i, ci, j, cj) explored with a BFS/DFS:
//   - close token i (if ci >= min_i): move to token i+1 with ci=0
//   - close token j similarly
//   - consume one shared part: ci++, cj++ (capped at 3), allowed only if
//     each side's max isn't already exhausted; if both current tokens are
//     literal, this is the position both literals must occupy, so require
//     their strings to be equal.
// Answer YES iff the state (nA, *, nB, *) is reachable from (0,0,0,0).
//
// Ambiguity check: the two "!.a" vs "!.a" and the a.*.a.?.?.?/a.a.a.? test
// cases posted on the discuss board were used to confirm min(!) = 3 (>=3,
// not >3) and that range-sum feasibility alone isn't sufficient in general
// (position-level literal collisions matter) -- both were verified against
// this DP by hand and match the board's reported answers.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct Token {
    int kind; // 0 = literal, 1 = '*', 2 = '?', 3 = '!'
    string word;
    int minR, maxR; // maxR = INF sentinel for unbounded
};

static const int INF = 1000;

vector<Token> parsePattern(const string &line) {
    vector<Token> toks;
    size_t start = 0;
    for (size_t i = 0; i <= line.size(); ++i) {
        if (i == line.size() || line[i] == '.') {
            string part = line.substr(start, i - start);
            Token t;
            if (part == "*") { t.kind = 1; t.minR = 1; t.maxR = INF; }
            else if (part == "?") { t.kind = 2; t.minR = 1; t.maxR = 3; }
            else if (part == "!") { t.kind = 3; t.minR = 3; t.maxR = INF; }
            else { t.kind = 0; t.word = part; t.minR = 1; t.maxR = 1; }
            toks.push_back(t);
            start = i + 1;
        }
    }
    return toks;
}

int cap(int x) { return x > 3 ? 3 : x; }

bool matches(const vector<Token> &A, const vector<Token> &B) {
    int nA = (int)A.size(), nB = (int)B.size();
    // state index: ((i*4+ci)*(nB+1)+j)*4+cj  with i in [0,nA], j in [0,nB]
    // ci/cj only meaningful when i<nA / j<nB, else treated as 0.
    vector<char> visited((size_t)(nA + 1) * 4 * (nB + 1) * 4, 0);
    // index function
    struct Idx {
        int nB;
        Idx(int nb) : nB(nb) {}
        int operator()(int i, int ci, int j, int cj) const {
            return (((i * 4 + ci) * (nB + 1) + j) * 4 + cj);
        }
    } idx(nB);

    queue<int> qi; queue<int> qci; queue<int> qj; queue<int> qcj;
    qi.push(0); qci.push(0); qj.push(0); qcj.push(0);
    visited[idx(0,0,0,0)] = 1;

    while (!qi.empty()) {
        int i = qi.front(); qi.pop();
        int ci = qci.front(); qci.pop();
        int j = qj.front(); qj.pop();
        int cj = qcj.front(); qcj.pop();

        if (i == nA && j == nB) return true;

        // close A token i
        if (i < nA && ci >= A[i].minR) {
            int id = idx(i + 1, 0, j, cj);
            if (!visited[id]) { visited[id] = 1; qi.push(i+1); qci.push(0); qj.push(j); qcj.push(cj); }
        }
        // close B token j
        if (j < nB && cj >= B[j].minR) {
            int id = idx(i, ci, j + 1, 0);
            if (!visited[id]) { visited[id] = 1; qi.push(i); qci.push(ci); qj.push(j+1); qcj.push(0); }
        }
        // consume a shared part
        if (i < nA && j < nB) {
            bool canA = (A[i].maxR >= INF) || (ci < A[i].maxR);
            bool canB = (B[j].maxR >= INF) || (cj < B[j].maxR);
            if (canA && canB) {
                bool ok = true;
                if (A[i].kind == 0 && B[j].kind == 0) {
                    ok = (A[i].word == B[j].word);
                }
                if (ok) {
                    int nci = cap(ci + 1), ncj = cap(cj + 1);
                    int id = idx(i, nci, j, ncj);
                    if (!visited[id]) { visited[id] = 1; qi.push(i); qci.push(nci); qj.push(j); qcj.push(ncj); }
                }
            }
        }
    }
    return false;
}

string trimLine(const string &s) {
    size_t b = 0, e = s.size();
    while (e > b && (s[e-1] == '\r' || s[e-1] == '\n' || s[e-1] == ' ' || s[e-1] == '\t')) --e;
    while (b < e && (s[b] == ' ' || s[b] == '\t')) ++b;
    return s.substr(b, e - b);
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    char buf[1024];
    fgets(buf, sizeof(buf), stdin); // consume rest of line
    while (t--) {
        string line1, line2;
        if (fgets(buf, sizeof(buf), stdin)) line1 = trimLine(buf);
        if (fgets(buf, sizeof(buf), stdin)) line2 = trimLine(buf);
        vector<Token> A = parsePattern(line1);
        vector<Token> B = parsePattern(line2);
        printf(matches(A, B) ? "YES\n" : "NO\n");
    }
    return 0;
}
