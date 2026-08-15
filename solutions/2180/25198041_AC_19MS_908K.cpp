// POJ 2180 - Bale Figures
// Model: claude-sonnet-5
// Approach: place bale 1 at origin; each subsequent bale is placed at a unit
// offset (L/R/F/B/O/U) from a previously placed bale, tracked in a bounded
// 3D occupancy grid (coordinates are guaranteed within 25 of the first bale,
// per the statement and confirmed on the discuss board). If a new bale's
// cell is already occupied, the shape is invalid -> print -1 (after still
// consuming the rest of the input). Otherwise the exposed surface area is
// the count, over all bales and all 6 unit faces, of faces whose neighbor
// cell is unoccupied, except a bale's bottom (-z) face is never exposed
// when the bale sits on the floor (z == 0) -- it touches the floor instead.
// No other constraint (e.g. enclosed air pockets) applies, per the board.
#include <cstdio>
#include <cstring>

const int OFFSET = 60;
const int DIM = 121; // covers coordinates in [-60, 60], well beyond the
                      // stated +-25 bound from the first bale.

static bool grid[DIM][DIM][DIM];
static int px[25001], py[25001], pz[25001];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    px[1] = py[1] = pz[1] = 0;
    grid[OFFSET][OFFSET][OFFSET] = true;
    bool invalid = false;

    for (int i = 2; i <= n; i++) {
        int j;
        char dirbuf[8];
        scanf("%d %7s", &j, dirbuf);
        char d = dirbuf[0];
        int x = px[j], y = py[j], z = pz[j];
        if (d == 'R') x++;
        else if (d == 'L') x--;
        else if (d == 'F') y++;
        else if (d == 'B') y--;
        else if (d == 'O') z++;
        else if (d == 'U') z--;
        px[i] = x; py[i] = y; pz[i] = z;

        int gx = x + OFFSET, gy = y + OFFSET, gz = z + OFFSET;
        if (gx < 0 || gx >= DIM || gy < 0 || gy >= DIM || gz < 0 || gz >= DIM) {
            invalid = true; // out of the guaranteed range; treat defensively
            continue;
        }
        if (grid[gx][gy][gz]) invalid = true;
        grid[gx][gy][gz] = true;
    }

    if (invalid) {
        printf("-1\n");
        return 0;
    }

    int dx[6] = {1, -1, 0, 0, 0, 0};
    int dy[6] = {0, 0, 1, -1, 0, 0};
    int dz[6] = {0, 0, 0, 0, 1, -1};

    long area = 0;
    for (int i = 1; i <= n; i++) {
        int x = px[i], y = py[i], z = pz[i];
        for (int k = 0; k < 6; k++) {
            if (dz[k] == -1 && z == 0) continue; // bottom face on the floor
            int nx = x + dx[k], ny = y + dy[k], nz = z + dz[k];
            int gx = nx + OFFSET, gy = ny + OFFSET, gz = nz + OFFSET;
            if (!grid[gx][gy][gz]) area++;
        }
    }

    printf("%ld\n", area);
    return 0;
}
