// POJ 2232 - New Stone-Forfex-Cloth Game
// Model: gpt-5.6-terra
// Count the gesture types.  With all three present every child can be made
// champion; with two types, every comparison eventually leaves the type that
// beats the other.  The discussion board confirms that a new player is chosen
// before every elimination, rather than once for the whole game.
#include <iostream>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        int cntS = 0, cntF = 0, cntC = 0;
        for (int i = 0; i < n; ++i) {
            char ch;
            cin >> ch;
            if (ch == 'S') ++cntS;
            else if (ch == 'F') ++cntF;
            else ++cntC;
        }
        if (cntS && cntF && cntC) {
            cout << n << '\n';
        } else if (!cntF && !cntC) {
            cout << cntS << '\n';
        } else if (!cntS && !cntC) {
            cout << cntF << '\n';
        } else if (!cntS && !cntF) {
            cout << cntC << '\n';
        } else if (!cntC) {
            cout << cntS << '\n'; // Stone beats Forfex.
        } else if (!cntF) {
            cout << cntC << '\n'; // Cloth beats Stone.
        } else {
            cout << cntF << '\n'; // Forfex beats Cloth.
        }
    }
    return 0;
}
