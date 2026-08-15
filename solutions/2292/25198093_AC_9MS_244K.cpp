// POJ 2292 - Optimal Keypad
// Model: claude-sonnet-5
//
// The 30 characters "abcdefghijklmnopqrstuvwxyz+*/?" sit on a tape in that
// fixed order and must be cut into 12 contiguous, non-empty labels. Typing
// the n-th character of a label costs n keystrokes, so the total cost over
// the dictionary depends only on where the 11 cuts fall: cost(a,b) = sum
// over positions p in [a,b] of freq[p]*(p-a+1), where freq[p] is how many
// times character p occurs across all words (word boundaries don't matter,
// only per-character counts). Minimize sum of cost over the 12 segments via
// a straightforward DP: D2[i][k] = min cost to split the suffix starting at
// position i into k labels (i, k <= 30 chars/12 labels, trivial O(N^2*K)).
//
// Tie-break: "the smallest cut string in lexicographic order" is ambiguous
// for a judge whose own discuss board raised exactly this question
// (message_id=12923, "+/*?these how to count for lexicographic order?")
// and never got answered, because '+','*','/','?' sort before all letters
// in ASCII but after 'z' in the tape's own order. This solution takes
// "lexicographic order" to mean the order the problem itself defines on
// its alphabet -- the tape's left-to-right order a<b<...<z<+<*</<? --
// since that is the only order the problem statement actually establishes,
// and it is what the natural DP reconstruction produces "for free" (always
// preferring the earliest/smallest feasible cut position at each of the 11
// steps, since all 30 tape characters are distinct so no further tie-break
// is ever needed). Reconstruction walks the 11 cuts left to right, and at
// each step picks the smallest feasible next cut position consistent with
// the overall optimal cost (checked against the suffix DP table), which is
// provably the lexicographically-smallest (tape order) choice for that
// position regardless of later decisions.
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

const int N = 30;
const int K = 12;
const int INF = 1000000000;

const char S[] = "abcdefghijklmnopqrstuvwxyz+*/?";

int idx[128];

int cost[N+1][N+1]; // cost(a,b) 0-indexed inclusive, a<=b
int D2[N+2][K+1];

int main(){
    for (int i = 0; i < N; i++) idx[(unsigned char)S[i]] = i;
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int m;
        scanf("%d", &m);
        int freq[N];
        memset(freq, 0, sizeof(freq));
        char buf[64];
        for (int i = 0; i < m; i++) {
            scanf("%s", buf);
            for (int j = 0; buf[j]; j++) {
                freq[idx[(unsigned char)buf[j]]]++;
            }
        }
        // cost(a,b) = sum_{p=a}^{b} freq[p]*(p-a+1)
        for (int a = 0; a < N; a++) {
            int acc = 0;
            int cnt = 0;
            for (int b = a; b < N; b++) {
                cnt++;
                acc += freq[b] * cnt;
                cost[a][b] = acc;
            }
        }
        // D2[i][k]: min cost to partition chars i..N-1 into k groups. D2[N][0]=0
        for (int i = 0; i <= N; i++)
            for (int k = 0; k <= K; k++) D2[i][k] = INF;
        D2[N][0] = 0;
        for (int i = N-1; i >= 0; i--) {
            for (int k = 1; k <= K; k++) {
                if (N - i < k) continue; // not enough chars
                int best = INF;
                // end ranges i..N-1, with N-(end+1) >= k-1 => end <= N-k
                int maxEnd = N - k;
                for (int end = i; end <= maxEnd; end++) {
                    int v = cost[i][end] + D2[end+1][k-1];
                    if (v < best) best = v;
                }
                D2[i][k] = best;
            }
        }
        // reconstruct
        string result;
        int start = 0, groupsLeft = K;
        for (int j = 0; j < 11; j++) {
            int maxEnd = N - groupsLeft; // end <= N - groupsLeft (so remaining groupsLeft-1 groups fit)
            int need = D2[start][groupsLeft];
            int bestEnd = -1;
            for (int end = start; end <= maxEnd; end++) {
                int v = cost[start][end] + D2[end+1][groupsLeft-1];
                if (v == need) {
                    // feasible; smallest end+1 gives earliest tape-order character
                    bestEnd = end;
                    break; // since end increasing, first feasible is smallest end
                }
            }
            result += S[bestEnd+1];
            start = bestEnd + 1;
            groupsLeft--;
        }
        printf("%s\n", result.c_str());
    }
    return 0;
}
