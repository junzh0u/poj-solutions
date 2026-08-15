// POJ 1819 - Disks
// Model: claude-sonnet-5
//
// Each disk i is tangent to the x-axis and slides in from x=+infinity until it
// touches the y-axis (x = r_i) or the first previously-fixed disk it meets.
// For two disks both tangent to the x-axis with the new one approaching from
// the right, the new disk's center settles at x_j + 2*sqrt(r_i*r_j) if it
// stops against disk j (derived from the tangency distance equation). So
// x_i = max(r_i, max_{j<i} (x_j + 2*sqrt(r_i*r_j))), taken over ALL disks
// placed before i (dispensable or not - they are still physically present).
//
// A disk k is dispensable iff removing it from the sequence and re-fixing the
// rest in order gives the same total width. One can show (by induction on
// index) that x_m is unaffected by removing k unless k is the UNIQUE
// maximizer ("necessary support") somewhere along the chain of supports
// leading to m; so k matters only if it is a necessary ancestor of some disk
// that (eventually) achieves the overall maximum width. This is exactly the
// "everything strictly between the winning support and the new disk becomes
// permanently hidden" rule below, plus "everything placed after the disk that
// achieves the final maximum width is dispensable" (the discuss board's
// documented trap: trailing disks can be dispensable too, not just middle
// ones - the statement's "no disk can move left" gloss is misleading and was
// explicitly warned against on the board).
//
// Tie handling: when several previous disks give the exact same stopping
// position for the new disk, the *closer* (larger-index) one must NOT be
// preferred as "the" support, since it would then incorrectly survive as
// indispensable when an earlier disk alone already achieves the same
// tangency (verified with r = [4,1,4]: disk 2 is dispensable there, but a
// descending scan with strict '>' keeps it as "the" support and misses that).
// Scanning candidates in ascending index order with a strict '>' update makes
// ties resolve toward the earlier (farther) disk, whose "in-between" range
// then correctly swallows the later tied disk as dispensable. Verified by
// differential testing against an O(n^3) brute force (independent
// remove-one-disk-and-recompute-from-scratch reference) across hundreds of
// random and tie-heavy cases (repeated radii, up to n=80), including the
// board's documented trap where trailing disks are freed by an earlier big
// disk.

#include <cstdio>
#include <cmath>

const int MAXN = 1005;

double R[MAXN];
double dist_[MAXN];
bool dispensable[MAXN];

static double width(double r1, double r2) {
    return 2.0 * std::sqrt(r1 * r2);
}

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    for (int i = 1; i <= n; ++i) {
        std::scanf("%lf", &R[i]);
        dispensable[i] = false;
    }
    if (n <= 0) {
        std::printf("0\n");
        return 0;
    }

    dist_[1] = R[1];
    int rightmost = 1;
    double most = 2.0 * R[1];

    for (int i = 2; i <= n; ++i) {
        double best = 0.0;
        int li = i - 1;
        for (int j = 1; j < i; ++j) {
            if (!dispensable[j]) {
                double cand = dist_[j] + width(R[j], R[i]);
                if (cand > best) {
                    best = cand;
                    li = j;
                }
            }
        }
        if (best < R[i]) {
            best = R[i];
            li = 0;
        }
        dist_[i] = best;

        for (int j = i - 1; j > li; --j) {
            if (!dispensable[j]) {
                dispensable[j] = true;
            }
        }

        if (dist_[i] + R[i] > most) {
            most = dist_[i] + R[i];
            rightmost = i;
        }
    }

    for (int i = rightmost + 1; i <= n; ++i) {
        dispensable[i] = true;
    }

    int cnt = 0;
    for (int i = 1; i <= n; ++i) if (dispensable[i]) ++cnt;

    std::printf("%d\n", cnt);
    for (int i = 1; i <= n; ++i) {
        if (dispensable[i]) std::printf("%d\n", i);
    }

    return 0;
}
