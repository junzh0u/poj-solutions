// POJ 2417 - Discrete Logging
// Model: gpt-5.6-terra
// Baby-step/giant-step: store B^j for one block, then multiply N by
// (B^m)^-1 repeatedly.  The least matching i*m+j is the least exponent.
// P is prime, but B need not generate all nonzero residues, so a miss means
// N is outside B's subgroup.  In particular, N == 1 has answer 0.
#include <cstdio>
#include <vector>

typedef long long int64;

static int64 mod_pow(int64 base, int64 exponent, int64 mod) {
    int64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % mod;
        base = base * base % mod;
        exponent >>= 1;
    }
    return result;
}

static unsigned int hash_value(int64 value) {
    return (unsigned int)((unsigned long long)value * 2654435761U);
}

static int discrete_log(int64 p, int64 b, int64 n) {
    int m = 1;
    while ((int64)m * m < p - 1) ++m;

    int capacity = 1;
    while (capacity < 2 * m) capacity <<= 1;
    std::vector<int64> keys(capacity, -1);
    std::vector<int> values(capacity, -1);
    int mask = capacity - 1;

    int64 value = 1;
    int j;
    for (j = 0; j < m; ++j) {
        int slot = (int)(hash_value(value) & mask);
        while (keys[slot] != -1 && keys[slot] != value)
            slot = (slot + 1) & mask;
        if (keys[slot] == -1) {
            keys[slot] = value;
            values[slot] = j;
        }
        value = value * b % p;
    }

    int64 inverse_block = mod_pow(value, p - 2, p);
    int64 current = n;
    int answer = -1;
    int i;
    for (i = 0; i <= m; ++i) {
        int slot = (int)(hash_value(current) & mask);
        while (keys[slot] != -1 && keys[slot] != current)
            slot = (slot + 1) & mask;
        if (keys[slot] == current) {
            int64 candidate = (int64)i * m + values[slot];
            if (candidate < p - 1 && (answer == -1 || candidate < answer))
                answer = (int)candidate;
        }
        current = current * inverse_block % p;
    }
    return answer;
}

int main() {
    int64 p, b, n;
    while (std::scanf("%lld%lld%lld", &p, &b, &n) == 3) {
        int answer = discrete_log(p, b, n);
        if (answer == -1) std::puts("no solution");
        else std::printf("%d\n", answer);
    }
    return 0;
}
