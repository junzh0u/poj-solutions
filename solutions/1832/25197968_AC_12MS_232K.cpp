// POJ 1832 - 连环锁
// Model: claude-sonnet-5
//
// Chinese-rings puzzle (Baguenaudier / nine-linked-rings generalization).
// Ring 1 (rightmost) can always be toggled. Ring k+1 can be toggled iff
// ring k is on (1) and all rings before it (1..k-1) are off (0).
//
// The reachable-state graph on n rings under this rule is a single
// Hamiltonian path over all 2^n states (verified by brute-force BFS for
// n up to 7: every state is reachable and has degree <= 2). So the
// minimum number of moves between any two states s1, s2 equals
// |rank(s1) - rank(s2)|, where rank(s) is the number of moves needed to
// reach s from the all-off state (0,0,...,0).
//
// rank is built ring by ring from ring 1 upward. Let R_k be the rank of
// the k-ring prefix (rings 1..k) alone, R_0 = 0. Extending with ring k
// (0-indexed bit a[k-1], 1-indexed ring k):
//   R_k = a[k-1] ? (2^k - 1 - R_{k-1}) : R_{k-1}
// (Derived and confirmed against brute-force BFS rank tables for n<=4;
// also matches a discuss-board post claiming the answer is the
// difference of individual "moves from all-off" distances, and other
// posts confirming 64-bit ints overflow and big integers are required
// since ranks can reach ~2^127.)
//
// Since ranks can be as large as 2^127-1 (~1.7e38), we implement a
// minimal decimal big-integer (double, add-one, subtract, compare,
// print) rather than relying on any 128-bit integer extension, so the
// program stays portable C++98.
//
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<int> Big; // little-endian, base 10 digits, no leading zero digits except value 0 -> {0}

Big fromInt(int v) {
    Big b;
    if (v == 0) b.push_back(0);
    while (v > 0) { b.push_back(v % 10); v /= 10; }
    return b;
}

void trim(Big &a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

// a*2
Big doubleIt(const Big &a) {
    Big r(a.size() + 1, 0);
    int carry = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int v = a[i] * 2 + carry;
        r[i] = v % 10;
        carry = v / 10;
    }
    r[a.size()] = carry;
    trim(r);
    return r;
}

// a+1
Big addOne(const Big &a) {
    Big r = a;
    int carry = 1;
    for (size_t i = 0; i < r.size() && carry; i++) {
        int v = r[i] + carry;
        r[i] = v % 10;
        carry = v / 10;
    }
    if (carry) r.push_back(carry);
    return r;
}

// compare a,b: -1 if a<b, 0 if equal, 1 if a>b
int cmpBig(const Big &a, const Big &b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

// a-b, requires a>=b
Big subBig(const Big &a, const Big &b) {
    Big r(a.size(), 0);
    int borrow = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int bv = (i < b.size() ? b[i] : 0);
        int v = a[i] - bv - borrow;
        if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
        r[i] = v;
    }
    trim(r);
    return r;
}

void printBig(const Big &a) {
    for (int i = (int)a.size() - 1; i >= 0; i--) putchar('0' + a[i]);
    putchar('\n');
}

// compute rank of state (array of n bits, index 0 = ring1) as a Big
Big rankOf(const vector<int> &a, int n) {
    Big R = fromInt(0);   // R_0
    Big M = fromInt(1);   // M_1 = 2^1 - 1 = 1, will represent 2^k - 1 at step k
    for (int k = 1; k <= n; k++) {
        if (k > 1) M = addOne(doubleIt(M)); // M_k = 2*M_{k-1} + 1
        if (a[k - 1]) {
            R = subBig(M, R);
        }
        // else R unchanged
    }
    return R;
}

int main() {
    int m;
    if (scanf("%d", &m) != 1) return 0;
    for (int t = 0; t < m; t++) {
        int n;
        scanf("%d", &n);
        // Input lists ring n (leftmost) first down to ring 1 (rightmost)
        // last, confirmed against both official samples. Our internal
        // array a[0..n-1] wants a[0] = ring 1, so reverse on read.
        vector<int> s1(n), s2(n);
        for (int i = n - 1; i >= 0; i--) scanf("%d", &s1[i]);
        for (int i = n - 1; i >= 0; i--) scanf("%d", &s2[i]);
        Big r1 = rankOf(s1, n);
        Big r2 = rankOf(s2, n);
        int c = cmpBig(r1, r2);
        if (c >= 0) printBig(subBig(r1, r2));
        else printBig(subBig(r2, r1));
    }
    return 0;
}
