// POJ 3658 - Artificial Lake
// Model: gpt-5.6-terra
// Simulate basin filling with a doubly linked list of unsubmerged levels.
// The current basin rises to its lower surviving boundary, transfers its
// accumulated width there, and follows the downhill side to the next basin.
// All time and area arithmetic is 64-bit; this source is submitted as G++.

#include <climits>
#include <iostream>

using namespace std;

const int MAX_N = 100000;

long long width_value[MAX_N + 2];
long long answer[MAX_N + 2];
int height_value[MAX_N + 2];
int previous_level[MAX_N + 2];
int next_level[MAX_N + 2];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    if (!(cin >> n)) {
        return 0;
    }

    height_value[0] = INT_MAX;
    height_value[n + 1] = INT_MAX;
    int current = 1;

    for (int i = 1; i <= n; ++i) {
        cin >> width_value[i] >> height_value[i];
        previous_level[i] = i - 1;
        next_level[i] = i + 1;
        if (height_value[i] < height_value[current]) {
            current = i;
        }
    }
    next_level[0] = 1;
    previous_level[n + 1] = n;

    long long elapsed = 0;
    for (int submerged = 0; submerged < n; ++submerged) {
        answer[current] = elapsed + width_value[current];

        const int left = previous_level[current];
        const int right = next_level[current];
        next_level[left] = right;
        previous_level[right] = left;

        if (left == 0 && right == n + 1) {
            break;
        }

        if (height_value[left] < height_value[right]) {
            elapsed += width_value[current] *
                       static_cast<long long>(height_value[left] - height_value[current]);
            width_value[left] += width_value[current];
            current = left;
            while (previous_level[current] != 0 &&
                   height_value[previous_level[current]] < height_value[current]) {
                current = previous_level[current];
            }
        } else {
            elapsed += width_value[current] *
                       static_cast<long long>(height_value[right] - height_value[current]);
            width_value[right] += width_value[current];
            current = right;
            while (next_level[current] != n + 1 &&
                   height_value[next_level[current]] < height_value[current]) {
                current = next_level[current];
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        cout << answer[i] << '\n';
    }
    return 0;
}
