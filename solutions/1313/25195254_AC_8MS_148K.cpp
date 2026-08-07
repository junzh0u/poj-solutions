// POJ 1313 - Booklet Printing
// Model: claude-opus-5
//
// Approach: a fold-over booklet rounds the page count up to a multiple of 4,
// so total = 4 * ceil(n / 4) and sheets = total / 4.  Folding sheet i (1-based)
// puts pages (total - 2i + 2) and (2i - 1) on the front and (2i) and
// (total - 2i + 1) on the back.  Any page number greater than n prints as
// "Blank"; a side whose two slots are both blank is not printed at all (only
// ever the back of sheet 1, when n == 1, but the check is written generally).
//
// Formatting is the whole difficulty here, and the discuss board confirms it:
// the literal is "back : " with a space before the colon so it lines up with
// "front:", and there is no blank line between test cases.  n <= 100, so plain
// int is ample.  Input is a sequence of test cases ending with a 0.

#include <cstdio>

static void slot(int page, int n, int first) {
    if (!first) printf(", ");
    if (page > n) printf("Blank");
    else printf("%d", page);
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int total = ((n + 3) / 4) * 4;
        int sheets = total / 4;
        printf("Printing order for %d pages:\n", n);
        for (int i = 1; i <= sheets; ++i) {
            int fl = total - 2 * i + 2;   /* front left  */
            int fr = 2 * i - 1;           /* front right */
            int bl = 2 * i;               /* back left   */
            int br = total - 2 * i + 1;   /* back right  */

            if (fl <= n || fr <= n) {
                printf("Sheet %d, front: ", i);
                slot(fl, n, 1);
                slot(fr, n, 0);
                printf("\n");
            }
            if (bl <= n || br <= n) {
                printf("Sheet %d, back : ", i);
                slot(bl, n, 1);
                slot(br, n, 0);
                printf("\n");
            }
        }
    }
    return 0;
}
