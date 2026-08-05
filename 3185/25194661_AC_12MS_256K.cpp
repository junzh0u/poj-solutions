// POJ 3185 - The Water Bowls
// Model: claude-sonnet-5
// Approach: fixed-size (n=20) 1D "lights out" style flip puzzle. Flipping
// bowl i also flips its immediate neighbors. Once the flip choice for
// position 0 is fixed, every later flip is forced: to zero out bowl i-1
// (which no later flip can touch again), bowl i must be flipped iff bowl
// i-1 is currently 1. So try both choices for flipping position 0 (2
// possibilities), greedily propagate left to right, and after each full
// pass check whether all bowls ended up drinkable (0); take the minimum
// flip count among the valid runs. No statement ambiguity found.
#include <cstdio>
#include <cstring>
using namespace std;

const int N = 20;

void doFlip(int state[], int i) {
    state[i] ^= 1;
    if (i - 1 >= 0) state[i - 1] ^= 1;
    if (i + 1 < N) state[i + 1] ^= 1;
}

int main() {
    int orig[N];
    for (int i = 0; i < N; ++i) scanf("%d", &orig[i]);

    int best = -1;
    for (int start = 0; start <= 1; ++start) {
        int state[N];
        memcpy(state, orig, sizeof(orig));
        int flips = 0;
        if (start == 1) { doFlip(state, 0); ++flips; }
        for (int i = 1; i < N; ++i) {
            if (state[i - 1] == 1) { doFlip(state, i); ++flips; }
        }
        bool ok = true;
        for (int i = 0; i < N; ++i) if (state[i] != 0) { ok = false; break; }
        if (ok && (best == -1 || flips < best)) best = flips;
    }

    printf("%d\n", best);
    return 0;
}
