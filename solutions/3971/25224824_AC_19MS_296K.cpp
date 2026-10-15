// POJ 3971 - Scales
// Model: gpt-6.1-sol
// Count signed binary representations using carries 0 and 1, from low
// bits to high bits. A weight contributes -1, 0, or 1 to the right-minus-
// left total. After all N weights, only zero carry balances the stone.
#include <iostream>
#include <string>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        int n, l, d;
        string w;
        cin >> n >> l >> d >> w;
        int zero = 1, one = 0;
        for (int i = 0; i < n; ++i) {
            int sum = (zero + one) % d;
            int bit = i < l ? w[l - 1 - i] - '0' : 0;
            if (bit == 0) zero = sum;
            else one = sum;
        }
        cout << zero << '\n';
    }
    return 0;
}
