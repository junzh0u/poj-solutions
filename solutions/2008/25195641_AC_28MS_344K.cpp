// POJ 2008 - Moo University - Team Tryouts
// Model: claude-opus-5
//
// A team is legal iff every member satisfies A*(H-h) + B*(W-w) <= C, where h
// and w are the minimum height and the minimum weight over the team. Ambiguity:
// h and w are two independent minima and need not come from the same calf; the
// official sample and the discuss board's posted case are both invariant under
// the "same calf" misreading, so a separating case was constructed instead
// (3 calves, A B C = 1120 783 5717, (6,4) (10,2) (7,7): 3 under the correct
// reading, 2 under the misreading).
//
// Key fact: maximizing over ALL pairs (h,w) of observed values is safe, no
// feasibility check needed. If a counted set's true minima (h'',w'') exceed the
// enumerated (h,w), every member still satisfies the constraint because the
// right-hand side C + A*h + B*w only grows. So the answer is
//   max over (h,w) of |{k : H_k >= h, W_k >= w, A*H_k + B*W_k <= C + A*h + B*w}|.
//
// O(N^2), the official USACO analysis: fix h, sweep w upward over the calves in
// weight order. The linear-constraint bound C + A*h + B*w is monotone in w, so
// calves enter the counted set once and for all in order of A*H + B*W (single
// pointer k); a calf is admitted only if it also meets the current H >= h and
// W >= w, and each calf is dropped again when the sweep passes its own weight.
// Ties in weight undercount at every position of a tie group but the first,
// which is harmless since only the maximum is kept.
//
// long long throughout because A*H + B*W reaches ~2e9 with the stated bounds,
// and hence language 0 (G++): POJ's "C++" is VC6, which has no long long, and
// the board reports the same code as G++-Accepted / C++-TLE.

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

static int N;
static long long A, B, C;
static long long H[1005], W[1005], T[1005];
static int byW[1005], byT[1005];
static bool counted[1005];

struct CmpW {
    bool operator()(int a, int b) const { return W[a] < W[b]; }
};
struct CmpT {
    bool operator()(int a, int b) const { return T[a] < T[b]; }
};

int main() {
    int n, a, b, c, h, w;
    while (scanf("%d", &n) == 1) {
        if (scanf("%d %d %d", &a, &b, &c) != 3) break;
        N = n;
        A = a; B = b; C = c;
        for (int i = 0; i < N; i++) {
            if (scanf("%d %d", &h, &w) != 2) return 0;
            H[i] = h;
            W[i] = w;
            T[i] = A * H[i] + B * W[i];
            byW[i] = i;
            byT[i] = i;
        }
        sort(byW, byW + N, CmpW());
        sort(byT, byT + N, CmpT());

        int best = 0;
        for (int i = 0; i < N; i++) {
            long long minh = H[i];
            memset(counted, 0, sizeof(bool) * N);
            int cnt = 0;
            int k = 0;
            for (int jj = 0; jj < N; jj++) {
                int j = byW[jj];
                long long minw = W[j];
                long long lim = A * minh + B * minw + C;
                while (k < N && T[byT[k]] <= lim) {
                    int p = byT[k];
                    if (H[p] >= minh && W[p] >= minw) {
                        counted[p] = true;
                        cnt++;
                    }
                    k++;
                }
                if (cnt > best) best = cnt;
                if (counted[j]) {
                    counted[j] = false;
                    cnt--;
                }
            }
        }
        printf("%d\n", best);
    }
    return 0;
}
