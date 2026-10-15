// POJ 3931 - Classmates
// Model: gpt-6.1-sol
// Reverse the Josephus elimination from the moment k people remain.
// The zero-based victim index gains two as the circle size grows, until
// it wraps to zero. Subsequent wrap sizes double. Search those intervals.
// k=1 denotes the final survivor, as required by the sample.
#include <iostream>
using namespace std;
int main() {
    long long m, k;
    while (cin >> m >> k && (m || k)) {
        if (m % 2 == 0) {
            if (k > 1 && m <= 2*k-2) cout << k+(m-2)/2 << '\n';
            else cout << "Impossible\n";
        } else {
            long long d=(m-1)/2, start=2*k-1;
            while (start <= d) start*=2;
            cout << start+d << '\n';
        }
    }
    return 0;
}
