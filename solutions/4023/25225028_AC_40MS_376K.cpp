// POJ 4023 - Binary Encoding
// Model: gpt-6.1-sol
// Use 2^n-m short codes, followed by the remaining n-bit leaves.
// POJ's statement is blank; the original NEERC 2011 problem B specifies
// one m (2..10000), ordered prefix-free codes of minimum total length.
#include <iostream>
using namespace std;
int main() {
    int m;
    if (!(cin >> m)) return 0;
    int n = 0, capacity = 1;
    while (capacity < m) { capacity *= 2; ++n; }
    int shortCount = capacity - m;
    for (int i = 0; i < m; ++i) {
        int value = i < shortCount ? i : i + shortCount;
        int length = i < shortCount ? n - 1 : n;
        for (int bit = length - 1; bit >= 0; --bit)
            cout << ((value >> bit) & 1);
        cout << '\n';
    }
    return 0;
}
