// POJ 1352 - Quantity of White Mice
// Model: gpt-6.1-sol
// Simulate age cohorts, removing deaths before the monthly capacity check.
// Each age 9 through 8+m gives two pairs per month, not every m months.
#include <iostream>
using namespace std;
int main() {
    int n, m, k;
    char comma;
    while (cin >> n && n != -1) {
        cin >> comma >> m >> comma >> k;
        int age[40] = {0};
        age[1] = 1;
        for (int month = 2; month <= k; ++month) {
            int survivors = 0;
            for (int a = n; a >= 2; --a) {
                age[a] = age[a-1];
                survivors += age[a];
            }
            age[1] = 0;
            if (survivors <= 100) {
                age[1] = age[7] + age[8];
                for (int a = 9; a <= 8+m; ++a) age[1] += 2*age[a];
            }
        }
        int total = 0;
        for (int a = 1; a <= n; ++a) total += age[a];
        cout << n << ',' << m << ',' << k << ": " << total << '\n';
    }
    return 0;
}
