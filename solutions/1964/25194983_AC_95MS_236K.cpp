// POJ 1964 - City Game
// Model: claude-sonnet-5
// Approach: classic largest-rectangle-in-histogram, applied row by row.
// For each row, height[j] accumulates consecutive 'F' cells ending at that
// row in column j (reset to 0 on 'R'). The largest all-'F' rectangle whose
// bottom edge is this row is the largest rectangle in that row's histogram,
// found in O(N) with a monotonic stack (sentinels height=0 at both ends).
// Overall O(M*N) per dataset. Answer is 3 * (max rectangle area) over all
// rows of all datasets.
// Ambiguity note: the discuss board reports the real input data does not
// strictly follow the stated "symbols separated by a blank space, one row
// per line" layout -- extra/missing whitespace and newlines appear. Reading
// purely token-by-token with scanf("%s", ...) sidesteps this: it skips any
// amount of whitespace (spaces, tabs, newlines) between tokens regardless of
// how the grid rows are actually broken up, so the malformed layout never
// matters.
#include <cstdio>
#include <cstring>

const int MAXN = 1005;
int height[MAXN];
int stk[MAXN];

int main() {
    int K;
    if (scanf("%d", &K) != 1) return 0;
    while (K--) {
        int M, N;
        scanf("%d %d", &M, &N);
        memset(height, 0, sizeof(int) * (N + 2));
        int maxArea = 0;
        char buf[8];
        for (int i = 0; i < M; ++i) {
            for (int j = 1; j <= N; ++j) {
                scanf("%s", buf);
                if (buf[0] == 'F') height[j]++;
                else height[j] = 0;
            }
            height[0] = 0;
            height[N + 1] = 0;
            int top = 0;
            stk[0] = 0;
            for (int j = 1; j <= N + 1; ++j) {
                while (height[stk[top]] > height[j]) {
                    int h = height[stk[top--]];
                    int width = j - stk[top] - 1;
                    int area = h * width;
                    if (area > maxArea) maxArea = area;
                }
                stk[++top] = j;
            }
        }
        printf("%d\n", maxArea * 3);
    }
    return 0;
}
