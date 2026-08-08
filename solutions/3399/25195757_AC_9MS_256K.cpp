// POJ 3399 - Product
// Model: claude-opus-5
//
// Pick K of N integers (|a_i| <= 30000, N <= 100) with maximal product and
// print them in non-increasing order (special judge).
//
// Sort ascending.  The product is never formed, so no big integers are
// needed: every decision compares two-element products, which fit in int
// (30000*30000 = 9e8 < 2^31).
//
//  * If K is odd and every element is negative, the product is forced
//    negative, so maximising it means minimising its magnitude: take the K
//    largest (smallest-magnitude) elements.  This is the branch the official
//    sample cannot reach and the discuss board keeps reporting as WA.
//  * Otherwise the best product is >= 0 and the largest element is >= 0.  If
//    K is odd take that largest element first (exchanging any other choice
//    for it cannot decrease the product; if it is 0 the answer is 0 anyway),
//    leaving an even count.  Then repeatedly take whichever is larger, the
//    product of the two smallest remaining or of the two largest remaining.
//
// Input framing: one data set, exactly as the statement specifies ("The input
// consists of N+1 lines").  A 2008 board post claiming multiple data sets was
// rebutted there, a later post warns that reading to EOF fails, and the AC
// program posted on the board reads N and K once.
#include <cstdio>
#include <algorithm>
using namespace std;

int a[105];
int res[105];

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    sort(a, a + n);

    int m = 0;
    if ((k & 1) && a[n - 1] < 0) {
        for (int i = n - 1; i >= n - k; i--) res[m++] = a[i];
    } else {
        int i = 0, j = n - 1, need = k;
        if (need & 1) { res[m++] = a[j]; j--; need--; }
        while (need > 0) {
            int p1 = a[i] * a[i + 1];
            int p2 = a[j] * a[j - 1];
            if (p1 >= p2) { res[m++] = a[i]; res[m++] = a[i + 1]; i += 2; }
            else          { res[m++] = a[j]; res[m++] = a[j - 1]; j -= 2; }
            need -= 2;
        }
    }

    sort(res, res + m);
    for (int t = m - 1; t >= 0; t--)
        printf("%d%c", res[t], t == 0 ? '\n' : ' ');
    return 0;
}
