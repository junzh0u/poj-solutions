// POJ 2330 - Advanced ASCII Cubes
// Model: gpt-5.6-terra
// Draw each visible cube into a dot-filled canvas.  Coordinates are measured
// upward from the bottom: a nearer table row starts two columns and two rows
// lower, and each higher cube starts three rows above the previous one.
// Only dots in the cube stencil are transparent; spaces erase edges hidden by
// a later face.  Cubes are painted bottom-to-top, then far-to-near and
// left-to-right, so nearer and upper visible faces have the final ownership.
#include <algorithm>
#include <cstdio>
#include <cstring>

using namespace std;

const char cube[6][8] = {
    "..+---+",
    "./   /|",
    "+---+ |",
    "|   | +",
    "|   |/.",
    "+---+.."
};

int height[50][50];
char canvas[400][310];

void drawCube(int tableRowFromFront, int column, int level) {
    int baseRow = tableRowFromFront * 2 + (level - 1) * 3;
    int baseColumn = column * 4 + tableRowFromFront * 2;
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 7; ++c) {
            char ch = cube[5 - r][c];
            if (ch != '.') {
                canvas[baseRow + r][baseColumn + c] = ch;
            }
        }
    }
}

int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;

    int maximumLevel = 0;
    int outputHeight = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%d", &height[i][j]);
            maximumLevel = max(maximumLevel, height[i][j]);
            outputHeight = max(outputHeight, 6 + (height[i][j] - 1) * 3 + 2 * (n - 1 - i));
        }
    }

    int outputWidth = m * 4 + 1 + 2 * n;
    memset(canvas, '.', sizeof(canvas));
    for (int r = 0; r < outputHeight; ++r) canvas[r][outputWidth] = '\0';

    for (int level = 1; level <= maximumLevel; ++level) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (height[i][j] >= level) drawCube(n - 1 - i, j, level);
            }
        }
    }

    for (int r = outputHeight - 1; r >= 0; --r) puts(canvas[r]);
    return 0;
}
