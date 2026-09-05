// POJ 3495 - Bitwise XOR of Arithmetic Progression
// Model: claude-sonnet-5
//
// We need XOR_{i=0}^{k} (x + i*z), k = floor((y-x)/z), with x,y,z < 2^32.
// k (hence the number of terms n=k+1) can itself be up to ~2^32, so we cannot
// iterate over terms; we need a bit-by-bit approach.
//
// For a fixed bit position p, the parity of the count of terms with bit p set
// determines whether bit p is set in the XOR (XOR of a bit column = parity of
// how many 1s are in that column). Writing bit_p(v) = floor(v/2^p) - 2*floor(v/2^(p+1)),
// summing over all n terms gives:
//   C_p = S(p) - 2*S(p+1),  where S(p) = sum_{i=0}^{n-1} floor((x+i*z)/2^p).
// Hence C_p mod 2 = S(p) mod 2 (the -2*S(p+1) term is always even).
//
// So for each bit p we just need the parity of the classic "floor sum"
//   S(p) = sum_{i=0}^{n-1} floor((z*i+x)/2^p),
// which is computable in O(log) time with the standard Euclidean-like
// floor_sum algorithm (as in AtCoder Library). We only need the final value
// mod 2; since every operation used (+, -, *) is compatible with reduction
// mod 2^64, and 2 | 2^64, doing the whole computation in uint64_t and letting
// additions/multiplications silently wrap is still exactly correct mod 2
// (only the accumulator "ans" can overflow -- the control-flow variables
// n, m, a, b stay provably within a safe range that never overflows a
// uint64_t, since n is non-increasing and a,b stay bounded by the shrinking
// modulus m). So no big-integer type is needed at all.
//
// Terms are all <= y < 2^32, so only bits 0..31 can ever be set; 32 bits
// fully cover the answer.
//
// Ambiguity: none found; statement and board (message 181626/181627, an
// accepted O(log^2) solution) confirm the intended complexity is
// O(log(y) * log(y)), matching this approach (32 bits, each an O(log)
// floor-sum). Verified against a brute force by direct simulation for many
// small random cases and against the sample.

#include <cstdio>
typedef unsigned long long u64;

// sum_{i=0}^{n-1} floor((a*i+b)/m), for n,m,a,b >= 0, m >= 1.
// We only need the *parity* of the true mathematical value; the uint64
// wraparound of the accumulator does not affect that parity (see comment
// above), so plain unsigned arithmetic suffices.
static u64 floor_sum(u64 n, u64 m, u64 a, u64 b) {
    u64 ans = 0;
    while (true) {
        if (a >= m) {
            ans += n * (n - 1) / 2 * (a / m);
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        u64 y_max = a * n + b;
        if (y_max < m) break;
        n = y_max / m;
        b = y_max % m;
        u64 tmp = m;
        m = a;
        a = tmp;
    }
    return ans;
}

int main() {
    unsigned int xi, yi, zi;
    while (scanf("%u %u %u", &xi, &yi, &zi) == 3) {
        u64 x = xi, y = yi, z = zi;
        u64 k = (y - x) / z;
        u64 n = k + 1; // number of terms
        u64 result = 0;
        for (int p = 0; p < 32; ++p) {
            u64 m = (u64)1 << p;
            u64 s = floor_sum(n, m, z, x);
            if (s & 1ULL) result |= ((u64)1 << p);
        }
        printf("%u\n", (unsigned int)result);
    }
    return 0;
}
