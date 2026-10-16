// POJ 3937 - Parlay Wagering
// Model: gpt-6.1-sol
// Store cents and truncated multiplier thousandths as integers. Each win
// adds its truncated winnings; a tie changes nothing and any loss wipes out
// the parlay. Saturation is safe because wins never decrease the return.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    int cases;
    cin >> cases;
    while (cases--) {
        long long bet;
        int n;
        cin >> bet >> n;
        const long long cap = 100000000;
        long long cents = bet >= 1000000 ? cap : bet * 100;
        for (int i=0; i<n; ++i) {
            int line;
            string result;
            cin >> line >> result;
            if (result == "Loss") cents = 0;
            else if (result == "Win") {
                int multiplier = line > 0 ? line * 10 : 100000 / (-line);
                cents = min(cap, cents + cents * multiplier / 1000);
            }
        }
        long long dollars = cents / 100;
        string digits;
        do { digits += char('0' + dollars % 10); dollars /= 10; } while (dollars);
        cout << '$';
        for (int i=int(digits.size())-1; i>=0; --i) {
            cout << digits[i];
            if (i && i%3==0) cout << ',';
        }
        cout << '.' << char('0'+cents%100/10) << char('0'+cents%10) << '\n';
    }
}
