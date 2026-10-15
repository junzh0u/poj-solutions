// POJ 1571 - Loansome Car Buyer
// Model: gpt-6.1-sol
// Simulate depreciation at month zero, then payments and depreciation each month.
// Missing rates inherit the previous rate; only one takes the singular suffix.
#include <iostream>
using namespace std;
int main() {
    int duration, records;
    double down, loan;
    while (cin >> duration >> down >> loan >> records && duration >= 0) {
        double rates[101];
        for (int i = 0; i <= 100; ++i) rates[i] = -1;
        for (int i = 0; i < records; ++i) {
            int month; double rate;
            cin >> month >> rate;
            if (month <= 100) rates[month] = rate;
        }
        double value = down + loan, rate = rates[0];
        int answer = duration;
        for (int month = 0; month <= duration; ++month) {
            if (rates[month] >= 0) rate = rates[month];
            value *= 1 - rate;
            double owed = duration ? loan * (duration - month) / duration : 0;
            if (owed < value) { answer = month; break; }
        }
        cout << answer << (answer == 1 ? " month\n" : " months\n");
    }
    return 0;
}
