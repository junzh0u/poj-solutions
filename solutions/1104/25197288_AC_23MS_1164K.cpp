// POJ 1104 - Robbery
// Model: gpt-5.6-terra
// Keep every position not ruled out by observations, propagate reachability
// forward, then discard positions that cannot lead to a later feasible time.
// A move may stay in place or change one coordinate by one.
#include <cstdio>
#include <cstring>

static bool possible[101][101][101];

static bool adjacent(const bool grid[101][101], int x, int y, int w, int h) {
    return grid[x][y]
        || (x > 1 && grid[x - 1][y])
        || (x < w && grid[x + 1][y])
        || (y > 1 && grid[x][y - 1])
        || (y < h && grid[x][y + 1]);
}

int main() {
    int w, h, t;
    int case_no = 1;
    while (std::scanf("%d%d%d", &w, &h, &t) == 3 && (w || h || t)) {
        std::memset(possible, 1, sizeof(possible));
        int n;
        std::scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            int time, left, top, right, bottom;
            std::scanf("%d%d%d%d%d", &time, &left, &top, &right, &bottom);
            for (int x = left; x <= right; ++x)
                for (int y = top; y <= bottom; ++y)
                    possible[time][x][y] = false;
        }

        for (int time = 2; time <= t; ++time)
            for (int x = 1; x <= w; ++x)
                for (int y = 1; y <= h; ++y)
                    if (possible[time][x][y] && !adjacent(possible[time - 1], x, y, w, h))
                        possible[time][x][y] = false;

        for (int time = t - 1; time >= 1; --time)
            for (int x = 1; x <= w; ++x)
                for (int y = 1; y <= h; ++y)
                    if (possible[time][x][y] && !adjacent(possible[time + 1], x, y, w, h))
                        possible[time][x][y] = false;

        std::printf("Robbery #%d:\n", case_no++);
        bool escaped = false;
        bool any_known = false;
        int count_at[101], x_at[101], y_at[101];
        for (int time = 1; time <= t; ++time) {
            int count = 0, known_x = 0, known_y = 0;
            for (int x = 1; x <= w; ++x)
                for (int y = 1; y <= h; ++y)
                    if (possible[time][x][y]) {
                        ++count;
                        known_x = x;
                        known_y = y;
                    }
            count_at[time] = count;
            x_at[time] = known_x;
            y_at[time] = known_y;
            if (count == 0) escaped = true;
            if (count == 1) any_known = true;
        }
        if (escaped) {
            std::printf("The robber has escaped.\n");
        } else if (!any_known) {
            std::printf("Nothing known.\n");
        } else {
            for (int time = 1; time <= t; ++time)
                if (count_at[time] == 1)
                    std::printf("Time step %d: The robber has been at %d,%d.\n", time, x_at[time], y_at[time]);
        }
        std::printf("\n");
    }
    return 0;
}
