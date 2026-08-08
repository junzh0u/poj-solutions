// POJ 2799 - IP Networks
// Model: claude-opus-5
//
// The smallest network containing all given addresses is the one whose mask
// keeps exactly the bit positions on which every address already agrees.
// Let A = AND of all addresses and O = OR of all addresses; A^O has a 1 in
// each position where the addresses disagree. Every disagreeing position, and
// everything below the highest of them, must fall inside the host part, so
// n = (index of highest set bit of A^O) + 1, mask = ~0 << n, network = A & mask.
//
// The statement's "2n IP addresses" is a lost superscript for 2^n; both
// degenerate masks are legal (255.255.255.255 for a single distinct address,
// 0.0.0.0 when the addresses span the whole space).
//
// The mask is built by shifting one bit at a time rather than as ~((1u<<n)-1),
// because n reaches 32 on the spanning case and a shift count of 32 is
// undefined - on x86 it shifts by 32%32 = 0 and yields 255.255.255.255 where
// 0.0.0.0 is correct. The discuss board reports exactly this failure, and the
// official sample does not expose it.
//
// Input is read to EOF so a file holding several data sets is handled too;
// the statement's Output section speaks of "this case".

#include <cstdio>

int main() {
    int m;
    while (scanf("%d", &m) == 1) {
        unsigned andAll = 0xFFFFFFFFu, orAll = 0u;
        for (int i = 0; i < m; ++i) {
            unsigned b0, b1, b2, b3;
            if (scanf("%u.%u.%u.%u", &b0, &b1, &b2, &b3) != 4) return 0;
            unsigned ip = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
            andAll &= ip;
            orAll |= ip;
        }
        unsigned diff = andAll ^ orAll;
        unsigned mask = 0xFFFFFFFFu;
        while (diff) { mask <<= 1; diff >>= 1; }
        unsigned net = andAll & mask;
        printf("%u.%u.%u.%u\n", (net >> 24) & 255u, (net >> 16) & 255u,
               (net >> 8) & 255u, net & 255u);
        printf("%u.%u.%u.%u\n", (mask >> 24) & 255u, (mask >> 16) & 255u,
               (mask >> 8) & 255u, mask & 255u);
    }
    return 0;
}
