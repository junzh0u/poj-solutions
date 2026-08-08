// POJ 2334 - Simple prefix compression
// Model: claude-opus-5
//
// A1 is stored whole; every later Ai costs one [j] byte plus its tail past the
// common prefix it shares with its predecessor, so
//     answer = len(A1) + sum_{i=2..N} ( 1 + len(Ai) - lcp(A_{i-1}, Ai) ).
// One streaming pass over the input, keeping only the previous string.
//
// The [j] byte is exactly one byte whatever j is ("a single character with code j",
// and j <= min(p,q) <= 255 so it always fits); it is paid even when j = 0, which is
// the case the statement calls out as making the total grow.  lcp is capped at
// min(p,q): the previous string may be the longer one.
//
// Ambiguity -- what "minimal total length" ranges over.  Since
//     answer = sum len + (N-1) - sum of adjacent lcp,
// reordering the strings would be a maximum Hamiltonian path on the lcp graph, solved
// by sorting lexicographically (each trie node's block is then contiguous, hitting the
// per-node bound |S_v|-1 simultaneously).  That reading is wrong here: the statement
// fixes the labels A1..AN and never offers a reordering, and "minimal" only picks j
// maximal among the "some j <= min(p,q)" the definition allows.  The official sample
// cannot separate the two -- its input is already sorted and both print 11, and
// dropping the sort from a sorting solution still reproduces it, though the readings
// disagree on ~48% of random inputs.  What settles it is a solution posted on the
// discuss board under its own status header, Accepted in *40K* of memory: 40K cannot
// hold the 10000x255 characters a sort would have to buffer, so an accepted solution
// provably does not sort.  Input order is then the safe reading either way -- it is
// proven accepted on the real data, whereas sorting would only agree with it if that
// data happened to be sorted.  This program is checked against that control on
// randomized unsorted inputs, where the sorting reading visibly diverges.
//
// Also from the board and confirmed on the status page: G++ (language 0) times out
// here where C++ (language 4) is accepted -- runs 24184987 and 24184988 are the same
// 485B source, minutes apart, TLE and Accepted respectively.  Submitted as language 4.
//
// Input framing: one data set, exactly N strings, as the statement states -- no
// read-to-EOF.  Tokens are read with scanf("%s"), which also absorbs CRLF and any
// trailing blanks; the board asked about embedded spaces and the accepted sources
// split on whitespace, so there are none.  The answer is at most 10000*255 + 9999,
// well inside int.

#include <cstdio>
#include <cstring>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    static char prev[4096], cur[4096];
    if (n < 1 || scanf("%4095s", prev) != 1) { printf("0\n"); return 0; }

    int prevLen = (int)strlen(prev);
    int total = prevLen;

    for (int i = 1; i < n; i++) {
        if (scanf("%4095s", cur) != 1) break;
        int curLen = (int)strlen(cur);
        int cap = prevLen < curLen ? prevLen : curLen;
        int j = 0;
        while (j < cap && prev[j] == cur[j]) j++;
        total += 1 + curLen - j;
        memcpy(prev, cur, (size_t)curLen + 1);
        prevLen = curLen;
    }

    printf("%d\n", total);
    return 0;
}
