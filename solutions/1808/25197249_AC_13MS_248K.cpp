// POJ 1808 - Quadratic Residues
// Model: gpt-5.6-terra
// Euler's criterion: for nonzero a modulo odd prime p, a^((p-1)/2) is
// 1 for a quadratic residue and p-1 for a quadratic non-residue. Normalize
// negative a first, then use binary exponentiation with 64-bit products.
#include <iostream>

using namespace std;

long long mod_pow(long long a, long long e, long long mod) {
    long long result = 1;
    while (e > 0) {
        if (e & 1) result = result * a % mod;
        a = a * a % mod;
        e >>= 1;
    }
    return result;
}

int main() {
    int scenarios;
    if (!(cin >> scenarios)) return 0;
    for (int i = 1; i <= scenarios; ++i) {
        long long a, p;
        cin >> a >> p;
        a %= p;
        if (a < 0) a += p;
        long long value = mod_pow(a, (p - 1) / 2, p);
        cout << "Scenario #" << i << ":\n";
        cout << (value == 1 ? 1 : -1) << "\n\n";
    }
    return 0;
}
