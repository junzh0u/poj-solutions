// POJ 1215 - Gondwanaland Telecom
// Model: claude-sonnet-5
// Approach: parse each call (charge step, number, start hh mm, end hh mm).
// Convert start/end to minutes-of-day using the literal integers given
// (the board's test data uses "24 10" as a valid end time, so hours are
// NOT reduced mod 24 before comparison); if end <= start the call wraps
// past midnight so add 1440 minutes to end. Then walk every minute of the
// call and classify it (mod 1440) into day [8:00,18:00), evening
// [18:00,22:00) or night [22:00,8:00) buckets, accumulate per-category
// minute counts, and price them with the step's three rates. Output format
// "%10s%6d%6d%6d%3c%8.2f" was confirmed against the official Discuss board
// (message 1730) and cross-checked against ~40 worked examples posted in
// message 96854, including the "24 10" style end time and the exact-lap
// (start==end) 24-hour call.
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;

int main() {
    static const double dayRate[5]     = {0.10, 0.25, 0.53, 0.87, 1.44};
    static const double eveningRate[5] = {0.06, 0.15, 0.33, 0.47, 0.80};
    static const double nightRate[5]   = {0.02, 0.05, 0.13, 0.17, 0.30};

    char step;
    while (cin >> step) {
        if (step == '#') break;
        string phone;
        int h1, m1, h2, m2;
        cin >> phone >> h1 >> m1 >> h2 >> m2;

        int start = h1 * 60 + m1;
        int end = h2 * 60 + m2;
        if (end <= start) end += 1440;

        int day = 0, evening = 0, night = 0;
        for (int m = start; m < end; ++m) {
            int t = m % 1440;
            if (t >= 480 && t < 1080) ++day;
            else if (t >= 1080 && t < 1320) ++evening;
            else ++night;
        }

        int idx = step - 'A';
        double cost = day * dayRate[idx] + evening * eveningRate[idx] + night * nightRate[idx];
        printf("%10s%6d%6d%6d%3c%8.2f\n", phone.c_str(), day, evening, night, step, cost);
    }
    return 0;
}
