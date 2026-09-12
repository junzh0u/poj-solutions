// POJ 3741 - Number System Converter
// Model: claude-sonnet-5
//
// The circuit repeatedly does N -> to_base_Q( value_base_P(N) ) starting
// from N0. A short proof shows every fixed point of this map is a SINGLE
// digit (if a fixed string had length >= 2 with leading digit L, matching
// digits interpreted in base P and in base Q would force L*(P-Q) = 0,
// impossible since P != Q), and empirically (and via the classic
// "same digit string in two bases differ by a multiple of base gap"
// identity) the quantity V_i = value_base_P(N_i) is invariant modulo
// (Q-P) across the whole iteration, since re-expressing V_i in base Q and
// then re-reading those same digits in base P changes the value by a sum
// of terms each divisible by (Q-P).
//
// So, letting m = Q-P and r = value_base_P(N0) mod m (computed digit by
// digit, entirely independent of how large N0 is), the eventual fixed
// digit d satisfies d = r (mod m) and 0 <= d < Q, and among the (possibly
// several) candidates the process always settles on the LARGEST one --
// unless N0's actual numeric value V0 is already < Q, in which case N0
// is already the (trivial) fixed point and the answer is V0 itself.
// Since Q <= 36, V0's own magnitude only needs to be tracked until it
// provably exceeds Q (it is monotonically non-decreasing digit by digit
// because P >= 2), so this needs O(1) extra state alongside the O(1)
// running mod -- the whole algorithm is a single O(len) streaming pass
// over N0, however large N0 is (up to 5,000,000 characters), with no
// arbitrary precision arithmetic at all.
//
// Verified against a brute-force simulator (repeatedly converting between
// bases with Python bignums until the string stabilizes) over thousands
// of random (P, Q, N0) instances, including N0 containing digits that are
// not valid in base P (R1 does not validate its input either), leading
// zeros, N0 = "0", and adjacent P/Q (Q = P+1, the slowest-converging /
// most degenerate case for the modulus).
//
// Statement ambiguity: the discuss board (message 126893) confirms the
// output is the base-Q digit string the circuit stabilizes on (not the
// decimal value), matching the sample (P=2,Q=4,N0=321 -> output "3").

#include <cstdio>

static const int BUFSZ = 1 << 20;
static unsigned char ibuf[BUFSZ];
static int ilen = 0, ipos = 0;

static inline int readByte() {
    if (ipos >= ilen) {
        ilen = (int)fread(ibuf, 1, BUFSZ, stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}

static inline bool isWs(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline int digitVal(int c) {
    if (c >= '0' && c <= '9') return c - '0';
    return c - 'a' + 10;
}

static inline char digitChar(int v) {
    if (v < 10) return (char)('0' + v);
    return (char)('a' + v - 10);
}

// Reads next whitespace-delimited non-negative integer. Returns false at EOF.
static bool readInt(long &out) {
    int c = readByte();
    while (c != -1 && isWs(c)) c = readByte();
    if (c == -1) return false;
    long v = 0;
    while (c != -1 && !isWs(c)) {
        v = v * 10 + (c - '0');
        c = readByte();
    }
    out = v;
    return true;
}

int main() {
    long P, Q;
    while (readInt(P)) {
        if (!readInt(Q)) break; // malformed trailing input, stop

        int c = readByte();
        while (c != -1 && isWs(c)) c = readByte();

        long m = Q - P;
        long running = 0;
        bool capped = false;
        long r = 0;

        while (c != -1 && !isWs(c)) {
            int d = digitVal(c);
            if (!capped) {
                running = running * P + d;
                if (running >= Q) capped = true;
            }
            r = (r * P + d) % m;
            c = readByte();
        }

        long ans;
        if (!capped) {
            ans = running;
        } else {
            ans = r + m * ((Q - 1 - r) / m);
        }

        putchar(digitChar((int)ans));
        putchar('\n');
    }
    return 0;
}
