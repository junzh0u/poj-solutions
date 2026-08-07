// POJ 3437 - Tree Grafting
// Model: claude-sonnet-5
// Approach: parse each line as a DFS "d"/"u" trace of an ordered tree.
// h1 (original height) is simply the max depth reached while walking the
// trace: depth += 1 on 'd', -= 1 on 'u'.
// h2 (height after left-child/right-sibling binary conversion): a node's
// binary depth is parent's binary depth + 1 if it is the first child, or
// previous sibling's binary depth + 1 otherwise (each sibling chain becomes
// a run of right edges in the binary tree). We track, per original-tree
// level, a "ref" value = the binary depth to add 1 to for the next child at
// that level; ref starts at the parent's own binary depth (first child) and
// is updated to the just-finished child's binary depth on 'u' (so the next
// sibling chains off it). Verified against the sample (dudduduudu -> 2=>4).
// No fixed-size input buffer is used: the discuss board reports RE from
// arrays sized only to the stated "<=10000 nodes" bound, since the trace
// itself needs up to ~2*(n-1) 'd'/'u' characters (one poster needed 20005
// after 18888 still overflowed) -- reading into std::string and sizing the
// per-line depth arrays off the actual token length sidesteps that bound
// entirely, whatever the real data size turns out to be. The board also
// flags a classic strlen-in-loop-condition TLE; this solution takes the
// length once via string::size().
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main(){
    string tok;
    int caseNum = 0;
    while (cin >> tok) {
        if (tok[0] == '#') break;
        int n = (int)tok.size();
        vector<int> bd(n + 2, 0), ref(n + 2, 0);
        int level = 0;
        bd[0] = 0;
        ref[0] = 0;
        int maxOrig = 0, maxBin = 0;
        for (int i = 0; i < n; ++i) {
            char c = tok[i];
            if (c == 'd') {
                int nb = ref[level] + 1;
                ++level;
                bd[level] = nb;
                ref[level] = nb;
                if (level > maxOrig) maxOrig = level;
                if (nb > maxBin) maxBin = nb;
            } else if (c == 'u') {
                ref[level - 1] = bd[level];
                --level;
            }
        }
        ++caseNum;
        printf("Tree %d: %d => %d\n", caseNum, maxOrig, maxBin);
    }
    return 0;
}
