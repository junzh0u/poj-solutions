// POJ 1366 - Word
// Model: claude-sonnet-5
// A word of length n (2<n<16) over {a,b} is cyclic; a rewriting rule maps
// (w[i-2], w[i], w[i+1]) -> new w[i], applied simultaneously to every
// position (indices mod n). Given s up to 2e9 rewriting steps we must
// print the lexicographically smallest cyclic rotation of the resulting
// word.
//
// Since the word space has only 2^n <= 32768 possible states and the
// rewrite step is a deterministic function on that space, the sequence
// of states is eventually periodic (rho-shaped). We simulate at most
// 2^n+1 steps, recording each state, until a state repeats; that gives
// the tail length and the cycle length, from which the state after any
// s steps (s fits in a 32-bit int since s <= 2e9 < INT_MAX) can be
// looked up directly without simulating further.
//
// Ambiguity: the statement doesn't say how many blocks the input holds;
// the discuss board confirms each block is exactly 11 lines (n, word,
// 8 rules, s) and gives no total count, so blocks are read until EOF.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        char wordBuf[20];
        scanf("%s", wordBuf);
        int rule[8];
        for (int i = 0; i < 8; ++i) {
            char tok[10];
            scanf("%s", tok);
            int idx = (tok[0]=='b'?4:0) + (tok[1]=='b'?2:0) + (tok[2]=='b'?1:0);
            rule[idx] = (tok[3]=='b') ? 1 : 0;
        }
        int s;
        scanf("%d", &s);

        int initVal = 0;
        for (int i = 0; i < n; ++i) if (wordBuf[i]=='b') initVal |= (1<<i);

        int sz = 1 << n;
        vector<int> visited(sz, -1);
        vector<int> seq;
        seq.reserve(sz + 1);
        seq.push_back(initVal);
        visited[initVal] = 0;
        int step = 0;
        int j = -1, L = -1;
        while (true) {
            int cur = seq[step];
            int nxt = 0;
            for (int i = 0; i < n; ++i) {
                int im2 = (i - 2 + n) % n;
                int ip1 = (i + 1) % n;
                int c1 = (cur >> im2) & 1;
                int c2 = (cur >> i) & 1;
                int c3 = (cur >> ip1) & 1;
                int idx = c1*4 + c2*2 + c3;
                if (rule[idx]) nxt |= (1 << i);
            }
            int nextStepNum = step + 1;
            if (visited[nxt] != -1) {
                j = visited[nxt];
                L = nextStepNum - j;
                break;
            }
            visited[nxt] = nextStepNum;
            seq.push_back(nxt);
            step = nextStepNum;
        }

        int ansVal;
        if (s <= step) {
            ansVal = seq[s];
        } else {
            int idx = j + (s - j) % L;
            ansVal = seq[idx];
        }

        char buf[20];
        for (int i = 0; i < n; ++i) buf[i] = ((ansVal>>i)&1) ? 'b':'a';
        buf[n] = 0;

        char best[20];
        strcpy(best, buf);
        for (int k = 1; k < n; ++k) {
            char rot[20];
            for (int i = 0; i < n; ++i) rot[i] = buf[(i+k)%n];
            rot[n] = 0;
            if (strcmp(rot, best) < 0) strcpy(best, rot);
        }
        printf("%s\n", best);
    }
    return 0;
}
