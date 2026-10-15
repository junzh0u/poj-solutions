// POJ 3824 - Flipper
// Model: gpt-6.1-sol
// Simulate each pile from top to bottom using signed card numbers.
// Flipping reverses the moving pile and negates every card's facing.
// Its reversed cards go above the adjacent pile for either direction.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;
int main() {
    int n, tc = 0;
    while (cin >> n && n) {
        string face, moves;
        cin >> face >> moves;
        vector< vector<int> > pile(n);
        for (int i = 0; i < n; ++i)
            pile[i].push_back(face[i] == 'U' ? i + 1 : -i - 1);
        int left = 0, right = n - 1;
        for (int i = 0; i < n - 1; ++i) {
            int from = moves[i] == 'L' ? left++ : right--;
            int to = moves[i] == 'L' ? left : right;
            vector<int> joined;
            for (int j = (int)pile[from].size() - 1; j >= 0; --j)
                joined.push_back(-pile[from][j]);
            joined.insert(joined.end(), pile[to].begin(), pile[to].end());
            pile[to].swap(joined);
        }
        cout << "Pile " << ++tc << '\n';
        int m, q;
        cin >> m;
        while (m--) {
            cin >> q;
            int card = pile[left][q - 1];
            cout << "Card " << q << " is a face " << (card > 0 ? "up " : "down ")
                 << abs(card) << ".\n";
        }
    }
    return 0;
}
