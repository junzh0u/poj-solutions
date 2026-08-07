// POJ 2646 - The Trip
// Model: claude-sonnet-5
// Approach: parse each amount into integer cents (avoids all floating-point
// precision issues the discuss board repeatedly reports). Let T = sum of
// cents, n = number of students. Since costs need only be equal "within one
// cent", the fair target for each student is either base = T/n or base+1,
// with exactly r = T mod n students receiving base+1 (so the targets sum to
// T exactly). Minimizing the total money moved is a 1-D optimal-transport /
// L1-matching problem between the multiset of amounts and the multiset of
// two-valued targets: the minimum total absolute difference is achieved by
// the monotone coupling, i.e. sort amounts ascending and give the top r
// (largest) amounts the target base+1, the rest base. The amount that must
// change hands equals the sum of the positive (amount - target) differences,
// which equals half of the total absolute deviation because the targets sum
// to exactly T.
// Verified against sample 2 by hand: cents {1500,1501,300,301}, T=3602,
// n=4, base=900, r=2 -> top two (1500,1501) target 901, bottom two (300,301)
// target 900 -> transferred = 599+600 = 1199 cents = $11.99, matching the
// official sample (the naive sum(|x-avg|)/2 formula gives $12.00 instead,
// since avg=900.5 is not an integer number of cents). The discuss board has
// an unanswered question about whether amounts above $1000 use a thousands
// comma ("1,000.00"); commas are stripped defensively before parsing since
// the stated cap is $10,000.00 and this costs nothing on plain input.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
using namespace std;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int cents[1005];
        int total = 0;
        for (int i = 0; i < n; i++) {
            char buf[32];
            scanf("%s", buf);
            {
                char *src = buf, *dst = buf;
                while (*src) {
                    if (*src != ',') *dst++ = *src;
                    src++;
                }
                *dst = 0;
            }
            char *dot = strchr(buf, '.');
            int dollars = 0, frac = 0;
            if (dot) {
                *dot = 0;
                dollars = atoi(buf);
                char *fracstr = dot + 1;
                int flen = (int)strlen(fracstr);
                if (flen == 0) frac = 0;
                else if (flen == 1) frac = (fracstr[0] - '0') * 10;
                else frac = (fracstr[0] - '0') * 10 + (fracstr[1] - '0');
            } else {
                dollars = atoi(buf);
                frac = 0;
            }
            int c = dollars * 100 + frac;
            cents[i] = c;
            total += c;
        }
        sort(cents, cents + n);
        int base = total / n;
        int r = total % n;
        int ans = 0;
        for (int i = 0; i < n; i++) {
            int target = (i >= n - r) ? base + 1 : base;
            int diff = cents[i] - target;
            if (diff > 0) ans += diff;
        }
        printf("$%d.%02d\n", ans / 100, ans % 100);
    }
    return 0;
}
