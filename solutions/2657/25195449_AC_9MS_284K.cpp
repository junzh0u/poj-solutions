// POJ 2657 - Comfort
// Model: claude-sonnet-5
// Approach: fields are numbered 1..N around a circle. For candidate jump
// length K = 1, 2, 3, ... (K < N; K = Z-1 always works via a single direct
// jump since field Z is guaranteed obstacle-free, so a valid K always exists
// below N), simulate the clockwise walk starting at field 1: pos = pos + K,
// and only subtract N when pos > N (never take pos % N directly, since a
// walk landing exactly on N must stay N, not wrap to 0 - the board flags
// this exact off-by-one). Stop the walk on reaching Z (success), on landing
// on an obstacle (this K fails), or on returning to field 1 without ever
// hitting Z (K fails, since the walk only cycles from there). The first K
// that reaches Z is the answer. Total simulated work across all K is
// sum_{K=1}^{N-1} N/gcd(K,N), comfortably small for N <= 1000.
//
// Statement ambiguity: nothing in the statement mentions multiple datasets,
// but the discuss board (message 43944 thread) has two independent users
// reporting WA on a single-dataset reading and AC after wrapping the read
// in while(scanf()!=EOF) (one dissenting reply claims it's single-case, but
// with far weaker corroboration - a lone reply, no reported verdict change).
// Reading to EOF is strictly safer: it subsumes genuine single-dataset input
// (the loop just runs once and then hits real EOF), so it was used
// unconditionally.
#include <cstdio>
#include <cstring>

int main() {
    int N, Z, M;
    while (scanf("%d %d %d", &N, &Z, &M) == 3) {
        bool obstacle[1002];
        memset(obstacle, 0, sizeof(obstacle));
        for (int i = 0; i < M; ++i) {
            int x;
            scanf("%d", &x);
            obstacle[x] = true;
        }
        int answer = -1;
        for (int K = 1; K < N && answer == -1; ++K) {
            int pos = 1;
            bool ok = false;
            while (true) {
                pos += K;
                if (pos > N) pos -= N;
                if (pos == Z) { ok = true; break; }
                if (obstacle[pos]) break;
                if (pos == 1) break; // cycled back to start without success
            }
            if (ok) answer = K;
        }
        printf("%d\n", answer);
    }
    return 0;
}
