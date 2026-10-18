// POJ 2869 - P-Networks
// Model: gpt-6-astra
// Label each input wire with its requested destination. Each adjacent
// swap moves both labels exactly as a stroke moves the corresponding paths.
// Sorting these labels therefore puts every path at its requested output.
// Repeated destinations are impossible because every stroke is a bijection.
// Bubble sort uses at most N*(N-1)/2 strokes, strictly below 4*N*N.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n;
    while (cin >> n && n) {
        vector<int> a(n), seen(n + 1, 0), strokes;
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            if (a[i] < 1 || a[i] > n) valid = false;
            else if (seen[a[i]]++) valid = false;
        }
        if (!valid) { cout << "No solution\n"; continue; }
        for (int end = n - 1; end > 0; --end)
            for (int j = 0; j < end; ++j)
                if (a[j] > a[j + 1]) {
                    swap(a[j], a[j + 1]);
                    strokes.push_back(j + 1);
                }
        cout << strokes.size();
        for (size_t i = 0; i < strokes.size(); ++i)
            cout << ' ' << strokes[i];
        cout << '\n';
    }
    return 0;
}
