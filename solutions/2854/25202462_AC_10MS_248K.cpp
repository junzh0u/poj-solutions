// POJ 2854 - Educational Journey
// Model: claude-sonnet-5
//
// A, C, M drive from Calgary to Edmonton; D drives from Edmonton to Calgary,
// all at constant (individually different) speeds along the same road, so
// each car's position is an affine function of time. A passes C at t1 and M
// at t2, meets D at t3; D meets C at t4 and M at t5. Find when C passes M.
//
// Fix the coordinate so x_A(t) = t (position is only defined up to an affine
// change of units/origin, which doesn't affect crossing times). Writing
// x_C(t) = t1 + c*(t-t1), x_M(t) = t2 + m*(t-t2), x_D(t) = t3 + d*(t-t3)
// (each forced through its known crossing with A), the two D-crossing
// equations x_D(t4)=x_C(t4) and x_D(t5)=x_M(t5) each give d as an affine
// function of c and of m respectively. Eliminating d leaves one linear
// relation between the unknown slopes c and m. The desired crossing time t6
// of C and M gives a second linear relation between c and m (parameterized
// by t6). For t6 to be forced (independent of the remaining one-parameter
// freedom in c,d,m), the two relations must describe the same line, which
// pins down t6 in closed form:
//   P = (t4-t3)*(t5-t2), Q = (t5-t3)*(t4-t1)
//   t6 = (t1*P - t2*Q) / (P - Q)
// Verified against all four sample lines exactly (including the exact
// integer case 12:00:00). Implemented with exact 64-bit integer arithmetic
// (values stay well under 2^63) and round-half-up division, so there is no
// floating point precision to worry about (the discuss board reports a
// double-precision off-by-one-second pitfall in the naive formulation on
// this judge; exact integer arithmetic sidesteps it entirely).
//
// Input: lines of t1..t5 in hh:mm:ss, terminated by a line containing -1.
#include <cstdio>
#include <cstring>

static long long roundDiv(long long N, long long D) {
    if (D < 0) { N = -N; D = -D; }
    long long q = N / D;
    long long r = N % D;
    if (r < 0) { r += D; q -= 1; }
    if (2 * r >= D) q += 1;
    return q;
}

int main() {
    char buf[16];
    while (scanf("%15s", buf) == 1) {
        if (buf[0] == '-') break;
        long long t[5];
        int h, m, s;
        sscanf(buf, "%d:%d:%d", &h, &m, &s);
        t[0] = (long long)h * 3600 + m * 60 + s;
        for (int i = 1; i < 5; ++i) {
            scanf("%15s", buf);
            sscanf(buf, "%d:%d:%d", &h, &m, &s);
            t[i] = (long long)h * 3600 + m * 60 + s;
        }
        long long t1 = t[0], t2 = t[1], t3 = t[2], t4 = t[3], t5 = t[4];
        long long P = (t4 - t3) * (t5 - t2);
        long long Q = (t5 - t3) * (t4 - t1);
        long long N = t1 * P - t2 * Q;
        long long D = P - Q;
        long long ans = roundDiv(N, D);
        ans = ((ans % 86400) + 86400) % 86400;
        printf("%02lld:%02lld:%02lld\n", ans / 3600, (ans % 3600) / 60, ans % 60);
    }
    return 0;
}
