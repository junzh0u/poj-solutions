// POJ 2940 - Wine Trading in Gergovia
//
// Model: claude-opus-5
//
// n houses in a row, one unit of distance apart. House i wants to buy a[i]
// bottles (a[i] >= 0) or sell -a[i] bottles (a[i] < 0); the a[i] sum to 0.
// Moving one bottle between adjacent houses costs one unit of work, so moving
// it from house i to house j costs |i - j|. Minimize the total work over all
// ways of matching individual bottles to individual demands. Several test
// cases; a line holding just 0 ends the input.
//
// Approach: the minimum is sum over i of |prefix(i)|, where prefix(i) is the
// sum of a[0..i]. Cut the street between house i and house i+1: the houses to
// the left have a net surplus/deficit of exactly prefix(i) bottles, so at
// least |prefix(i)| bottles must cross that cut, and every crossing costs one
// unit of work. Summing the cuts gives a lower bound, and it is achievable:
// carry the running imbalance rightwards one step at a time, which pays each
// cut exactly its lower bound. So one left-to-right sweep accumulating
// |running balance| is both necessary and sufficient. O(n) time, O(1) space.
//
// Bounds: n <= 100000 and |a[i]| <= 1000, so a prefix reaches 5*10^7 and the
// answer reaches ~2.5*10^12 - well past 32 bits, as the statement warns, so
// the accumulator is a long long.
//
// Output note: POJ's "C++" language (4) is the MSVC-flavoured compiler, where
// printf("%lld") is not reliable (its spelling there is %I64d). The answer is
// non-negative, so it is printed digit by digit instead, which is correct
// under either compiler.

#include <cstdio>

static void writeLL(long long v) {
    char buf[24];
    int len = 0;
    if (v == 0) {
        buf[len++] = '0';
    } else {
        while (v > 0) {
            buf[len++] = (char)('0' + (int)(v % 10));
            v /= 10;
        }
    }
    while (len > 0) putchar(buf[--len]);
    putchar('\n');
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        long long balance = 0, work = 0;
        for (int i = 0; i < n; ++i) {
            int a;
            if (scanf("%d", &a) != 1) return 0;
            balance += a;
            work += (balance < 0 ? -balance : balance);
        }
        writeLL(work);
    }
    return 0;
}
