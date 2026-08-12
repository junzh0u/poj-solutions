// POJ 3863 - Business Center
// Model: gpt-5.6-terra
// For one elevator, using k upward moves leaves k*(u+d)-n*d floors.
// The first strictly positive such value is its positive residue modulo u+d.

#include <iostream>

using namespace std;

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;

    int answer = 2147483647;
    for (int i = 0; i < m; ++i) {
        int up, down;
        cin >> up >> down;
        int period = up + down;
        int residue = (n * down) % period;
        int floor = residue == 0 ? period : period - residue;
        if (floor < answer) answer = floor;
    }
    cout << answer << '\n';
    return 0;
}
