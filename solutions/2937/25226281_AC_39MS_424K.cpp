// POJ 2937 - Dihedral Groups
// Model: gpt-6-astra
// Track the clockwise position of point zero and reflection parity.
// A shortest word is r^a, m r^a m, m r^a, or r^a m.
// Minimize elementary operations, not the number of abbreviated blocks.
// The identity is represented by an empty output line.
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    string line;
    while (cin >> n && n) {
        getline(cin, line);
        getline(cin, line);
        istringstream in(line);
        char op;
        int count, p = 0, mirror = 0;
        while (in >> op >> count) {
            if (op == 'r') p = (p + count % n) % n;
            else if (count % 2) {
                p = (n - p) % n;
                mirror ^= 1;
            }
        }
        if (!mirror) {
            if (p != 0) {
                if (p <= n - p + 2) cout << 'r' << p;
                else cout << "m1 r" << n - p << " m1";
            }
        } else if (p == 0) cout << "m1";
        else if (p <= n - p) cout << "m1 r" << p;
        else cout << 'r' << n - p << " m1";
        cout << '\n';
    }
    return 0;
}
