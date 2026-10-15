// POJ 3379 - Alphabet
// Model: gpt-6.1-sol
// Trace the final token backward through rotations after each insertion.
// Batch transitions that do not wrap; count ancestor insertions modulo 26.
// Counting advances k tokens from the start, as in the figure.
#include <iostream>
#include <string>
using namespace std;
int main() {
    int n, k, m;
    string s;
    if (!(cin >> n >> k >> m >> s)) return 0;
    int p = 0, add = 0, t = m;
    while (t > 0) {
        if (p == 0) {
            add = (add + 1) % 26;
            p = k % (n + t - 1);
            --t;
        } else {
            int q = (n + t - p - 1) / (k + 1);
            if (q > t) q = t;
            if (q) {
                p += q * k;
                t -= q;
            } else {
                p = (p + k) % (n + t - 1);
                --t;
            }
        }
    }
    cout << char('A' + (s[p] - 'A' + add) % 26) << '\n';
    return 0;
}
