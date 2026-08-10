// POJ 1392 - Ouroboros Snake
// Model: gpt-5.6-terra
// Generate the lexicographically smallest binary de Bruijn circle by
// concatenating, in order, all binary Lyndon words whose lengths divide n.
// Answer each query by reading its cyclic n-bit window as a binary integer.

#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

int working[32];
vector<int> circle_bits[16];

void generate_lyndon(int position, int period, int n) {
    if (position > n) {
        if (n % period == 0) {
            for (int i = 1; i <= period; ++i) {
                circle_bits[n].push_back(working[i]);
            }
        }
        return;
    }

    working[position] = working[position - period];
    generate_lyndon(position + 1, period, n);

    for (int bit = working[position - period] + 1; bit <= 1; ++bit) {
        working[position] = bit;
        generate_lyndon(position + 1, position, n);
    }
}

int main() {
    for (int n = 1; n <= 15; ++n) {
        memset(working, 0, sizeof(working));
        generate_lyndon(1, 1, n);
    }

    int n;
    int k;
    while (cin >> n >> k && (n != 0 || k != 0)) {
        const int length = 1 << n;
        int value = 0;
        for (int offset = 0; offset < n; ++offset) {
            value = (value << 1) |
                    circle_bits[n][(k + offset) % length];
        }
        cout << value << '\n';
    }
    return 0;
}
