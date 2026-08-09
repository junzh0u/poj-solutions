// POJ 1205 - Water Treatment Plants
// Model: claude-sonnet-5
// Approach: with n cities in a row, each city chooses a symbol V (discharge
// itself), > (send to the right neighbour) or < (send to the left
// neighbour). The only real constraints are: the leftmost city cannot be
// '<' (no left neighbour), the rightmost city cannot be '>' (no right
// neighbour), and no adjacent "><" pair (that would mean both cities try to
// use the same pipe in opposite directions at once). A city IS allowed to
// receive from both sides at once and discharge (e.g. ">V<" is valid for 3
// cities) -- a first reading that forbade this was disproved by exhaustive
// enumeration (it undercounts n=3 as 7 instead of the sample's 8). Counting
// valid strings under just those three constraints was brute-forced for
// n=1..12 and matches F(2n) exactly in every case (standard Fibonacci
// numbering, F(1)=F(2)=1), matching all three official samples too (n=2->3,
// n=3->8, n=20->102334155=F(40)). n can be up to 100 so F(200) needs bignum
// arithmetic; implemented as base-10000 little-endian digit vectors. Input
// is a sequence of integers, one per line, read to EOF (no count given).
#include <cstdio>
#include <vector>
using namespace std;

typedef vector<int> Big; // base 10000, little-endian

Big add(const Big &a, const Big &b) {
    Big c;
    int carry = 0;
    size_t n = a.size() > b.size() ? a.size() : b.size();
    for (size_t i = 0; i < n || carry; i++) {
        int x = carry;
        if (i < a.size()) x += a[i];
        if (i < b.size()) x += b[i];
        c.push_back(x % 10000);
        carry = x / 10000;
    }
    return c;
}

void printBig(const Big &a) {
    if (a.empty()) { printf("0\n"); return; }
    printf("%d", a.back());
    for (int i = (int)a.size() - 2; i >= 0; i--) printf("%04d", a[i]);
    printf("\n");
}

int main() {
    const int MAXIDX = 220;
    static Big fib[MAXIDX + 1];
    fib[0] = Big(1, 0);
    fib[1] = Big(1, 1);
    for (int i = 2; i <= MAXIDX; i++) fib[i] = add(fib[i-1], fib[i-2]);

    int n;
    while (scanf("%d", &n) == 1) {
        printBig(fib[2 * n]);
    }
    return 0;
}
