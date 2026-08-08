// POJ 1165 - The Primes
// Model: claude-opus-5
//
// Fill a 5x5 grid so that all 5 rows, all 5 columns and both diagonals read as
// 5-digit primes with the same digit sum S; the top-left digit is given.
//
// Search by whole lines rather than by cells, each step indexed so only
// candidates already agreeing with the fixed digits are visited:
//   row 0        primes beginning with the corner digit D
//   diagonal     ditto, grouped by its middle digit (= the centre cell)
//   anti-diagonal  keyed on (centre cell, G[0][4])
//   column 0     keyed on (D, G[4][0])
//   rows 1,3     keyed on (1st, 2nd, 4th digit), all three already fixed
//   row 2        keyed on (1st, 3rd digit)
//   row 4        keyed on (1st, 5th digit)
// Columns 1..4 are then verified, with partial columns pruned as soon as their
// 2-, 3- and 4-digit prefixes stop being prefixes of any candidate prime.
// Solutions are collected and sorted, since the enumeration order fixes only
// the first row.  A digit sum divisible by 3 admits no 5-digit prime at all,
// so most inputs die immediately.
//
// Ambiguity: "Both diagonals are read from left to right" fixes the direction
// of the anti-diagonal, which is read starting from its LEFTMOST cell, i.e.
// G[4][0] G[3][1] G[2][2] G[1][3] G[0][4] (bottom-left to top-right).  The
// official sample cannot distinguish this from the reverse reading: both
// readings are prime in all three of its solutions, and because transposing a
// grid swaps rows with columns and reverses the anti-diagonal, the two
// readings always yield the same NUMBER of solutions for a given corner - so
// even a solution count matches under both.  They differ on which squares are
// produced: for digit sum 37 and corner 9 the reverse reading yields
// 98389/77977/78787/77977/77977 where the correct reading yields
// 97777/87877/39799/87877/97777.  A data set posted on the discuss board with
// its expected output is what settles it, and it selects the left-to-right
// reading implemented here.
//
// Input framing: the statement says the file holds two lines, but the same
// board data set concatenates several cases, so the input is read to EOF.
// With a single case this is identical to reading one pair.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

static bool comp[100000];
static unsigned char dg[100000][5];        /* decimal digits, most significant first */
static vector<int> bucket[46];             /* 5-digit primes grouped by digit sum */

/* candidate lists for the current digit sum, keyed by known digit positions */
static vector<int> i1[10];                 /* by 1st                         */
static vector<int> i13[10][10];            /* by (1st, 3rd)                  */
static vector<int> i15[10][10];            /* by (1st, 5th)                  */
static vector<int> i35[10][10];            /* by (3rd, 5th)                  */
static vector<int> i124[10][10][10];       /* by (1st, 2nd, 4th)             */

static int goodTag[100000];                /* == tag iff a prime of this digit sum */
static int pf2[100], pf3[1000], pf4[10000];/* extendable 2/3/4-digit prefixes      */
static int nxt3[100];                      /* digits extending a 2-digit prefix    */
static int mask124[10][10], mask13[10];    /* which 1st digits leave a candidate   */
static int tag;

static int G[5][5];

struct Sol { int r[5]; };
static bool lessSol(const Sol& a, const Sol& b) {
    for (int i = 0; i < 5; i++) if (a.r[i] != b.r[i]) return a.r[i] < b.r[i];
    return false;
}

static void sieve() {
    comp[0] = comp[1] = true;
    for (int i = 2; i < 100000; i++) {
        if (comp[i]) continue;
        for (int j = 2 * i; j < 100000; j += i) comp[j] = true;
    }
    for (int v = 10000; v < 100000; v++) {
        int x = v, s = 0;
        for (int k = 4; k >= 0; k--) { dg[v][k] = (unsigned char)(x % 10); s += x % 10; x /= 10; }
        if (!comp[v]) bucket[s].push_back(v);
    }
}

/* Loop variables are given distinct names throughout: POJ's non-GNU C++
   compiler scopes a for-init declaration to the enclosing block, which would
   otherwise turn every reused "for (int i ...)" into a redefinition error. */
static void build(int S) {
    int qa, qb, qc, qd;
    size_t qt;
    for (qa = 0; qa < 10; qa++) {
        i1[qa].clear();
        for (qb = 0; qb < 10; qb++) {
            i13[qa][qb].clear(); i15[qa][qb].clear(); i35[qa][qb].clear();
            for (qc = 0; qc < 10; qc++) i124[qa][qb][qc].clear();
        }
    }
    tag++;
    for (qt = 0; qt < bucket[S].size(); qt++) {
        int v = bucket[S][qt];
        goodTag[v] = tag;
        pf2[v / 1000] = tag; pf3[v / 100] = tag; pf4[v / 10] = tag;
        const unsigned char* d = dg[v];
        i1[d[0]].push_back(v);
        i13[d[0]][d[2]].push_back(v);
        i15[d[0]][d[4]].push_back(v);
        i35[d[2]][d[4]].push_back(v);
        i124[d[0]][d[1]][d[3]].push_back(v);
    }
    for (qc = 0; qc < 100; qc++) {
        nxt3[qc] = 0;
        for (qd = 0; qd < 10; qd++) if (pf3[qc * 10 + qd] == tag) nxt3[qc] |= 1 << qd;
    }
    for (qb = 0; qb < 10; qb++) {
        mask13[qb] = 0;
        for (qa = 0; qa < 10; qa++) if (!i13[qa][qb].empty()) mask13[qb] |= 1 << qa;
        for (qc = 0; qc < 10; qc++) {
            mask124[qb][qc] = 0;
            for (qa = 0; qa < 10; qa++) if (!i124[qa][qb][qc].empty()) mask124[qb][qc] |= 1 << qa;
        }
    }
}

static inline int rowVal(int r) {
    return G[r][0] * 10000 + G[r][1] * 1000 + G[r][2] * 100 + G[r][3] * 10 + G[r][4];
}

int main() {
    sieve();
    int S, D, k0, rr;
    size_t oi;
    while (scanf("%d", &S) == 1 && scanf("%d", &D) == 1) {
        vector<Sol> out;
        if (S >= 1 && S <= 45 && D >= 1 && D <= 9) {
            build(S);
            const vector<int>& corner = i1[D];
            for (size_t x0 = 0; x0 < corner.size(); x0++) {          /* row 0 */
                for (k0 = 0; k0 < 5; k0++) G[0][k0] = dg[corner[x0]][k0];
                for (int mid = 0; mid < 10; mid++) {                 /* centre cell */
                    const vector<int>& av = i35[mid][G[0][4]];       /* anti-diagonal */
                    if (av.empty()) continue;
                    const int m2 = mask13[mid];
                    if (!m2) continue;
                    const vector<int>& dv = i13[D][mid];             /* main diagonal */
                    for (size_t x1 = 0; x1 < dv.size(); x1++) {
                        const unsigned char* dd = dg[dv[x1]];
                        G[1][1] = dd[1]; G[2][2] = dd[2]; G[3][3] = dd[3]; G[4][4] = dd[4];
                        for (size_t x2 = 0; x2 < av.size(); x2++) {
                            const unsigned char* da = dg[av[x2]];
                            G[4][0] = da[0]; G[3][1] = da[1]; G[1][3] = da[3];
                            const vector<int>& r4v = i15[G[4][0]][G[4][4]];   /* row 4 */
                            if (r4v.empty()) continue;
                            const int m1 = mask124[G[1][1]][G[1][3]];
                            const int m3 = mask124[G[3][1]][G[3][3]];
                            if (!m1 || !m3) continue;
                            const vector<int>& cv = i15[D][G[4][0]];          /* column 0 */
                            for (size_t x3 = 0; x3 < cv.size(); x3++) {
                                const unsigned char* dc = dg[cv[x3]];
                                if (!((m1 >> dc[1]) & 1)) continue;
                                if (!((m2 >> dc[2]) & 1)) continue;
                                if (!((m3 >> dc[3]) & 1)) continue;
                                G[1][0] = dc[1]; G[2][0] = dc[2]; G[3][0] = dc[3];
                                const vector<int>& r1v = i124[G[1][0]][G[1][1]][G[1][3]];
                                const vector<int>& r3v = i124[G[3][0]][G[3][1]][G[3][3]];
                                const vector<int>& r2v = i13[G[2][0]][G[2][2]];
                                for (size_t x4 = 0; x4 < r1v.size(); x4++) {   /* row 1 */
                                    const unsigned char* d1 = dg[r1v[x4]];
                                    G[1][2] = d1[2]; G[1][4] = d1[4];
                                    const int c1 = G[0][1] * 10 + G[1][1];
                                    const int c2 = G[0][2] * 10 + G[1][2];
                                    const int c3 = G[0][3] * 10 + G[1][3];
                                    const int c4 = G[0][4] * 10 + G[1][4];
                                    if (pf2[c1] != tag || pf2[c2] != tag ||
                                        pf2[c3] != tag || pf2[c4] != tag) continue;
                                    if (pf3[c2 * 10 + G[2][2]] != tag) continue;
                                    const int a1 = nxt3[c1], a3 = nxt3[c3], a4 = nxt3[c4];
                                    if (!a1 || !a3 || !a4) continue;
                                    for (size_t x5 = 0; x5 < r2v.size(); x5++) {   /* row 2 */
                                        const unsigned char* d2 = dg[r2v[x5]];
                                        if (!((a1 >> d2[1]) & 1)) continue;
                                        if (!((a3 >> d2[3]) & 1)) continue;
                                        if (!((a4 >> d2[4]) & 1)) continue;
                                        G[2][1] = d2[1]; G[2][3] = d2[3]; G[2][4] = d2[4];
                                        const int e1 = c1 * 10 + G[2][1];
                                        const int e2 = c2 * 10 + G[2][2];
                                        const int e3 = c3 * 10 + G[2][3];
                                        const int e4 = c4 * 10 + G[2][4];
                                        for (size_t x6 = 0; x6 < r3v.size(); x6++) {   /* row 3 */
                                            const unsigned char* d3 = dg[r3v[x6]];
                                            G[3][2] = d3[2]; G[3][4] = d3[4];
                                            const int f1 = e1 * 10 + G[3][1];
                                            const int f2 = e2 * 10 + G[3][2];
                                            const int f3 = e3 * 10 + G[3][3];
                                            const int f4 = e4 * 10 + G[3][4];
                                            if (pf4[f1] != tag || pf4[f2] != tag ||
                                                pf4[f3] != tag || pf4[f4] != tag) continue;
                                            if (goodTag[f4 * 10 + G[4][4]] != tag) continue;
                                            for (size_t x7 = 0; x7 < r4v.size(); x7++) {
                                                const unsigned char* d4 = dg[r4v[x7]];
                                                G[4][1] = d4[1]; G[4][2] = d4[2]; G[4][3] = d4[3];
                                                if (goodTag[f1 * 10 + G[4][1]] != tag) continue;
                                                if (goodTag[f2 * 10 + G[4][2]] != tag) continue;
                                                if (goodTag[f3 * 10 + G[4][3]] != tag) continue;
                                                Sol s;
                                                for (rr = 0; rr < 5; rr++) s.r[rr] = rowVal(rr);
                                                out.push_back(s);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            sort(out.begin(), out.end(), lessSol);
        }
        for (oi = 0; oi < out.size(); oi++) {
            for (rr = 0; rr < 5; rr++) printf("%d\n", out[oi].r[rr]);
            printf("\n");
        }
    }
    return 0;
}
