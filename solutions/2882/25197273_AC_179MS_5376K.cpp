// POJ 2882 - Food Cubes
// Model: gpt-5.6-terra
// Flood-fill the outside of the fixed coordinate box.  Every remaining
// connected empty component is enclosed by cubes and is therefore a hole.
// Coordinates are deliberately kept on 0..101: the added one-cell shell is
// the exterior, including when a cube lies at a stated coordinate boundary.
#include <cstdio>
#include <cstring>

const int SIDE = 102;
const int CELLS = SIDE * SIDE * SIDE;

char state[CELLS];
int que[CELLS];

int id(int x, int y, int z) {
    return (x * SIDE + y) * SIDE + z;
}

void fill(int start) {
    int head = 0, tail = 0;
    que[tail++] = start;
    state[start] = 2;
    while (head < tail) {
        int p = que[head++];
        int z = p % SIDE;
        int q = p / SIDE;
        int y = q % SIDE;
        int x = q / SIDE;
        int n;
        if (x > 0) {
            n = p - SIDE * SIDE;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
        if (x + 1 < SIDE) {
            n = p + SIDE * SIDE;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
        if (y > 0) {
            n = p - SIDE;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
        if (y + 1 < SIDE) {
            n = p + SIDE;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
        if (z > 0) {
            n = p - 1;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
        if (z + 1 < SIDE) {
            n = p + 1;
            if (!state[n]) { state[n] = 2; que[tail++] = n; }
        }
    }
}

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        std::memset(state, 0, sizeof(state));
        int m;
        std::scanf("%d", &m);
        while (m--) {
            int x, y, z;
            std::scanf("%d%d%d", &x, &y, &z);
            state[id(x, y, z)] = 1;
        }
        fill(0);
        int holes = 0;
        int p;
        for (p = 0; p < CELLS; ++p) {
            if (!state[p]) {
                ++holes;
                fill(p);
            }
        }
        std::printf("%d\n", holes);
    }
    return 0;
}
