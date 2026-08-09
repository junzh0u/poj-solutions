// POJ 2323 - PERMS
// Model: gpt-5.6-terra
// Approach: insert the largest value into a smaller permutation. Its position
// contributes any number of inversions from zero through the old permutation's
// size, giving a short prefix-sum dynamic program.
#include <iostream>

using namespace std;

long long ways[19][201];

int main() {
    ways[0][0] = 1;
    for (int n = 1; n <= 18; ++n) {
        for (int inversions = 0; inversions <= 200; ++inversions) {
            long long total = 0;
            for (int added = 0; added < n && added <= inversions; ++added)
                total += ways[n - 1][inversions - added];
            ways[n][inversions] = total;
        }
    }

    int n, k;
    while (cin >> n >> k && (n != 0 || k != 0))
        cout << ways[n][k] << '\n';
    return 0;
}
