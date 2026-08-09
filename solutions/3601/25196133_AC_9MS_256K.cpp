// POJ 3601 - Tower of Hanoi
// Model: gpt-5.6-terra
// Keep two costs for the first i sizes: dp1 ignores the final order within
// equal-size disks, while dp2 restores it.  A size occurring once needs no
// restoration; otherwise move its block twice, with the smaller sizes moved
// around it using dp1.
#include <iostream>

using namespace std;

int main() {
    int n, mod;
    while (cin >> n >> mod) {
        int count;
        cin >> count;
        int dp1 = count % mod;
        int dp2 = (2 * (count % mod) - 1 + mod) % mod;
        for (int i = 1; i < n; ++i) {
            cin >> count;
            int previousDp1 = dp1;
            dp1 = (2 * previousDp1 + count % mod) % mod;
            if (count == 1) {
                dp2 = dp1;
            } else {
                dp2 = (2 * previousDp1 + 2 * (count % mod) + dp2) % mod;
            }
        }
        cout << dp2 << '\n';
    }
    return 0;
}
