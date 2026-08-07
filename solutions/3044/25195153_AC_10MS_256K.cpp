// POJ 3044 - City Skyline
// Model: claude-opus-5
//
// The skyline is a per-column height profile; a building is a box [l,r] x [0,h],
// so the profile a set of buildings makes is the column-wise max of their heights.
// A building may never poke above the profile, so any building spanning [l,r] has
// height <= min h over [l,r]; and every column x with h(x) > 0 needs some building
// of height exactly h(x) covering it. Minimising the count is then: two columns of
// equal height can share one building iff no strictly lower column separates them.
//
// That is exactly what a monotonic stack decides in one pass. Keep the stack of
// heights that are still "open" (strictly increasing bottom to top). For each new
// height y: pop every open height greater than y -- a lower column now separates
// them from anything to the right, so those buildings are finished. If y then
// equals the stack top, this column reuses that still-open building and costs
// nothing; otherwise it opens a new one, billed only when y > 0 (height 0 is bare
// ground, not a building). O(N) time, O(N) stack.
//
// Only the y values matter: x and W never enter the answer, since the profile's
// shape, not its widths, decides which runs can share a building. Repeated
// consecutive y values (allowed by "strictly increasing x" alone) fall out of the
// stack-top-equals-y case for free.
//
// Verified against an exact minimum-set-cover brute force written from the
// statement's wording (enumerate every legal box, cover every positive column):
// exhaustively over all 5460 profiles with W <= 6 and heights <= 3, plus 1000
// random profiles up to W = 13.

#include <cstdio>

static int stk[50005];

int main() {
    int n, w;
    if (scanf("%d %d", &n, &w) != 2) return 0;
    int top = 0, cnt = 0;
    for (int i = 0; i < n; ++i) {
        int x, y;
        if (scanf("%d %d", &x, &y) != 2) break;
        while (top > 0 && stk[top - 1] > y) --top;
        if (top > 0 && stk[top - 1] == y) continue;
        stk[top++] = y;
        if (y > 0) ++cnt;
    }
    printf("%d\n", cnt);
    return 0;
}
