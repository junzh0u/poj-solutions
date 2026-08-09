// POJ 1289 - The Cat in the Hat
// Model: claude-sonnet-5
// Approach: The cats form a uniform tree: every non-smallest cat has
// exactly N children, and a child's height is 1/(N+1) times its parent's
// height. So if the tree has L levels below the root (root = level 0,
// leaves = level L, all leaves height 1), then H = (N+1)^L exactly and the
// leaf/worker count W = N^L. Given H and W, search L = 1..62 (2^62 already
// dwarfs any 64-bit H) for an integer d = N+1 with d^L == H, found by
// integer binary search on d in [2, H] using an overflow-safe power
// (multiply-and-cap), so no floating point / pow() precision issues. For
// each such d, check whether (d-1)^L == W; the first match (guaranteed
// unique by the problem's construction) gives N and L, and the answer is
// computed by directly simulating the L levels (non-working count = sum of
// per-level population before the leaves; height sum = sum of
// height*population per level, including the leaves).
// Ambiguity: statement's N is the number of *children* per cat, while the
// height ratio uses N+1 - i.e. children count and height-divisor are off
// by one. Verified against both sample cases (216,125)->(31,671) with
// N=5,L=3 and (5764801,1679616)->(335923,30275911) with N=6,L=8; the naive
// reading where child count also equals the height divisor fails both
// samples. Board (showmessage 179437, 60229) confirms the same
// (N+1)^L=H, N^L=W formulation and flags the H=1,W=1 -> "0 1" edge case
// (showmessage 80846 et al.), which is naturally covered here since the
// L=0 (no levels) case means the single root cat is itself the only
// (already smallest) cat.

#include <iostream>
using namespace std;

typedef long long ll;

// returns base^exp, or cap+1 if it would exceed cap. Checks for overflow
// *before* multiplying (r > cap/base implies r*base > cap), since both r
// and base can independently be close to the 64-bit range here and their
// product would otherwise overflow before any post-hoc check could catch it.
ll safePow(ll base, int exp, ll cap) {
    ll r = 1;
    for (int i = 0; i < exp; ++i) {
        if (r > cap / base) return cap + 1;
        r *= base;
    }
    return r;
}

int main() {
    ll H, W;
    while (cin >> H >> W) {
        if (H == 0 && W == 0) break;

        if (H == 1) {
            // Single cat, already the smallest; it does the work itself.
            cout << "0 1\n";
            continue;
        }

        ll foundD = -1;
        int foundL = -1;
        for (int L = 1; L <= 62; ++L) {
            // binary search d in [2, H] with d^L == H
            ll lo = 2, hi = H;
            ll d = -1;
            while (lo <= hi) {
                ll mid = lo + (hi - lo) / 2;
                ll p = safePow(mid, L, H);
                if (p == H) { d = mid; break; }
                else if (p < H) lo = mid + 1;
                else hi = mid - 1;
            }
            if (d == -1) continue;
            ll worker = safePow(d - 1, L, W > H ? W : H);
            if (worker == W) {
                foundD = d;
                foundL = L;
                break;
            }
        }

        // Problem guarantees a valid (N, L); simulate to get the answer.
        ll d = foundD;
        int L = foundL;
        ll h = H, count = 1;
        ll nonworking = 0, sumH = 0;
        ll N = d - 1;
        for (int i = 0; i < L; ++i) {
            nonworking += count;
            sumH += h * count;
            h /= d;
            count *= N;
        }
        sumH += h * count; // leaves, h should be 1 here

        cout << nonworking << " " << sumH << "\n";
    }
    return 0;
}
