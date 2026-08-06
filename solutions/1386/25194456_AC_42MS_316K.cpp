// POJ 1386 - Play on Words
//
// Each plate is a directed edge from its word's first letter to its last, so
// chaining every plate exactly once is asking for a directed Eulerian path
// over the 26 letters. The body of each word never matters -- only its two end
// characters -- so the reader keeps just those and discards the rest, which is
// what keeps an input of up to 100000 words of 1000 characters affordable.
//
// Two conditions, both required:
//   * Degrees (the *directed* form, not the undirected one from POJ 2513):
//     either every letter has in == out (an Eulerian circuit), or exactly one
//     letter has out == in + 1 -- the start, the vertex carrying the surplus
//     outgoing edge -- and exactly one has in == out + 1, the end, with all
//     others balanced. Any |out - in| >= 2 kills it outright.
//   * Connectivity: all letters that appear in any word must lie in a single
//     weakly connected component. A DSU over the 26 letters, uniting the two
//     endpoints of every word, settles this; letters that never appear are
//     ignored. Without this check two separately balanced groups of words
//     (e.g. "ab","ba" plus "cd","dc") would pass the degree test yet admit no
//     ordering.
//
// Ambiguity worth noting: the statement never says the words are distinct --
// "The same word may appear several times" -- so repeats are parallel edges,
// counted with multiplicity, which multigraph Euler theory already handles.
// A single word suffices on its own (a one-edge path), including one whose
// first and last letter coincide (a self-loop, in == out == 1).
//
// O(total input length) time, O(1) extra memory.

#include <cstdio>
#include <cstring>

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* Reads the next whitespace-delimited token, storing only its first and last
   character -- the body of the word is irrelevant to the puzzle. */
static bool readWord(int &first, int &last) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    first = c;
    last = c;
    c = gc();
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        last = c;
        c = gc();
    }
    return true;
}

static bool readInt(int &v) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int r = 0;
    while (c >= '0' && c <= '9') { r = r * 10 + (c - '0'); c = gc(); }
    v = r * sign;
    return true;
}

static int par[26];

static int find(int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

static void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a != b) par[a] = b;
}

int main() {
    int T;
    if (!readInt(T)) return 0;
    while (T-- > 0) {
        int n;
        if (!readInt(n)) break;
        int outd[26], ind[26];
        bool used[26];
        for (int i = 0; i < 26; ++i) { outd[i] = ind[i] = 0; used[i] = false; par[i] = i; }
        for (int i = 0; i < n; ++i) {
            int f, l;
            if (!readWord(f, l)) { f = 'a'; l = 'a'; }
            int a = f - 'a', b = l - 'a';
            ++outd[a]; ++ind[b];
            used[a] = used[b] = true;
            unite(a, b);
        }
        /* Weak connectivity over the letters that actually appear. */
        bool ok = true;
        int root = -1;
        for (int i = 0; i < 26; ++i) {
            if (!used[i]) continue;
            if (root == -1) root = find(i);
            else if (find(i) != root) { ok = false; break; }
        }
        /* Directed Eulerian path degrees: at most one vertex with one more
           outgoing edge than incoming (the start), at most one with one more
           incoming than outgoing (the end), everything else balanced. */
        if (ok) {
            int plus = 0, minus = 0;
            for (int i = 0; i < 26; ++i) {
                int d = outd[i] - ind[i];
                if (d == 0) continue;
                if (d == 1) ++plus;
                else if (d == -1) ++minus;
                else { ok = false; break; }
            }
            if (ok && !((plus == 0 && minus == 0) || (plus == 1 && minus == 1))) ok = false;
        }
        puts(ok ? "Ordering is possible." : "The door cannot be opened.");
    }
    return 0;
}
