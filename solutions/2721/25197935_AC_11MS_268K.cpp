// POJ 2721 - Suit Distribution
// Model: claude-sonnet-5
// Approach: Given a,b (0<=a,b<=13, a+b<=13), the two opponents hold a+b=n
// cards of the suit in question among their combined 26 unseen cards (the
// other 26-n of those unseen cards are non-suit; your team holds the
// remaining 13-n suit cards, which is irrelevant once n is fixed). West's
// 13 cards are a uniformly random 13-subset of those 26 unseen cards, so
//   P(West has x suit cards) = C(n,x)*C(26-n,13-x) / C(26,13).
// East then automatically holds n-x suit cards. The requested "a-b split"
// probability is P(West=a,East=b) plus, when a!=b, the mirror-image
// P(West=b,East=a); when a==b there is only one term (verified against all
// three sample lines, matching to 8 decimals). C(26,13)=10400600 and every
// product of two such combinations stays under 6e6, comfortably inside a
// 32-bit long (no long long needed, so this also works under POJ's old
// MSVC language 4) -- the discuss board's main reported pitfall was
// integer overflow from a naive combination formula.
#include <cstdio>
#include <cstring>

static long C[27][27];

int main() {
    for (int n = 0; n <= 26; ++n) {
        C[n][0] = 1;
        for (int k = 1; k <= n; ++k) {
            C[n][k] = C[n-1][k-1] + (k <= n-1 ? C[n-1][k] : 0);
        }
    }

    int a, b;
    long denom = C[26][13];
    while (scanf("%d %d", &a, &b) == 2) {
        if (a == -1 && b == -1) break;
        int n = a + b;
        long num = C[n][a] * C[26-n][13-a];
        if (a != b) num += C[n][b] * C[26-n][13-b];
        double prob = (double)num / (double)denom;
        printf("%d-%d split: %.8f\n", a, b, prob);
    }
    return 0;
}
