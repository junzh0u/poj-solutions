// POJ 1897 - Data Mining
// Model: gpt-6.1-sol
// Enumerate shifts and minimize the final record's ending offset.
// Formula (4) means (x + (x << A)) >> B. For N >= 2,
// floor(Sp*(1+2^A)/2^B) >= Sq is necessary at the first gap
// and sufficient for every gap. A <= 31 suffices under the limits:
// beyond it the vanishing additive term cannot change the floor.
#include <iostream>
using namespace std;
int main() {
    long long n, p, q;
    if (!(cin >> n >> p >> q)) return 0;
    if (n == 1) { cout << q << " 0 0\n"; return 0; }
    long long best = -1;
    int ba = 0, bb = 0;
    for (int a = 0; a <= 31; ++a) {
        long long step = p * ((1LL << a) + 1);
        for (int b = 0; b <= 41; ++b) {
            if ((step >> b) < q) continue;
            long long k = ((step * (n-1)) >> b) + q;
            if (best < 0 || k < best) { best = k; ba = a; bb = b; }
        }
    }
    cout << best << ' ' << ba << ' ' << bb << '\n';
    return 0;
}
